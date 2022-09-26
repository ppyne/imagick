all: imagick.c
	emcc imagick.c -o imagick.html -sASSERTIONS=1 -sALLOW_MEMORY_GROWTH=1 -sEXPORTED_FUNCTIONS=__IMResize,__IMCharcoal,__IMSketch,__IMQuantize,__IMPosterize,__IMListUniqueColors,__IMAutoLevel,__IMAutoGamma,__IMContrast,__IMColors,__IMMorphology,_malloc,_main -sEXPORTED_RUNTIME_METHODS=ccall,cwrap `pkg-config --cflags --libs MagickWand`