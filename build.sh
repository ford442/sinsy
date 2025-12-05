#!/bin/bash
chmod +x configure
autoreconf -i
emconfigure ./configure
emmake make -j55
