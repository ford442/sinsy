#!/bin/bash
source /workspaces/sinsy/emsdk/emsdk_env.sh

set -e # Exit immediately if any command fails

# 1. Define installation path for the Wasm dependencies
INSTALL_DIR="$(pwd)/wasm_install"
mkdir -p "$INSTALL_DIR"

# Helper to update config scripts using GitHub mirrors
update_config_scripts() {
    local CONFIG_DIR="config"
    mkdir -p "$CONFIG_DIR"
    
    echo "Updating config.sub and config.guess in $(pwd)/$CONFIG_DIR..."
    curl -f -L --max-time 30 -o "$CONFIG_DIR/config.guess" 'https://raw.githubusercontent.com/gcc-mirror/gcc/master/config.guess'
    curl -f -L --max-time 30 -o "$CONFIG_DIR/config.sub"   'https://raw.githubusercontent.com/gcc-mirror/gcc/master/config.sub'
    
    chmod +x "$CONFIG_DIR/config.guess" "$CONFIG_DIR/config.sub"
    sed -i 's/\r$//' "$CONFIG_DIR/config.guess" "$CONFIG_DIR/config.sub"
}

echo "=== Building hts_engine_API ==="
cd hts_engine_API
touch ChangeLog
autoreconf -ivf
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
# FIX: Added 'FS' and 'callMain' to EXPORTED_RUNTIME_METHODS
# FIX: Added INVOKE_RUN=0 so it waits for your button click instead of running immediately
emconfigure ./configure \
  --host=wasm32-unknown-emscripten \
  CPPFLAGS="-I$INSTALL_DIR/include"

# Compile Sinsy
emmake make -j$(nproc) \
  LDFLAGS="-L$INSTALL_DIR/lib -s EXPORTED_FUNCTIONS=[_main] -s EXPORTED_RUNTIME_METHODS=[FS,callMain] -s DISABLE_EXCEPTION_CATCHING=0 -s ALLOW_MEMORY_GROWTH=1 -s INVOKE_RUN=0 --preload-file dic@/dic --preload-file voices@/voices --preload-file scores@/scores"

# Rename output to .js if needed
if [ -f bin/sinsy ] && [ ! -f bin/sinsy.js ]; then
    echo "Renaming bin/sinsy to bin/sinsy.js..."
    mv bin/sinsy bin/sinsy.js
fi

echo "✅ Build Complete."
