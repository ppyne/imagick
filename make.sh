#!/bin/bash

set -e

BUILD_WEBP=1
BUILD_IMAGE_MAGICK=1
BUILD_IMAGICK=0

IMAGE_MAGICK=ImageMagick6-6.9.12-66

export CFLAGS="-O3"
export DESTINATION="`pwd`/image_magick"
export CPPFLAGS="-I${DESTINATION}/include"
export LDFLAGS="-L${DESTINATION}/lib -sASSERTIONS=1 -sALLOW_MEMORY_GROWTH=1"
export PKG_CONFIG_PATH="${DESTINATION}/lib/pkgconfig/"

if [ $BUILD_WEBP -eq 1 ]; then
	if ! [ -d "libwebp-1.2.4" ]; then
		tar xvfz libwebp-1.2.4.tar.gz
	fi
	cd libwebp-1.2.4
	emconfigure ./configure --prefix="$DESTINATION" --exec-prefix="$DESTINATION" --disable-shared --enable-static --disable-threading
	emmake make
	emmake make install
	cd ..
    # On MacOS, desperate times require desperate measures...
    #cp libwebp.la "${DESTINATION}/lib/"
    #cp libwebpmux.la "${DESTINATION}/lib/"
    #cp libwebpdemux.la "${DESTINATION}/lib/"
    # End of desperate times
fi

if [ $BUILD_IMAGE_MAGICK -eq 1 ]; then

    #rm -rf "${IMAGE_MAGICK}"
    if ! [ -d "${IMAGE_MAGICK}" ]; then
        tar xvfz "${IMAGE_MAGICK}.tar.gz"
    fi
    cd "${IMAGE_MAGICK}"
    # On MacOS and Linux, Desperate times require desperate measures...
	gsed -i "2 i export PKG_CONFIG_PATH" configure
	gsed -i "2 i PKG_CONFIG_PATH=\"$DESTINATION/lib/pkgconfig/\"" configure
    # End of desperate times
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
        --with-webp \
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
    # On MacOS, desperate times require desperate measures...
    #cp MagickWand.pc "${DESTINATION}/lib/pkgconfig/"
    # End of desperate times
fi

if [ $BUILD_IMAGICK -eq 1 ]; then
    PKG_CONFIG_PATH="${PKG_CONFIG_PATH}" CFLAGS="-O3" make
fi