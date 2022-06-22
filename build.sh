#!/bin/bash

set -e

export OPTIMIZE="-O3 -pedantic"
export LDFLAGS="${OPTIMIZE}"
export CFLAGS="${OPTIMIZE}"
export CPPFLAGS="${OPTIMIZE}"

(
  g++ \
  ${OPTIMIZE} \
  -flto \
  -stdlib=libc++ \
  -I ./apngopt/7z \
  -I ./apngopt/libpng \
  -I ./apngopt/zlib \
  -I ./apngopt/zopfli \
  -I ./apngopt/imagequant \
  -I ./apngopt \
  ./binding.cpp \
)

