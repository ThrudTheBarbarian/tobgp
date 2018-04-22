//
//  ppm.c
//  tobgp
//
//  Created by Simon Gornall on 1/15/18.
//  Copyright © 2018 Simon Gornall. All rights reserved.
//

#include <string.h>
#include <math.h>

#include "ppm.h"

/****************************************************************************\
|* Read a PPM file
\****************************************************************************/
rgb * readPPM(const char *file, int *W, int *H)
	{
	/************************************************************************\
	|* Try to open the file
	\************************************************************************/
	FILE *fp = fopen(file, "rb");
	if (fp == NULL)
		{
		fprintf(stderr, "Cannot open '%s' for read\n", file);
		return NULL;
		}
	/************************************************************************\
	|* Read in the PPM file header
	\************************************************************************/
	int depth = 0;
	int count = 0;
	char buf[1024];
	while (count < 3)
		{
		fgets(buf, 1024, fp);
		if (buf[0] == '#')
			continue;
		
		switch (count++)
			{
			case 0:
				if (strncmp(buf, "P6", 2) != 0)
					{
					fprintf(stderr, "Unsupported PPM type '%s'\n", buf);
					fclose(fp);
					return NULL;
					}
				break;
				
			case 1:
				if (sscanf(buf, "%d %d", W, H) != 2)
					{
					fprintf(stderr, "Can't read width and height\n");
					fclose(fp);
					return NULL;
					}
				break;
				
			case 2:
				if (sscanf(buf, "%d", &depth) != 1)
					{
					fprintf(stderr, "Can't read pixel depth\n");
					fclose(fp);
					return NULL;
					}
				if (depth != 255)
					{
					fprintf(stderr, "Unsupported pixel depth %d\n", depth);
					fclose(fp);
					return NULL;
					}
				break;
			}
		}
	
	//printf("%s: width=%d height=%d depth=%d\n", file, *W, *H, depth);
	/************************************************************************\
	|* Read in the PPM data
	\************************************************************************/
	rgb *data = (rgb *) malloc (*W * *H * 3);
	int num   = (int)fread(data, *W, *H * 3, fp);
	if (num != *H * 3)
		{
		fprintf(stderr, "'%s' does not have sufficient data [%d != %d]\n",
				file, num, *H * 3);
		fclose(fp);
		free(data);
		return NULL;
		}
	
	/************************************************************************\
	|* Housekeeping
	\************************************************************************/
	fclose(fp);
	return data;
	}

/****************************************************************************\
|* Add a PPM pixel to the larger storage struct (so no overflow)
\****************************************************************************/
rgbi addPixel(rgbi in, rgb pix)
	{
	in.r += pix.r;
	in.g += pix.g;
	in.b += pix.b;
	return in;
	}

/****************************************************************************\
|* Average an extended RGB struct into an 8-bit one
\****************************************************************************/
rgb avgPixel(rgbi in, int divisor)
	{
	rgb pix;
	pix.r = in.r/divisor;
	pix.g = in.g/divisor;
	pix.b = in.b/divisor;
	return pix;
	}

/****************************************************************************\
|* Return the distance between two rgb structs
\****************************************************************************/
float pixelDistance(rgb p1, rgb p2)
	{
	int dr = p1.r - p2.r;
	int dg = p1.g - p2.g;
	int db = p1.b - p2.b;
	
	float fdr = sqrt(dr * dr) * .299;
	float fdg = sqrt(dg * dg) * .587;
	float fdb = sqrt(db * db) * .114;

	return fdr + fdb + fdg;
	}

