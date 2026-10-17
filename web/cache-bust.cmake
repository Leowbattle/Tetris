# Splices a content-derived version into the generated page and its asset URLs.
#
# Emscripten emits a fixed set of filenames (index.js, index.wasm, index.data),
# and hosting providers cache index.js for hours. After a deployment a browser
# can therefore pair a fresh index.html with a cached index.js from an older
# build. When the shell then calls an export that only exists in the new build
# (for example setTouchAction), the on-screen controls silently do nothing.
#
# Appending ?v=<hash> to the script URL and to Module.locateFile() gives every
# build a distinct cache key, so the page and its runtime always match.
#
# Usage: cmake -DINDEX_HTML=<path to index.html> -P cache-bust.cmake

if(NOT INDEX_HTML)
    message(FATAL_ERROR "INDEX_HTML is required")
endif()

get_filename_component(ASSET_DIR "${INDEX_HTML}" DIRECTORY)

# Hash every generated artifact so the version changes whenever any of them
# does, not just when index.js happens to change.
set(asset_hashes "")
foreach(asset index.js index.wasm index.data)
    if(EXISTS "${ASSET_DIR}/${asset}")
        file(SHA256 "${ASSET_DIR}/${asset}" asset_hash)
        string(APPEND asset_hashes "${asset_hash}")
    endif()
endforeach()

if(NOT asset_hashes)
    message(FATAL_ERROR "No Emscripten artifacts found next to ${INDEX_HTML}")
endif()

string(SHA256 build_hash "${asset_hashes}")
string(SUBSTRING "${build_hash}" 0 12 BUILD_VERSION)

file(READ "${INDEX_HTML}" html)

# The script tag is the only place the page itself names an asset; index.wasm
# and index.data are resolved at runtime through Module.locateFile().
string(REPLACE "src=\"index.js\"" "src=\"index.js?v=${BUILD_VERSION}\"" html "${html}")
string(REPLACE "src='index.js'" "src='index.js?v=${BUILD_VERSION}'" html "${html}")
string(REPLACE "src=index.js" "src=index.js?v=${BUILD_VERSION}" html "${html}")

# Consumed by Module.locateFile() in the page shell.
string(REPLACE "__BUILD_VERSION__" "${BUILD_VERSION}" html "${html}")

file(WRITE "${INDEX_HTML}" "${html}")

message(STATUS "Stamped web build ${BUILD_VERSION}")
