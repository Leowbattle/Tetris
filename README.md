# Tetris

A Tetris game made in C using SDL2.

Play on itch.io: https://leowbattle.itch.io/tetris

## Building

The game can be built for the desktop with CMake, or for the browser with
Emscripten. The existing `Tetris.sln` is still available for Visual Studio.

### Desktop (Linux)

Requires a C compiler, CMake, pkg-config and the SDL2 development packages:

```sh
# Debian / Ubuntu
sudo apt-get install build-essential cmake pkg-config \
    libsdl2-dev libsdl2-ttf-dev

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

The game loads `resources/...` relative to the current working directory. The
build copies the resources next to the binary, so run it from the build
directory:

```sh
cd build
./tetris
```

To produce a self-contained folder for distribution:

```sh
cmake --install build --prefix dist
cd dist
./tetris
```

### Web (Emscripten)

Install and activate the
[emsdk](https://emscripten.org/docs/getting_started/downloads.html), then:

```sh
emcmake cmake -B build-web -DCMAKE_BUILD_TYPE=Release
cmake --build build-web --parallel
```

This produces `build-web/index.html`, `index.js`, `index.wasm` and `index.data`.
The build preloads `Tetris/resources` into the virtual filesystem at
`/resources`, so no extra files are needed at runtime.

A `file://` URL will not work because the `.wasm`/`.data` files are fetched over
HTTP — serve the directory instead:

```sh
python3 -m http.server -d build-web 8000
# then open http://localhost:8000/index.html
```

## Continuous integration

`.github/workflows/build.yml` builds both targets on every push and pull
request and uploads them as workflow artifacts:

- **`Tetris-linux-x86_64`** — a `.tar.gz` containing the binary and its
  `resources` directory.
- **`Tetris-web`** — a `.zip` containing `index.html`, `index.js`, `index.wasm`
  and `index.data`.

On pushes to `master`, the web build is also published to GitHub Pages. Enable
this once under **Settings → Pages → Build and deployment → Source: GitHub
Actions**.
