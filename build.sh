#!/bin/bash
source /content/build_space/emsdk/emsdk_env.sh # Uncomment if running in an environment where this is needed
set -e # Exit immediately if any command fails

# 1. Define installation path for the Wasm dependencies
INSTALL_DIR="$(pwd)/wasm_install"
mkdir -p "$INSTALL_DIR"

# Helper to update config scripts securely
update_config_scripts() {
    local CONFIG_DIR="config"
    # Ensure config directory exists
    mkdir -p "$CONFIG_DIR"
    
    echo "Updating config.sub and config.guess in $(pwd)/$CONFIG_DIR..."
    
    # Use curl with -L to follow redirects and -f to fail on HTTP errors
    curl -f -L -o "$CONFIG_DIR/config.guess" 'https://git.savannah.gnu.org/gitweb/?p=config.git;a=blob_plain;f=config.guess;hb=HEAD'
    curl -f -L -o "$CONFIG_DIR/config.sub"   'https://git.savannah.gnu.org/gitweb/?p=config.git;a=blob_plain;f=config.sub;hb=HEAD'
    
    # Verify the files are not empty
    if [ ! -s "$CONFIG_DIR/config.sub" ]; then
        echo "Error: Failed to download config.sub (file is empty)."
        exit 1
    fi

    # Ensure they are executable
    chmod +x "$CONFIG_DIR/config.guess" "$CONFIG_DIR/config.sub"
    
    # Fix potential Windows/DOS line endings if downloaded weirdly
    sed -i 's/\r$//' "$CONFIG_DIR/config.guess" "$CONFIG_DIR/config.sub"
}

echo "=== Building hts_engine_API ==="
cd hts_engine_API

# Fix: Create dummy ChangeLog to satisfy automake strictness
touch ChangeLog

# Generate configure script
autoreconf -ivf

# Update config scripts AFTER autoreconf (so they aren't overwritten by old ones)
update_config_scripts

# Configure and build HTS Engine
emconfigure ./configure --prefix="$INSTALL_DIR" --host=wasm32-unknown-emscripten
emmake make -j$(nproc)
emmake make install

cd ..
echo "=== Building Sinsy ==="
touch ChangeLog
chmod +x configure
autoreconf -ivf
update_config_scripts

# Configure Sinsy
# We added --preload-file flags here using local paths (dic, voices, scores)
# mapping them to virtual paths (/dic, /voices, /scores).
emconfigure ./configure \
  --with-hts-engine-header-path="$INSTALL_DIR/include" \
  --with-hts-engine-library-path="$INSTALL_DIR/lib" \
  --host=wasm32-unknown-emscripten \
  LDFLAGS="-s \"EXPORTED_FUNCTIONS=['_main']\" -s ALLOW_MEMORY_GROWTH=1 --preload-file dic@/dic --preload-file voices@/voices --preload-file scores@/scores"

# Compile Sinsy
emmake make -j$(nproc)

# Rename the output binary if needed
if [ -f bin/sinsy ] && [ ! -f bin/sinsy.js ]; then
    echo "Renaming bin/sinsy to bin/sinsy.js..."
    mv bin/sinsy bin/sinsy.js
fi

echo "✅ Build Complete."
