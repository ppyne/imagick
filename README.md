# imagick

<img src="ImageMagick_logo.svg" width="290" height="300" alt="Image Magick Logo" />

Running ImageMagick wand library in WASM.

Try it here [ppyne.github.io/imagick](https://ppyne.github.io/imagick/).

## Requirements

- ImageMagick Legacy version 6.9.12-62 (tar.gz present in the repo)
- Emscripten SDK version 3.1.20
- A Unix like environment (Mac or Linux is fine) with bash
- Essential GNU building tools like make or pkg-config
- A web server like Apache2

## How to build

Create a directory in the path served by your web server.

Of course, don't forget to run the Emscripten SDK evironment script before doing anything (`emsdk/emsdk_env.sh`).

Review the content of the script `make.sh` and adapt it to your project or your environment.

Make the script `make.sh` executable (`chmod +x ./make.sh`).

Feel free to add new functions to `imagick.c` and `index.html`, and ask for a pull request to make your work available to as many people as possible (MagickWand Image API for C documentation is available [here](https://legacy.imagemagick.org/script/magick-wand.php)).

And enjoy.

## Notes

Except for WebP, we didn't need to convert images from one format to another with ImageMagick yet, so we didn't link against libpng or libjpeg. But it is possible, we made a try with libwebp and it works like a charm with all sorts of options. Also it is possible to convert formats like PNG or JPEG with JS directly (See below for an explanation).

We use the Emscripten filesystem (FS) to exchange data between JS and ImageMagick.

We also found convenient to use the native raw ".rgba" ImageMagick file format, which has the same byte order as the JS ImageData.

The `imagick.html` file present is not very usefull, the most important files are `imagick.wasm`, `imagick.js` and `index.html`.

Since the JS Canvas permits shapes and text drawings natively, we don't see the need to use ImageMagick for this purpose, and didn't see the need to compile IM with the freetype lib.

And yes for many years now we use pure browser JS and jquery, and still do, we are not much exited by Nodejs, or TypeScript... sorry.

Here are the options overview set at compliation time for ImageMagick:

```
                  Option                        Value
------------------------------------------------------------------------------
Shared libraries  --enable-shared=no  no
Static libraries  --enable-static=yes  yes
Module support    --with-modules=no   no
GNU ld            --with-gnu-ld=yes      yes
Quantum depth     --with-quantum-depth=16       16
High Dynamic Range Imagery
                  --enable-hdri=yes             yes

Install documentation:                          no

Memory allocation library:
  JEMalloc          --with-jemalloc=no          no
  TCMalloc          --with-tcmalloc=no          no
  UMem              --with-umem=no              no

Delegate library configuration:
  BZLIB             --with-bzlib=no             no
  Autotrace         --with-autotrace=no         no
  DJVU              --with-djvu=no              no
  DPS               --with-dps=no               no
  FFTW              --with-fftw=no              no
  FLIF              --with-flif=no              no
  FlashPIX          --with-fpx=no               no
  FontConfig        --with-fontconfig=no        no
  FreeType          --with-freetype=no          no
  Ghostscript lib   --with-gslib=no             no
  Graphviz          --with-gvc=no
  HEIC              --with-heic=no              no
  JBIG              --with-jbig=no              no
  JPEG v1           --with-jpeg=no              no
  JPEG XL           --with-jxl=no               no
  LCMS              --with-lcms=no              no
  LQR               --with-lqr=no               no
  LTDL              --with-ltdl=no              no
  LZMA              --with-lzma=no              no
  Magick++          --with-magick-plus-plus=no  no
  OpenEXR           --with-openexr=no           no
  OpenJP2           --with-openjp2=no           no
  PANGO             --with-pango=no             no
  PERL              --with-perl=no              no
  PNG               --with-png=no               no
  RAQM              --with-raqm=no              no
  RAW               --with-raw=no               no
  RSVG              --with-rsvg=no              no
  TIFF              --with-tiff=no              no
  WEBP              --with-webp=yes             yes
  WMF               --with-wmf=no               no
  X11               --with-x=no                 no
  XML               --with-xml=no               no
  ZLIB              --with-zlib=no              no
  ZSTD              --with-zstd=no              no
```

### Native browser image format conversion

Here is an example of convertion to PNG thanks to the canvas.

```javascript
let img = document.querySelector('#my_image');
let canvas = document.createElement("canvas");
canvas.width = img.width;
canvas.height = img.height;
let ctx = canvas.getContext("2d");
ctx.drawImage(img, 0, 0);
img.src = canvas.toDataURL('image/png');
```

Thanks to this process, you can convert any PNG, JPEG or even WEBP format to another.

The last line can be replaced with this one for JPEG:

```javascript
img.src = canvas.toDataURL('image/jpeg', 0.8);
```

where `0.8` is the image quality (0 to 1).

Or with this one for WEBP (with recent browsers only):

```javascript
img.src = canvas.toDataURL('image/webp', 0.7);
```

where `0.7` is the image quality (0 to 1).

See the documentation [here](https://developer.mozilla.org/en-US/docs/Web/API/HTMLCanvasElement/toDataURL) to learn more.