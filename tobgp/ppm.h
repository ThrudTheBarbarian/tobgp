//
//  ppm.h
//  tobgp
//
//  Created by Simon Gornall on 1/15/18.
//  Copyright © 2018 Simon Gornall. All rights reserved.
//

#ifndef ppm_h
#define ppm_h

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct
	{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	} rgb;

typedef struct
	{
	uint32_t r;
	uint32_t g;
	uint32_t b;
	} rgbi;

/****************************************************************************\
|* Read a PPM file
\****************************************************************************/
rgb * readPPM(const char *file, int *W, int *H);

/****************************************************************************\
|* Add a PPM pixel to the larger storage struct (so no overflow)
\****************************************************************************/
rgbi addPixel(rgbi in, rgb pix);

/****************************************************************************\
|* Average an extended RGB struct into an 8-bit one
\****************************************************************************/
rgb 		avgPixel(rgbi in, int divisor);

/****************************************************************************\
|* Return the distance between two rgb structs
\****************************************************************************/
float pixelDistance(rgb p1, rgb p2);

#endif /* ppm_h */
