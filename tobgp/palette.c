//
//  palette.c
//  tobgp
//
//  Created by Simon Gornall on 29/01/2018.
//  Copyright © 2018 Simon Gornall. All rights reserved.
//

#include "palette.h"

static int _numColours 	= 0;
static int _dHue		= 1;


/****************************************************************************\
|* Expects a PPM image with 8 colours on each row, and 16 rows. Will extract
|* the RGB values at the centerpoints of the rectangles.
\****************************************************************************/
int paletteReadPPM(char *pFile, rgb *palette)
	{
	int ok = 0;

	int w, h;
	rgb * pix = readPPM(pFile, &w, &h);
	if (pix)
		{
		int dx 	= w / 8;
		int dy 	= h / 16;
		int y	= dy / 2;
		int idx	= 0;
		
		for (int i=0; i<16; i++)
			{
			int x = dx / 2;
			for (int j=0; j<8; j++)
				{
				rgb p = pix[y*w+x];
				//printf("%d : (%d,%d) = %d,%d,%d\n", idx, x, y, p.r, p.g, p.b);
				palette[idx++] = p;
				x += dx;
				}
			y += dy;
			}
		ok 			= 1;
		_numColours = 128;
		_dHue		= 8;
		}
	
	#if 0
	for (int i=0; i<128; i++)
		{
		if (i%8 ==0)
			printf("\nrow %x\n", i/8);
		printf("%4d : %02x, %02x, %02x\n", i, palette[i].r,
											  palette[i].g,
											  palette[i].b);
		}
	#endif
	
	return ok;
	}

/****************************************************************************\
|* Read an ACT palette file
\****************************************************************************/
int paletteReadAct(char *aFile, rgb *palette)
	{
	int ok = 0;
	
	FILE *fp = fopen(aFile, "rb");
	if (fp)
		{
		fread(palette, 3, 256, fp);
		fclose(fp);
		_numColours = 256;
		_dHue		= 16;
		ok = 1;
		}
	return ok;
	}
	
/****************************************************************************\
|* Find the closest match within the available palette
\****************************************************************************/
rgb paletteFind(rgb *palette, rgb pix, uint8_t *hue, uint8_t *lum)
	{
	rgb best 	= {255,0,0};
	float diff	= 64*65536;
	for (int i=0; i<_numColours; i++)
		{
		float pd = pixelDistance(pix, palette[i]);
		if (pd < diff)
			{
			best 	= palette[i];
			*hue	= i/_dHue;
			*lum	= (i%_dHue) << ((_dHue == 8) ? 1 : 0);
			diff 	= pd;
			}
		}
	
	return best;
	}

