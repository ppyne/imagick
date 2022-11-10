#!/bin/bash

set -e

BUILD_WEBP=1
BUILD_ZLIB=1
BUILD_LIBPNG=1
BUILD_JPEG=1
BUILD_OPENJPEG=1
BUILD_FFTW=1
BUILD_AOM=1
BUILD_DAV1D=1
BUILD_X265=1
BUILD_DE265=1
BUILD_HEIC=1
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

if [ $BUILD_ZLIB -eq 1 ]; then
	if ! [ -d "zlib-1.2.12" ]; then
		tar xvfz zlib-1.2.12.tar.gz
	fi
	cd zlib-1.2.12
	emconfigure ./configure --static --prefix="$DESTINATION"
	emmake make
	emmake make install
	cd ..
fi

if [ $BUILD_LIBPNG -eq 1 ]; then
	#rm -rf libpng-1.6.37
	if ! [ -d "libpng-1.6.37" ]; then
		tar xvfz libpng-1.6.37.tar.gz
	fi
	cd libpng-1.6.37
	emconfigure ./configure --prefix="$DESTINATION" --exec-prefix="$DESTINATION" --disable-shared --enable-static
	emmake make
	emmake make install
	cd ..
fi

if [ $BUILD_JPEG -eq 1 ]; then
	if ! [ -d "jpeg-8c" ]; then
		tar xvfz jpegsrc.v8c.tar.gz
	fi
	cd jpeg-8c
	emconfigure ./configure --prefix="$DESTINATION" --exec-prefix="$DESTINATION" --disable-shared --enable-static
	emmake make
	emmake make install
	cd ..
fi

if [ $BUILD_OPENJPEG -eq 1 ]; then
	if ! [ -d "openjpeg-2.5.0" ]; then
		tar xvfz openjpeg-2.5.0.tar.gz
	fi
	cd openjpeg-2.5.0
	if ! [ -d "build" ]; then mkdir build; else rm -rf build; mkdir build; fi
	cd  build
	emcmake cmake -DCMAKE_INSTALL_PREFIX:PATH=$DESTINATION -DENABLE_SHARED=OFF -DENABLE_STATIC=ON ..
	emmake make
	emmake make install
	cd ../..
fi

if [ $BUILD_FFTW -eq 1 ]; then
	if ! [ -d "fftw-3.3.10" ]; then
		tar xvfz fftw-3.3.10.tar.gz
	fi
	cd fftw-3.3.10
	emconfigure ./configure --prefix="$DESTINATION" --exec-prefix="$DESTINATION" --disable-shared --enable-static
	emmake make
	emmake make install
	cd ..
fi

if [ $BUILD_AOM -eq 1 ]; then
	if ! [ -d "aom-1.0.0" ]; then
		tar xvfz aom-1.0.0.tar.gz
	fi
	cd aom-1.0.0/build
    if ! [ -d "emscripten" ]; then mkdir emscripten; else rm -rf emscripten; mkdir emscripten; fi
    cd emscripten
	emcmake cmake -DCMAKE_INSTALL_PREFIX:PATH=$DESTINATION -DENABLE_SHARED=OFF -DENABLE_STATIC=ON -DBUILD_SHARED_LIBS=OFF -DENABLE_MMX=OFF -DENABLE_SSE=OFF -DENABLE_SSE2=OFF -DENABLE_SSE3=OFF -DENABLE_SSSE3=OFF -DENABLE_SSE4_1=OFF -DENABLE_SSE4_2=OFF -DENABLE_AVX=OFF -DENABLE_AVX2=OFF -DAOM_TARGET_CPU=generic -DENABLE_TESTS=OFF ../..
	emmake make
	emmake make install
	cd ../../..
fi

if [ $BUILD_DAV1D -eq 1 ]; then
	if ! [ -d "dav1d-0.7.1" ]; then
		tar xvfz dav1d-0.7.1.tar.gz
	fi
	cd dav1d-0.7.1
    if ! [ -d "build" ]; then mkdir build; else rm -rf build; mkdir build; fi
    meson setup build -Denable_tools=false -Denable_tests=false --default-library static --cross-file ../wasm.ini
    cd build
    meson compile --verbose
    cd ..
    DESTDIR="${DESTINATION}" meson install -C build
    # Desperate times require desperate measures...
    cp "../dav1d.pc" "${DESTINATION}/lib/pkgconfig/dav1d.pc"
    cd ..
fi

if [ $BUILD_X265 -eq 1 ]; then
	if ! [ -d "x265_3.4" ]; then
		tar xvfz x265_3.4.tar.gz
	fi
	cd x265_3.4/build
    if ! [ -d "emscripten" ]; then mkdir emscripten; else rm -rf emscripten; mkdir emscripten; fi
    cd emscripten
	emcmake cmake -DCMAKE_INSTALL_PREFIX:PATH=$DESTINATION -DENABLE_SHARED=OFF -DENABLE_STATIC=ON -DENABLE_CLI=OFF ../../source
	emmake make
	emmake make install
	cd ../../..
fi

if [ $BUILD_DE265 -eq 1 ]; then
	if ! [ -d "libde265-1.0.9" ]; then
		tar xvfz libde265-1.0.9.tar.gz
	fi
	cd libde265-1.0.9
    emconfigure ./configure --build=x86_64-pc-linux-gnu --disable-sse --disable-dec265 --disable-sherlock265 --prefix="$DESTINATION" --exec-prefix="$DESTINATION" --disable-shared --enable-static
	emmake make
	emmake make install
	cd ..
fi

if [ $BUILD_HEIC -eq 1 ]; then
	if ! [ -d "libheif-1.13.0" ]; then
		tar xvfz libheif-1.13.0.tar.gz
	fi
	cd libheif-1.13.0
	emconfigure ./configure --build=x86_64-pc-linux-gnu --prefix="$DESTINATION" --exec-prefix="$DESTINATION" --disable-shared --enable-static --disable-multithreading --disable-go --disable-examples --disable-tests PKG_CONFIG_PATH="${DESTINATION}/lib/pkgconfig/" LDFLAGS="--bind"
	emmake make
	emmake make install
	cd ..
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
        --with-quantum-depth=16 \
        --enable-hdri \
        --with-zlib \
        --with-png \
        --with-jpeg \
        --with-openjp2 \
        --with-webp \
        --with-heic \
        --with-fftw \
        --enable-static \
        --disable-shared \
        --without-threads \
        --without-x \
        --without-magick-plus-plus \
        --without-perl \
        --disable-docs \
        --disable-largefile \
        --without-tiff \
        --disable-openmp \
        --without-bzlib \
        --without-dps \
        --without-freetype \
        --without-lzma \
        --without-jxl \
        --without-jbig \
        --without-lcms \
        --without-wmf \
        --without-xml \
        --without-flif \
        --without-fpx \
        --without-djvu \
        --without-fontconfig \
        --without-raqm \
        --without-gslib \
        --without-gvc \
        --without-lqr \
        --without-openexr \
        --without-pango \
        --without-raw \
        --without-rsvg \
        --without-xml \
        --without-zstd \
        --prefix="${DESTINATION}" \
        --exec-prefix="${DESTINATION}" \
        LDFLAGS="-L${DESTINATION}/lib -sASSERTIONS=1 -sALLOW_MEMORY_GROWTH=1 --bind"

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