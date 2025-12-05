#!/bin/bash
source /content/build_space/emsdk/emsdk_env.sh
chmod +x configure
autoreconf -i
emconfigure ./configure
emmake make -j55
