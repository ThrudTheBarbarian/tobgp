//
//  bgp.c
//  tobgp
//
//  Created by Simon Gornall on 15/02/2018.
//  Copyright © 2018 Simon Gornall. All rights reserved.
//

#include <string.h>

#include "bgp.h"


typedef struct
	{
	char header[30];			// BUGBITER_APAC240I_PICTURE_V3.0
	uint8_t type;				// Should be 'A'
	uint8_t xsize;				// 4..88
	uint8_t ysize;				// 1..240
	int8_t hpos;				// in bytes (2 pixels), signed
	int8_t vpos;				// in pixels, signed
	uint8_t bg;					// background colour
	uint8_t nLen;				// Length of name
	} ApacHeader;


static void wordWrite(FILE *fp, int w)
	{
	uint8_t hi		= w >> 8;
	uint8_t lo		= w & 0xFF;
	fwrite(&lo, 1, 1, fp);
	fwrite(&hi, 1, 1, fp);
	}

static void blockWrite(FILE *fp, int w, int h, uint8_t *data)
	{
	uint16_t size = w * h / 2;
	wordWrite(fp, size);

	for (int i=0; i<size*2; i+=2)
		{
		uint8_t d = (data[i] << 4) | ((data[i+1] & 0xf));
		fwrite(&d, 1, 1, fp);
		}
	}


int writeApacBGP(char *filename, int w, int h, uint8_t *hue, uint8_t *lum)
	{
	int ok = 0;
	char *desc		= "No description";
	uint16_t dLen	= strlen(desc);
	uint8_t floyd 	= 0;
	
	ApacHeader hdr 	= {"BUGBITER_APAC240I_PICTURE_V3.0", 'A',0,0,0,0,0,0};
	hdr.xsize 		= w & 0xFF;
	hdr.ysize		= h & 0xFF;
	hdr.nLen		= strlen(filename);
	
	FILE *fp 		= fopen(filename, "wb");
	if (fp)
		{
		fwrite(&hdr, sizeof(ApacHeader), 1, fp);
		fwrite(filename, hdr.nLen, 1, fp);
		fwrite(&floyd, 1, 1, fp);
		fwrite("paletteN.pal", 12, 1, fp);
		
		wordWrite(fp, dLen);
		fwrite(desc, dLen, 1, fp);

		blockWrite(fp, w, h, lum);
		blockWrite(fp, w, h, hue);
		fclose(fp);
		}
	else
		ok = -1;
	return ok;
	}
