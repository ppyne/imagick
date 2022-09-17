# imagick

Running ImageMagick wand library in WASM.

Try it here [ppyne.github.io/imagick](https://ppyne.github.io/imagick/).

## Requirements

- ImageMagick Legacy version 6.9.12-62 (tar.gz present in the repo)
- Emscripten version 3.1.20
- Unix like environment (Mac or Linux is fine) with bash
- essential GNU building tools like make or pkg-config
- a web server like Apache2

## How to build

Create a directory in the path served by your web server.
Review the content of the script `make.sh` and adapt it to your project or your environment.
Make the script `make.sh` executable (`chmod +x ./make.sh`).
And enjoy.

## Notes

We didn't need to convert images from one format to another, so we didn't link agains libpng, libjpeg or libwebp. But it is possible, we made a try with libwebp and it worked like a charm.

We use the Emscripten filesystem (FS) to exchange data between JS and WASM.

We also found convenient to use the native raw ".rgba" ImageMagick file format, which has the same byte order as the JS ImageData.
