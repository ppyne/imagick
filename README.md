# imagick

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

Feel free to add new functions to `imagick.c` and `index.html`, and ask for a pull request to make your work available to as many people as possible.

And enjoy.

## Notes

We didn't need to convert images from one format to another, so we didn't link agains libpng, libjpeg or libwebp. But it is possible, we made a try with libwebp and it worked like a charm.

We use the Emscripten filesystem (FS) to exchange data between JS and WASM.

We also found convenient to use the native raw ".rgba" ImageMagick file format, which has the same byte order as the JS ImageData.

The `imagick.html` file present is not very usefull, the most important files are `imagick.wasm`, `imagick.js` and `index.html`.

And yes for many years now we use pure browser JS and jquery, and still do, we are not much exited by Nodejs, or TypeScript... sorry.