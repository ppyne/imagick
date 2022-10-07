#include <emscripten.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
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

void _IMQuantize(const unsigned int width, const unsigned int height, const size_t number_colors, const int colorspace, const size_t treedepth, const bool dither, const bool measure_error) {
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

void _IMPosterize(const unsigned int width, const unsigned int height, const size_t levels, const bool dither) {
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
        while (MagickNextImage(magick_wand) != MagickFalse) MagickPosterizeImage(magick_wand, levels, dither);
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

void _IMQuantizeAndListUniqueColors(const unsigned int width, const unsigned int height, const size_t number_colors) {
    MagickBooleanType status;
    MagickWand *magick_wand;
    PixelIterator *iter;
    PixelWand **row;
    MagickPixelPacket pixel;
    size_t rows, cols, y, x;
    FILE *output;
    bool first = true;
    char buffer[32];

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
            while (MagickNextImage(magick_wand) != MagickFalse) MagickQuantizeImages(magick_wand, number_colors, sRGBColorspace, 0, 0, 0);

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
                            sprintf(buffer, "[%d,%d,%d]", (unsigned char)pixel.red, (unsigned char)pixel.green, (unsigned char)pixel.blue);
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

void storeThresholdsXml() {
    const char *filename = "thresholds.xml";
    if (access(filename, F_OK) == 0) return;
    const char data[] = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
"<!DOCTYPE thresholds [\n"
"  <!ELEMENT thresholds (threshold)+>\n"
"  <!ATTLIST thresholds xmlns CDATA #FIXED ''>\n"
"  <!ELEMENT threshold (description,levels)>\n"
"  <!ATTLIST threshold xmlns CDATA #FIXED '' alias NMTOKEN #IMPLIED\n"
"    map NMTOKEN #REQUIRED>\n"
"  <!ELEMENT description (#PCDATA)>\n"
"  <!ATTLIST description xmlns CDATA #FIXED ''>\n"
"  <!ELEMENT levels (#PCDATA)>\n"
"  <!ATTLIST levels xmlns CDATA #FIXED '' divisor CDATA #REQUIRED\n"
"    height CDATA #REQUIRED width CDATA #REQUIRED>\n"
"]>\n"
"<!--\n"
"  Threshold Maps for Ordered Posterized Dither\n"
"\n"
"  Each \"<threshold>\" element defines the map name, description, and an array\n"
"  of \"levels\" used to provide the threshold map for ordered dithering and\n"
"  digital halftoning.\n"
"\n"
"  The \"alias\" attribute provides a backward compatible name for this threshold\n"
"  map (pre-dating IM v6.2.9-6), and are deprecated.\n"
"\n"
"  The description is a english description of what the threshold map achieves\n"
"  and is only used for 'listing' the maps.\n"
"\n"
"  The map itself is a rectangular array of integers or threshold \"levels\"\n"
"  of the given \"width\" and \"height\" declared within the enclosing <levels>\n"
"  element.  That is \"width*height\" integers or \"levels\" *must* be provided\n"
"  within each map.\n"
"\n"
"  Each of the \"levels\" integer values (each value representing the threshold\n"
"  intensity \"level/divisor\" at which that pixel is turned on.  The \"levels\"\n"
"  integers given can be any postive integers between \"0\" and the \"divisor\",\n"
"  excluding those limits.\n"
"\n"
"  The \"divisor\" not only defines the upper limit and threshold divisor for each\n"
"  \"level\" but also the total number of pseudo-levels the threshold mapping\n"
"  creates and fills with a dither pattern.  That is a ordered bitmap dither\n"
"  of a pure greyscale gradient will use a maximum of \"divisor\" ordered bitmap\n"
"  patterns, including the patterns with all the pixels 'on' and all the pixel\n"
"  'off'. It may define less patterns than that, but the color channels will\n"
"  be thresholded in units based on \"divisor\".\n"
"\n"
"  Alternatively for a multi-level posterization, ImageMagick inserts\n"
"  \"divisor-2\" dither patterns (as defined by the threshold map) between each of\n"
"  channel color level produced.\n"
"\n"
"  For example the map \"o2x2\" has a divisor of 5, which will define 3 bitmap\n"
"  patterns plus the patterns with all pixels 'on' and 'off'.  A greyscale\n"
"  gradient will thus have 5 distinct areas.\n"
"-->\n"
"<thresholds>\n"
"\n"
"<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
"  Minimal Dither and Non-Dither Threshold Maps\n"
"-->\n"
"  <threshold map=\"threshold\" alias=\"1x1\">\n"
"    <description>Threshold 1x1 (non-dither)</description>\n"
"    <levels width=\"1\" height=\"1\" divisor=\"2\">\n"
"        1\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"  <threshold map=\"checks\" alias=\"2x1\">\n"
"    <description>Checkerboard 2x1 (dither)</description>\n"
"    <levels width=\"2\" height=\"2\" divisor=\"3\">\n"
"       1 2\n"
"       2 1\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
"  (dispersed) Ordered Dither Patterns\n"
"-->\n"
"  <threshold map=\"o2x2\" alias=\"2x2\">\n"
"    <description>Ordered 2x2 (dispersed)</description>\n"
"    <levels width=\"2\" height=\"2\" divisor=\"5\">\n"
"       1  3\n"
"       4  2\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"  <threshold map=\"o3x3\" alias=\"3x3\">\n"
"    <description>Ordered 3x3 (dispersed)</description>\n"
"    <levels width=\"3\" height=\"3\" divisor=\"10\">\n"
"        3  7  4\n"
"        6  1  9\n"
"        2  8  5\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"  <threshold map=\"o4x4\" alias=\"4x4\">\n"
"    <!--\n"
"       From \"Dithering Algorithms\"\n"
"       http://www.efg2.com/Lab/Library/ImageProcessing/DHALF.TXT\n"
"     -->\n"
"    <description>Ordered 4x4 (dispersed)</description>\n"
"    <levels width=\"4\" height=\"4\" divisor=\"17\">\n"
"       1   9   3  11\n"
"      13   5  15   7\n"
"       4  12   2  10\n"
"      16   8  14   6\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"  <threshold map=\"o8x8\" alias=\"8x8\">\n"
"    <!-- Extracted from original 'OrderedDither()' Function -->\n"
"    <description>Ordered 8x8 (dispersed)</description>\n"
"    <levels width=\"8\" height=\"8\" divisor=\"65\">\n"
"         1  49  13  61   4  52  16  64\n"
"        33  17  45  29  36  20  48  32\n"
"         9  57   5  53  12  60   8  56\n"
"        41  25  37  21  44  28  40  24\n"
"         3  51  15  63   2  50  14  62\n"
"        35  19  47  31  34  18  46  30\n"
"        11  59   7  55  10  58   6  54\n"
"        43  27  39  23  42  26  38  22\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
"  Halftones - Angled 45 degrees\n"
"\n"
"  Initially added to ImageMagick by Glenn Randers-Pehrson, IM v6.2.8-6,\n"
"  modified to be more symmetrical with intensity by Anthony, IM v6.2.9-7\n"
"\n"
"  These patterns initially start as circles, but then form diamonds\n"
"  pattern at the 50% threshold level, before forming negated circles,\n"
"  as it approached the other threshold extereme.\n"
"-->\n"
"  <threshold map=\"h4x4a\" alias=\"4x1\">\n"
"    <description>Halftone 4x4 (angled)</description>\n"
"    <levels width=\"4\" height=\"4\" divisor=\"9\">\n"
"       4  2  7  5\n"
"       3  1  8  6\n"
"       7  5  4  2\n"
"       8  6  3  1\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"  <threshold map=\"h6x6a\" alias=\"6x1\">\n"
"    <description>Halftone 6x6 (angled)</description>\n"
"    <levels width=\"6\" height=\"6\" divisor=\"19\">\n"
"       14  13  10   8   2   3\n"
"       16  18  12   7   1   4\n"
"       15  17  11   9   6   5\n"
"        8   2   3  14  13  10\n"
"        7   1   4  16  18  12\n"
"        9   6   5  15  17  11\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"  <threshold map=\"h8x8a\" alias=\"8x1\">\n"
"    <description>Halftone 8x8 (angled)</description>\n"
"    <levels width=\"8\" height=\"8\" divisor=\"33\">\n"
"       13   7   8  14  17  21  22  18\n"
"        6   1   3   9  28  31  29  23\n"
"        5   2   4  10  27  32  30  24\n"
"       16  12  11  15  20  26  25  19\n"
"       17  21  22  18  13   7   8  14\n"
"       28  31  29  23   6   1   3   9\n"
"       27  32  30  24   5   2   4  10\n"
"       20  26  25  19  16  12  11  15\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
"  Halftones - Orthogonally Aligned, or Un-angled\n"
"\n"
"  Initially added by Anthony Thyssen, IM v6.2.9-5 using techniques from\n"
"  \"Dithering & Halftoning\" by Gernot Haffmann\n"
"  http://www.fho-emden.de/~hoffmann/hilb010101.pdf\n"
"\n"
"  These patterns initially start as circles, but then form square\n"
"  pattern at the 50% threshold level, before forming negated circles,\n"
"  as it approached the other threshold extereme.\n"
"-->\n"
"  <threshold map=\"h4x4o\">\n"
"    <description>Halftone 4x4 (orthogonal)</description>\n"
"    <levels width=\"4\" height=\"4\" divisor=\"17\">\n"
"        7  13  11   4\n"
"       12  16  14   8\n"
"       10  15   6   2\n"
"        5   9   3   1\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"  <threshold map=\"h6x6o\">\n"
"    <description>Halftone 6x6 (orthogonal)</description>\n"
"    <levels width=\"6\" height=\"6\" divisor=\"37\">\n"
"        7  17  27  14   9   4\n"
"       21  29  33  31  18  11\n"
"       24  32  36  34  25  22\n"
"       19  30  35  28  20  10\n"
"        8  15  26  16   6   2\n"
"        5  13  23  12   3   1\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"  <threshold map=\"h8x8o\">\n"
"    <description>Halftone 8x8 (orthogonal)</description>\n"
"    <levels width=\"8\" height=\"8\" divisor=\"65\">\n"
"        7  21  33  43  36  19   9   4\n"
"       16  27  51  55  49  29  14  11\n"
"       31  47  57  61  59  45  35  23\n"
"       41  53  60  64  62  52  40  38\n"
"       37  44  58  63  56  46  30  22\n"
"       15  28  48  54  50  26  17  10\n"
"        8  18  34  42  32  20   6   2\n"
"        5  13  25  39  24  12   3   1\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"  <threshold map=\"h16x16o\">\n"
"    <!--\n"
"       Direct extract from \"Dithering & Halftoning\" by Gernot Haffmann.\n"
"       This may need some fine tuning for symmetry of the halftone dots,\n"
"       as it was a mathematically formulated pattern.\n"
"    -->\n"
"    <description>Halftone 16x16 (orthogonal)</description>\n"
"    <levels width=\"16\" height=\"16\" divisor=\"257\">\n"
"        4  12  24  44  72 100 136 152 150 134  98  70  42  23  11   3\n"
"        7  16  32  52  76 104 144 160 158 142 102  74  50  31  15   6\n"
"       19  27  40  60  92 132 168 180 178 166 130  90  58  39  26  18\n"
"       36  48  56  80 124 176 188 204 203 187 175 122  79  55  47  35\n"
"       64  68  84 116 164 200 212 224 223 211 199 162 114  83  67  63\n"
"       88  96 112 156 192 216 232 240 239 231 214 190 154 111  95  87\n"
"      108 120 148 184 208 228 244 252 251 243 226 206 182 147 119 107\n"
"      128 140 172 196 219 235 247 256 255 246 234 218 194 171 139 127\n"
"      126 138 170 195 220 236 248 253 254 245 233 217 193 169 137 125\n"
"      106 118 146 183 207 227 242 249 250 241 225 205 181 145 117 105\n"
"       86  94 110 155 191 215 229 238 237 230 213 189 153 109  93  85\n"
"       62  66  82 115 163 198 210 221 222 209 197 161 113  81  65  61\n"
"       34  46  54  78 123 174 186 202 201 185 173 121  77  53  45  33\n"
"       20  28  37  59  91 131 167 179 177 165 129  89  57  38  25  17\n"
"        8  13  29  51  75 103 143 159 157 141 101  73  49  30  14   5\n"
"        1   9  21  43  71  99 135 151 149 133  97  69  41  22  10   2\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
"  Halftones - Orthogonally Expanding Circle Patterns\n"
"\n"
"  Added by Glenn Randers-Pehrson, 4 Nov 2010, ImageMagick 6.6.5-6\n"
"\n"
"  Rather than producing a diamond 50% threshold pattern, these\n"
"  continue to generate larger (overlapping) circles.  They are\n"
"  more like a true halftone pattern formed by covering a surface\n"
"  with either pure white or pure black circular dots.\n"
"\n"
"  WARNING: true halftone patterns only use true circles even in\n"
"  areas of highly varying intensity.  Threshold dither patterns\n"
"  can generate distorted circles in such areas.\n"
"-->\n"
"\n"
"  <threshold map=\"c5x5b\" alias=\"c5x5\">\n"
"    <description>Circles 5x5 (black)</description>\n"
"    <levels width=\"5\" height=\"5\" divisor=\"26\">\n"
"         1 21 16 15  4\n"
"         5 17 20 19 14\n"
"         6 21 25 24 12\n"
"         7 18 22 23 11\n"
"         2  8  9 10  3\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"\n"
"  <threshold map=\"c5x5w\">\n"
"    <description>Circles 5x5 (white)</description>\n"
"    <levels width=\"5\" height=\"5\" divisor=\"26\">\n"
"        25 21 10 11 22\n"
"        20  9  6  7 12\n"
"        19  5  1  2 13\n"
"        18  8  4  3 14\n"
"        24 17 16 15 23\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"  <threshold map=\"c6x6b\" alias=\"c6x6\">\n"
"    <description>Circles 6x6 (black)</description>\n"
"    <levels width=\"6\" height=\"6\" divisor=\"37\">\n"
"         1  5 14 13 12  4\n"
"         6 22 28 27 21 11\n"
"        15 29 35 34 26 20\n"
"        16 30 36 33 25 19\n"
"         7 23 31 32 24 10\n"
"         2  8 17 18  9  3\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"  <threshold map=\"c6x6w\">\n"
"    <description>Circles 6x6 (white)</description>\n"
"    <levels width=\"6\" height=\"6\" divisor=\"37\">\n"
"        36 32 23 24 25 33\n"
"        31 15  9 10 16 26\n"
"        22  8  2  3 11 17\n"
"        21  7  1  4 12 18\n"
"        30 14  6  5 13 27\n"
"        35 29 20 19 28 34\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"  <threshold map=\"c7x7b\" alias=\"c7x7\">\n"
"    <description>Circles 7x7 (black)</description>\n"
"    <levels width=\"7\" height=\"7\" divisor=\"50\">\n"
"        3  9 18 28 17  8  2\n"
"       10 24 33 39 32 23  7\n"
"       19 34 44 48 43 31 16\n"
"       25 40 45 49 47 38 27\n"
"       20 35 41 46 42 29 15\n"
"       11 21 36 37 28 22  6\n"
"        4 12 13 26 14  5  1\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"\n"
"  <threshold map=\"c7x7w\">\n"
"    <description>Circles 7x7 (white)</description>\n"
"    <levels width=\"7\" height=\"7\" divisor=\"50\">\n"
"       47 41 32 22 33 42 48\n"
"       40 26 17 11 18 27 43\n"
"       31 16  6  2  7 19 34\n"
"       25 10  5  1  3 12 23\n"
"       30 15  9  4  8 20 35\n"
"       39 29 14 13 21 28 44\n"
"       46 38 37 24 36 45 49\n"
"    </levels>\n"
"  </threshold>\n"
"\n"
"\n"
"<!-- - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
"  Special Purpose Dithers\n"
"-->\n"
"  <threshold map=\"diag5x5\" alias=\"diag\">\n"
"    <description>Simple Diagonal Line Dither</description>\n"
"    <levels width=\"5\" height=\"5\" divisor=\"6\">\n"
"        4 2 1 3 5\n"
"        2 1 3 5 4\n"
"        1 3 5 4 2\n"
"        3 5 4 2 1\n"
"        5 4 2 1 3\n"
"    </levels>\n"
"  </threshold>"
"  <threshold map=\"hlines2x2\" alias=\"hlines2\">\n"
"    <description>Horizontal Lines 2x2</description>\n"
"    <levels width=\"2\" height=\"2\" divisor=\"3\">\n"
"        1   1\n"
"        2   2\n"
"    </levels>\n"
"  </threshold>"
"  <threshold map=\"hlines2x2a\">\n"
"    <description>Horizontal Lines 2x2 (bounds adjusted)</description>\n"
"    <levels width=\"2\" height=\"2\" divisor=\"10\">\n"
"        3   3\n"
"        9   9\n"
"    </levels>\n"
"  </threshold>"
"  <threshold map=\"hlines12x4\" alias=\"hlines\">\n"
"    <description>Horizontal Lines 12x4</description>\n"
"    <levels width=\"12\" height=\"4\" divisor=\"9\">\n"
"      7 8 8 8 8 7 6 6 5 5 5 6\n"
"      2 1 1 1 1 2 3 4 4 4 3 3\n"
"      6 6 5 5 5 6 7 8 8 8 8 7\n"
"      3 4 4 4 3 3 2 1 1 1 1 2\n"
"    </levels>\n"
"  </threshold>"
"\n"
"</thresholds>";
    size_t len = strlen(data);
    FILE *handle = fopen(filename, "w");
    if (handle != NULL) {
        fwrite((void *)data, len, 1, handle);
        fclose(handle);
    } else {
        console_error("Error while opening thresholds.xml for writing: ", strerror(errno));
    }
}

void _IMColors(const unsigned int width, const unsigned int height, const unsigned number, const bool ordered_dither, const char *dither_name) {
    char strsize[64];
    char strnumber[16];
    char strdither[16];
    sprintf(strsize, "%dx%d", width, height);
    sprintf(strnumber, "%d", number);
    sprintf(strdither, "%s", dither_name);
    storeThresholdsXml();
    putenv("MAGICK_CONFIGURE_PATH=/");
    int argcount = 11;
    char *cmdargs[] = {
        "convert",
        "-size",
        strsize,
        "-depth",
        "8",
        (char *)SRC_FILE,
        ordered_dither?"-ordered-dither":"-dither",
        strdither,
        "-colors",
        strnumber,
        (char *)DST_FILE,
        NULL
    };
    MagickWandGenesis();
    ImageInfo *info = AcquireImageInfo();
    ExceptionInfo *e = AcquireExceptionInfo();
    MagickBooleanType cmdres = MagickCommandGenesis(info, ConvertImageCommand, argcount, cmdargs, NULL, e);
    if (cmdres == MagickFalse) console_error("An error occured while executing command.", "");
    if (e->severity != UndefinedException) {
        console_error("Reason: ", e->reason);
        console_error("Description: ", e->description);
    }
    info=DestroyImageInfo(info);
    e=DestroyExceptionInfo(e);
    MagickWandTerminus();
}

void _IMMorphology(const unsigned int width, const unsigned int height, const char *morphology_name, const char *parameters) {
    char strsize[64];
    char morphology[64];
    char params[64];
    sprintf(strsize, "%dx%d", width, height);
    sprintf(morphology, "%s", morphology_name);
    sprintf(params, "%s", parameters);
    int argcount = 8;
    char *cmdargs[] = {
        "convert",
        "-size",
        strsize,
        (char *)SRC_FILE,
        "-morphology",
        morphology,
        params,
        (char *)DST_FILE,
        NULL
    };
    MagickWandGenesis();
    ImageInfo *info = AcquireImageInfo();
    ExceptionInfo *e = AcquireExceptionInfo();
    MagickBooleanType cmdres = MagickCommandGenesis(info, ConvertImageCommand, argcount, cmdargs, NULL, e);
    if (cmdres == MagickFalse) console_error("An error occured while executing command.", "");
    if (e->severity != UndefinedException) {
        console_error("Reason: ", e->reason);
        console_error("Description: ", e->description);
    }
    info=DestroyImageInfo(info);
    e=DestroyExceptionInfo(e);
    MagickWandTerminus();
}

void _IMSigmoidalContrast(const unsigned int width, const unsigned int height, const bool decrease, const int contrast, const int mid_point) {
    char strsize[64];
    char command[64];
    char params[64];
    sprintf(strsize, "%dx%d", width, height);
    sprintf(command, "%csigmoidal-contrast", decrease?'+':'-');
    sprintf(params, "%dx%d%%", contrast, mid_point);
    int argcount = 7;
    char *cmdargs[] = {
        "convert",
        "-size",
        strsize,
        (char *)SRC_FILE,
        command,
        params,
        (char *)DST_FILE,
        NULL
    };
    MagickWandGenesis();
    ImageInfo *info = AcquireImageInfo();
    ExceptionInfo *e = AcquireExceptionInfo();
    MagickBooleanType cmdres = MagickCommandGenesis(info, ConvertImageCommand, argcount, cmdargs, NULL, e);
    if (cmdres == MagickFalse) console_error("An error occured while executing command.", "");
    if (e->severity != UndefinedException) {
        console_error("Reason: ", e->reason);
        console_error("Description: ", e->description);
    }
    info=DestroyImageInfo(info);
    e=DestroyExceptionInfo(e);
    MagickWandTerminus();
}

void _IMCmdResize(const unsigned int width, const unsigned int height, const unsigned int dstWidth, const unsigned int dstHeight, const char *filter) {
    char strsize[64];
    char filter_name[64];
    char params[64];
    sprintf(strsize, "%dx%d", width, height);
    sprintf(filter_name, "%s", filter);
    sprintf(params, "%dx%d!", dstWidth, dstHeight);
    char **cmdargs;
    int argcount = 9;
    char *cmdargsFilter[] = {
        "convert",
        "-size",
        strsize,
        (char *)SRC_FILE,
        "-filter",
        filter_name,
        "-resize",
        params,
        (char *)DST_FILE,
        NULL
    };
    char *cmdargsNoFilter[] = {
        "convert",
        "-size",
        strsize,
        (char *)SRC_FILE,
        "-resize",
        params,
        (char *)DST_FILE,
        NULL
    };
    if (strlen(filter_name) == 0) {
        cmdargs = cmdargsNoFilter;
        argcount = 7;
    } else cmdargs = cmdargsFilter;
    MagickWandGenesis();
    ImageInfo *info = AcquireImageInfo();
    ExceptionInfo *e = AcquireExceptionInfo();
    MagickBooleanType cmdres = MagickCommandGenesis(info, ConvertImageCommand, argcount, cmdargs, NULL, e);
    if (cmdres == MagickFalse) console_error("An error occured while executing command.", "");
    if (e->severity != UndefinedException) {
        console_error("Reason: ", e->reason);
        console_error("Description: ", e->description);
    }
    info=DestroyImageInfo(info);
    e=DestroyExceptionInfo(e);
    MagickWandTerminus();
}

void _IMExposure(const unsigned int width, const unsigned int height, const double stops, const double offset, const double gamma) {
    char strsize[64];
    char params[64];
    char _gamma[64];
    double gain = pow(2.0, stops);
    sprintf(strsize, "%dx%d", width, height);
    sprintf(params, "%f,%f", gain, offset);
    sprintf(_gamma, "%f", gamma);
    int argcount = 14;
    char *cmdargs[] = {
        "convert",
        "-size",
        strsize,
        (char *)SRC_FILE,
        "-colorspace",
        "RGB",
        "-function",
        "polynomial",
        params,
        "-gamma",
        _gamma,
        "-colorspace",
        "sRGB",
        (char *)DST_FILE,
        NULL
    };
    MagickWandGenesis();
    ImageInfo *info = AcquireImageInfo();
    ExceptionInfo *e = AcquireExceptionInfo();
    MagickBooleanType cmdres = MagickCommandGenesis(info, ConvertImageCommand, argcount, cmdargs, NULL, e);
    if (cmdres == MagickFalse) console_error("An error occured while executing command.", "");
    if (e->severity != UndefinedException) {
        console_error("Reason: ", e->reason);
        console_error("Description: ", e->description);
    }
    info=DestroyImageInfo(info);
    e=DestroyExceptionInfo(e);
    MagickWandTerminus();
}

void _IMGlow(const unsigned int width, const unsigned int height, const double amount, const unsigned int softening) {
    char strsize[64];
    char _amount[64];
    char sigma[64];
    sprintf(strsize, "%dx%d", width, height);
    if (softening > 0) sprintf(_amount, "%f", amount - 1.0);
    else sprintf(_amount, "%f", amount);
    if (softening > 0) sprintf(sigma, "0x%f", (double)softening / 3.0);
    int argcount = 8;
    char **cmdargs;
    char *cmdargsNoSoftening[] = {
        "convert",
        "-size",
        strsize,
        (char *)SRC_FILE,
        "-evaluate",
        "multiply",
        _amount,
        (char *)DST_FILE,
        NULL
    };
    char *cmdargsSoftening[] = {
        "convert",
        "-size",
        strsize,
        (char *)SRC_FILE,
        "(",
        "+clone",
        "-evaluate",
        "multiply",
        _amount,
        "-blur",
        sigma,
        ")",
        "-compose",
        "plus",
        "-composite",
        (char *)DST_FILE,
        NULL
    };
    if (softening != 0.0) {
        cmdargs = cmdargsSoftening;
        argcount = 16;
    } else cmdargs = cmdargsNoSoftening;
    MagickWandGenesis();
    ImageInfo *info = AcquireImageInfo();
    ExceptionInfo *e = AcquireExceptionInfo();
    MagickBooleanType cmdres = MagickCommandGenesis(info, ConvertImageCommand, argcount, cmdargs, NULL, e);
    if (cmdres == MagickFalse) console_error("An error occured while executing command.", "");
    if (e->severity != UndefinedException) {
        console_error("Reason: ", e->reason);
        console_error("Description: ", e->description);
    }
    info=DestroyImageInfo(info);
    e=DestroyExceptionInfo(e);
    MagickWandTerminus();
}

int main() {
    is_ready();
    return 0;
}