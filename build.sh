#!/bin/bash
source /content/build_space/emsdk/emsdk_env.sh # Uncomment if running in an environment where this is needed
# Exit immediately if a command exits with a non-zero status
set -e

# Define installation path for the Wasm dependencies
INSTALL_DIR="$(pwd)/wasm_install"
mkdir -p "$INSTALL_DIR"

# Helper function to update config.sub and config.guess
update_config_scripts() {
    echo "Updating config.sub and config.guess in $(pwd)/config..."
    # Download latest config.guess and config.sub from GNU savannah
    curl -o config/config.guess 'https://git.savannah.gnu.org/gitweb/?p=config.git;a=blob_plain;f=config.guess;hb=HEAD'
    curl -o config/config.sub 'https://git.savannah.gnu.org/gitweb/?p=config.git;a=blob_plain;f=config.sub;hb=HEAD'
    chmod +x config/config.guess config/config.sub
}

echo "=== Building hts_engine_API ==="
cd hts_engine_API

# Generate configure script
autoreconf -i

# FIX: Update config scripts to recognize 'emscripten'
update_config_scripts

# Configure and build
emconfigure ./configure --prefix="$INSTALL_DIR" --host=wasm32-unknown-emscripten
emmake make -j$(nproc)
emmake make install

cd ..

echo "=== Building Sinsy ==="
# Generate configure script
chmod +x configure
autoreconf -i

# FIX: Update config scripts to recognize 'emscripten'
update_config_scripts

# Configure Sinsy using the local HTS engine build
emconfigure ./configure \
  --with-hts-engine-header-path="$INSTALL_DIR/include" \
  --with-hts-engine-library-path="$INSTALL_DIR/lib" \
  --host=wasm32-unknown-emscripten

# Compile Sinsy
emmake make -j$(nproc)

echo "Build complete."
