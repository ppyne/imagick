#include <emscripten.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <wand/MagickWand.h>

const char SRC_FILE[] = "src.rgba";
const char DST_FILE[] = "dst.rgba";

EM_JS(void, is_ready, (), {
    if (window !== undefined) {
        const event = new Event('IMReady');
        window.dispatchEvent(event);
    }
});

EM_JS(void, console_log, (const char *str), {
    console.log(UTF8ToString(str));
});

EM_JS(void, console_error, (const char *str1, const char *str2), {
    console.error(UTF8ToString(str1) + UTF8ToString(str2));
});

EM_JS(void, console_error_wand, (const char *str1, const char *str2, unsigned long n, const char *str3), {
    console.error(UTF8ToString(str1) + ' ' + UTF8ToString(str2) + ' line ' + n + ' ' + UTF8ToString(str3));
});

EM_JS(void, console_log_quadruplet, (size_t i, const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a), {
    let _r = r.toString(16);
    if (_r.length < 2) _r = '0' + _r;
    let _g = g.toString(16);
    if (_g.length < 2) _g = '0' + _g;
    let _b = b.toString(16);
    if (_b.length < 2) _b = '0' + _b;
    let _a = a.toString(16);
    if (_a.length < 2) _a = '0' + _a;
    console.log(i, _r, _g, _b, _a);
});

EM_JS(void, console_log_color, (size_t i, const unsigned char r, const unsigned char g, const unsigned char b), {
    let _r = r.toString(16);
    if (_r.length < 2) _r = '0' + _r;
    let _g = g.toString(16);
    if (_g.length < 2) _g = '0' + _g;
    let _b = b.toString(16);
    if (_b.length < 2) _b = '0' + _b;
    console.log(i, '#' + _r + _g + _b);
});

EM_JS(void, console_log_len, (size_t len), {
    console.log('len', len);
});

#define strwanderror(wand) { \
        char *description; \
        ExceptionType severity; \
        description = MagickGetException(wand, &severity); \
        console_error_wand(GetMagickModule(), description); \
        description = (char *)MagickRelinquishMemory(description); \
}

void _IMResize(const unsigned int srcWidth, const unsigned int srcHeight, const unsigned int dstWidth, const unsigned int dstHeight, const int filter, const double blur) {
    MagickBooleanType status;
    MagickWand *magick_wand;
    MagickWandGenesis();
    magick_wand = NewMagickWand();
    MagickSetSize(magick_wand, srcWidth, srcHeight);
    status = MagickReadImage(magick_wand, SRC_FILE);
    if (status == MagickFalse) {
        strwanderror(magick_wand);
    } else {
        MagickResetIterator(magick_wand);
        while (MagickNextImage(magick_wand) != MagickFalse) MagickResizeImage(magick_wand, dstWidth, dstHeight, filter, blur);
        status = MagickWriteImages(magick_wand, DST_FILE, MagickTrue);
        if (status == MagickFalse) {
            strwanderror(magick_wand);
        } else {
            magick_wand = DestroyMagickWand(magick_wand);
            MagickWandTerminus();
        }
    }
}

void _IMCharcoal(const unsigned int width, const unsigned int height, const double radius, const double sigma) {
    MagickBooleanType status;
    MagickWand *magick_wand;
    MagickWandGenesis();
    magick_wand = NewMagickWand();
    MagickSetSize(magick_wand, width, height);
    status = MagickReadImage(magick_wand, SRC_FILE);
    if (status == MagickFalse) {
        strwanderror(magick_wand);
    } else {
        MagickResetIterator(magick_wand);
        while (MagickNextImage(magick_wand) != MagickFalse) MagickCharcoalImage(magick_wand, radius, sigma);
        status = MagickWriteImages(magick_wand, DST_FILE, MagickTrue);
        if (status == MagickFalse) {
            strwanderror(magick_wand);
        } else {
            magick_wand = DestroyMagickWand(magick_wand);
            MagickWandTerminus();
        }
    }
}

void _IMSketch(const unsigned int width, const unsigned int height, const double radius, const double sigma, const double angle) {
    MagickBooleanType status;
    MagickWand *magick_wand;
    MagickWandGenesis();
    magick_wand = NewMagickWand();
    MagickSetSize(magick_wand, width, height);
    status = MagickReadImage(magick_wand, SRC_FILE);
    if (status == MagickFalse) {
        strwanderror(magick_wand);
    } else {
        MagickResetIterator(magick_wand);
        while (MagickNextImage(magick_wand) != MagickFalse) MagickSketchImage(magick_wand, radius, sigma, angle);
        status = MagickWriteImages(magick_wand, DST_FILE, MagickTrue);
        if (status == MagickFalse) {
            strwanderror(magick_wand);
        } else {
            magick_wand = DestroyMagickWand(magick_wand);
            MagickWandTerminus();
        }
    }
}

void _IMQuantize(const unsigned int width, const unsigned int height, const size_t number_colors, const int colorspace, const size_t treedepth, const bool dither,
  const bool measure_error) {
    MagickBooleanType status;
    MagickWand *magick_wand;
    MagickWandGenesis();
    magick_wand = NewMagickWand();
    MagickSetSize(magick_wand, width, height);
    status = MagickReadImage(magick_wand, SRC_FILE);
    if (status == MagickFalse) {
        strwanderror(magick_wand);
    } else {
        MagickResetIterator(magick_wand);
        while (MagickNextImage(magick_wand) != MagickFalse) MagickQuantizeImages(magick_wand, number_colors, colorspace, treedepth, dither, measure_error);
        status = MagickWriteImages(magick_wand, DST_FILE, MagickTrue);
        if (status == MagickFalse) {
            strwanderror(magick_wand);
        } else {
            magick_wand = DestroyMagickWand(magick_wand);
            MagickWandTerminus();
        }
    }
}

void _IMListUniqueColors(const unsigned int width, const unsigned int height) {
    MagickBooleanType status;
    MagickWand *magick_wand;
    PixelIterator *iter;
    PixelWand **row;
    MagickPixelPacket pixel;
    size_t rows, cols, y, x;
    FILE *output;
    bool first = true;
    char buffer[11];

    if ((output = fopen("dst.json", "w")) != NULL) {
        fwrite((void *)"[", 1, 1, output);
        MagickWandGenesis();
        magick_wand = NewMagickWand();
        MagickSetSize(magick_wand, width, height);
        status = MagickReadImage(magick_wand, SRC_FILE);
        if (status == MagickFalse) {
            fwrite((void *)"]", 1, 1, output);
            fclose(output);
            strwanderror(magick_wand);
        } else {
            MagickResetIterator(magick_wand);
            while (MagickNextImage(magick_wand) != MagickFalse) MagickUniqueImageColors(magick_wand);
            while (MagickNextImage(magick_wand) != MagickFalse) {
                MagickGetSize(magick_wand, &cols, &rows);
                iter = NewPixelIterator(magick_wand);
                if (IsPixelIterator(iter) != MagickFalse) {
                    for (y = 0; y < rows; y++) {
                        row = PixelGetNextIteratorRow(iter, &cols);
                        for (x = 0; x < cols; ++x) {
                            PixelGetMagickColor(row[x], &pixel);
                            sprintf(buffer, "\"#%02X%02X%02X\"", (unsigned char)pixel.red, (unsigned char)pixel.green, (unsigned char)pixel.blue);
                            if (first == true) first = false;
                            else fwrite((void *)",", 1, 1, output);
                            fwrite((void *)buffer, strlen(buffer), 1, output);
                        }
                        PixelSyncIterator(iter);
                    }
                    fwrite((void *)"]", 1, 1, output);
                    fclose(output);
                    iter = DestroyPixelIterator(iter);
                }
            }
            magick_wand = DestroyMagickWand(magick_wand);
            MagickWandTerminus();
        }
    } else console_error("Error while opening \"dst.json\" for writing: ", strerror(errno));
}

/* channel types in magick/magick-type.h */
void _IMAutoLevel(const unsigned int width, const unsigned int height, const int channels) {
    MagickBooleanType status;
    MagickWand *magick_wand;
    MagickWandGenesis();
    magick_wand = NewMagickWand();
    MagickSetSize(magick_wand, width, height);
    status = MagickReadImage(magick_wand, SRC_FILE);
    if (status == MagickFalse) {
        strwanderror(magick_wand);
    } else {
        MagickResetIterator(magick_wand);
        while (MagickNextImage(magick_wand) != MagickFalse) MagickAutoLevelImageChannel(magick_wand, channels);
        status = MagickWriteImages(magick_wand, DST_FILE, MagickTrue);
        if (status == MagickFalse) {
            strwanderror(magick_wand);
        } else {
            magick_wand = DestroyMagickWand(magick_wand);
            MagickWandTerminus();
        }
    }
}

void _IMAutoGamma(const unsigned int width, const unsigned int height, const int channels) {
    MagickBooleanType status;
    MagickWand *magick_wand;
    MagickWandGenesis();
    magick_wand = NewMagickWand();
    MagickSetSize(magick_wand, width, height);
    status = MagickReadImage(magick_wand, SRC_FILE);
    if (status == MagickFalse) {
        strwanderror(magick_wand);
    } else {
        MagickResetIterator(magick_wand);
        while (MagickNextImage(magick_wand) != MagickFalse) MagickAutoGammaImageChannel(magick_wand, channels);
        status = MagickWriteImages(magick_wand, DST_FILE, MagickTrue);
        if (status == MagickFalse) {
            strwanderror(magick_wand);
        } else {
            magick_wand = DestroyMagickWand(magick_wand);
            MagickWandTerminus();
        }
    }
}

void _IMContrast(const unsigned int width, const unsigned int height, bool increase) {
    MagickBooleanType status;
    MagickWand *magick_wand;
    MagickWandGenesis();
    magick_wand = NewMagickWand();
    MagickSetSize(magick_wand, width, height);
    status = MagickReadImage(magick_wand, SRC_FILE);
    if (status == MagickFalse) {
        strwanderror(magick_wand);
    } else {
        MagickResetIterator(magick_wand);
        while (MagickNextImage(magick_wand) != MagickFalse) MagickContrastImage(magick_wand, increase);
        status = MagickWriteImages(magick_wand, DST_FILE, MagickTrue);
        if (status == MagickFalse) {
            strwanderror(magick_wand);
        } else {
            magick_wand = DestroyMagickWand(magick_wand);
            MagickWandTerminus();
        }
    }
}

int main() {
    is_ready();
    return 0;
}