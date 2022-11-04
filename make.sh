#!/bin/bash

set -e

BUILD_IMAGE_MAGICK=1
BUILD_IMAGICK=0

IMAGE_MAGICK=ImageMagick6-6.9.12-66

export CFLAGS="-O3"
export DESTINATION="`pwd`/image_magick"
export CPPFLAGS="-I${DESTINATION}/include"
export LDFLAGS="-L${DESTINATION}/lib -sASSERTIONS=1 -sALLOW_MEMORY_GROWTH=1"
export PKG_CONFIG_PATH="${DESTINATION}/lib/pkgconfig/"

if [ $BUILD_IMAGE_MAGICK -eq 1 ]; then

    rm -rf "${IMAGE_MAGICK}"
    rm -rf "${DESTINATION}"
    tar xvfz "${IMAGE_MAGICK}.tar.gz"
    cd "${IMAGE_MAGICK}"

    emconfigure ./configure \
        --without-threads \
        --without-x \
        --without-magick-plus-plus \
        --without-perl \
        --enable-static \
        --disable-shared \
        --with-quantum-depth=16 \
        --enable-hdri \
        --disable-docs \
        --disable-largefile \
        --without-png \
        --without-tiff \
        --without-webp \
        --disable-openmp \
        --without-bzlib \
        --without-dps \
        --without-freetype \
        --without-jpeg \
        --without-lzma \
        --without-zlib \
        --without-jxl \
        --without-jbig \
        --without-openjp2 \
        --without-lcms \
        --without-wmf \
        --without-xml \
        --without-fftw \
        --without-flif \
        --without-fpx \
        --without-djvu \
        --without-fontconfig \
        --without-raqm \
        --without-gslib \
        --without-gvc \
        --without-heic \
        --without-lqr \
        --without-openexr \
        --without-pango \
        --without-raw \
        --without-rsvg \
        --without-xml \
        --without-zstd \
        --prefix="${DESTINATION}" \
        --exec-prefix="${DESTINATION}"

    emmake make
    emmake make install
    cd ..
fi

if [ $BUILD_IMAGICK -eq 1 ]; then
    PKG_CONFIG_PATH="${PKG_CONFIG_PATH}" CFLAGS="-O3" make
fi