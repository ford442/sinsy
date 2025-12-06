#!/bin/bash
source /content/build_space/emsdk/emsdk_env.sh # Uncomment if running in an environment where this is needed
set -e # Exit immediately if any command fails

# 1. Define installation path for the Wasm dependencies
INSTALL_DIR="$(pwd)/wasm_install"
mkdir -p "$INSTALL_DIR"

# Helper to update config scripts using GitHub mirrors (Faster/More Reliable)
update_config_scripts() {
    local CONFIG_DIR="config"
    mkdir -p "$CONFIG_DIR"
    
    echo "Updating config.sub and config.guess in $(pwd)/$CONFIG_DIR..."
    
    # Use GitHub Raw URLs with a 30-second max time to prevent hanging
    # -L follows redirects, -f fails on error, --max-time 30 prevents hangs
    curl -f -L --max-time 30 -o "$CONFIG_DIR/config.guess" 'https://raw.githubusercontent.com/gcc-mirror/gcc/master/config.guess'
    curl -f -L --max-time 30 -o "$CONFIG_DIR/config.sub"   'https://raw.githubusercontent.com/gcc-mirror/gcc/master/config.sub'
    
    # Verify the files are not empty
    if [ ! -s "$CONFIG_DIR/config.sub" ]; then
        echo "Error: Failed to download config.sub (file is empty)."
        exit 1
    fi

    chmod +x "$CONFIG_DIR/config.guess" "$CONFIG_DIR/config.sub"
    sed -i 's/\r$//' "$CONFIG_DIR/config.guess" "$CONFIG_DIR/config.sub"
}

echo "=== Building hts_engine_API ==="
cd hts_engine_API
touch ChangeLog

# Generate configure script
autoreconf -ivf

# Update config scripts (Force overwrite with new versions)
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

# Update config scripts for Sinsy
update_config_scripts

# Configure Sinsy with Preloaded Files
# Note: "dic@/dic" maps local folder 'dic' to '/dic' in the browser
emconfigure ./configure \
  --with-hts-engine-header-path="$INSTALL_DIR/include" \
  --with-hts-engine-library-path="$INSTALL_DIR/lib" \
  --host=wasm32-unknown-emscripten \
  LDFLAGS="-s \"EXPORTED_FUNCTIONS=['_main']\" -s ALLOW_MEMORY_GROWTH=1 --preload-file dic@/dic --preload-file voices@/voices --preload-file scores@/scores"

# Compile Sinsy
emmake make -j$(nproc)


echo "✅ Build Complete."
