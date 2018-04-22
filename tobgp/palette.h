//
//  palette.h
//  tobgp
//
//  Created by Simon Gornall on 29/01/2018.
//  Copyright © 2018 Simon Gornall. All rights reserved.
//

#ifndef palette_h
#define palette_h

#include <stdio.h>
#include "ppm.h"


/****************************************************************************\
|* Read a PPM palette file
\****************************************************************************/
int paletteReadPPM(char *pFile, rgb *palette);

/****************************************************************************\
|* Read an ACT palette file
\****************************************************************************/
int paletteReadAct(char *aFile, rgb *palette);

/****************************************************************************\
|* Find the closest match within the available palette
\****************************************************************************/
rgb paletteFind(rgb *palette, rgb pix, uint8_t *hue, uint8_t *lum);

#endif /* palette_h */
