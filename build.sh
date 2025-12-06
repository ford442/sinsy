#!/bin/bash
source /content/build_space/emsdk/emsdk_env.sh # Uncomment if running in an environment where this is needed

# 1. Define an installation path for the Wasm dependencies
# We use $(pwd)/wasm_install to store the compiled hts_engine_API locally
INSTALL_DIR="$(pwd)/wasm_install"
mkdir -p "$INSTALL_DIR"

echo "=== Building hts_engine_API ==="
cd hts_engine_API

# Generate configuration files for HTS engine
chmod +x configure
autoreconf -i

# Configure HTS engine to build as a static library for Emscripten
# --prefix tells it to install into our local folder, not /usr/local
emconfigure ./configure --prefix="$INSTALL_DIR" --host=wasm32-unknown-emscripten

# Compile and install HTS engine
emmake make -j55
emmake make install

cd ..

echo "=== Building Sinsy ==="
# Generate configuration files for Sinsy
chmod +x configure
autoreconf -i

# Configure Sinsy to use the HTS engine we just built
# We point --with-hts-engine-* to our local install directory
emconfigure ./configure \
  --with-hts-engine-header-path="$INSTALL_DIR/include" \
  --with-hts-engine-library-path="$INSTALL_DIR/lib" \
  --host=wasm32-unknown-emscripten

# Compile Sinsy
emmake make -j55

echo "Build complete."
