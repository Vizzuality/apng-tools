#!/bin/bash

set -e

export OPTIMIZE="-O3 -pedantic"
export LDFLAGS="${OPTIMIZE}"
export CFLAGS="${OPTIMIZE}"
export CPPFLAGS="${OPTIMIZE}"

cd apngopt; make; cd ..

(
  g++ \
  -o apng-optimizer \
  ${OPTIMIZE} \
  -flto \
  -I ./apngopt -I ./apngopt/7z -I ./apngopt/imagequant -I ./apngopt/libpng -I ./apngopt/zlib -I ./apngopt/zopfli \
  ./apngopt/obj/*.o \
  ./apngopt/obj/**/*.o \
  ./apngopt/imagequant/target/release/libimagequant_sys.a \
  ./binding.cpp \
)

