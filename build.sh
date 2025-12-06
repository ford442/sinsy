#!/bin/bash
source /content/build_space/emsdk/emsdk_env.sh # Uncomment if running in an environment where this is needed

set -e # Exit immediately if a command exits with a non-zero status

# 1. Define installation path for the Wasm dependencies
INSTALL_DIR="$(pwd)/wasm_install"
mkdir -p "$INSTALL_DIR"

# Helper to update config scripts for Wasm support
update_config_scripts() {
    echo "Updating config.sub and config.guess in $(pwd)/config..."
    curl -o config/config.guess 'https://git.savannah.gnu.org/gitweb/?p=config.git;a=blob_plain;f=config.guess;hb=HEAD'
    curl -o config/config.sub 'https://git.savannah.gnu.org/gitweb/?p=config.git;a=blob_plain;f=config.sub;hb=HEAD'
    chmod +x config/config.guess config/config.sub
}

echo "=== Building hts_engine_API ==="
cd hts_engine_API

# FIX: Create dummy ChangeLog to satisfy automake strictness
touch ChangeLog

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
# Ensure Sinsy also has necessary files (just in case)
touch ChangeLog

# Generate configure script
chmod +x configure
autoreconf -i

# FIX: Update config scripts to recognize 'emscripten'
update_config_scripts

# Configure Sinsy
# LDFLAGS includes the fix for proper quoting of EXPORTED_FUNCTIONS
emconfigure ./configure \
  --with-hts-engine-header-path="$INSTALL_DIR/include" \
  --with-hts-engine-library-path="$INSTALL_DIR/lib" \
  --host=wasm32-unknown-emscripten \
  LDFLAGS="-s \"EXPORTED_FUNCTIONS=['_main']\" -s ALLOW_MEMORY_GROWTH=1"

# Compile Sinsy
emmake make -j$(nproc)

# FIX: Rename the output binary if it lacks the .js extension
if [ -f bin/sinsy ] && [ ! -f bin/sinsy.js ]; then
    echo "Renaming bin/sinsy to bin/sinsy.js..."
    mv bin/sinsy bin/sinsy.js
fi

echo "✅ Build Complete."
