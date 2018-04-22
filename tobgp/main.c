//
//  main.c
//  tobgp
//
//  Created by Simon Gornall on 1/15/18.
//  Copyright © 2018 Simon Gornall. All rights reserved.
//

#include <string.h>

#include "args.h"
#include "bgp.h"
#include "palette.h"
#include "ppm.h"

/****************************************************************************\
|* Declare prototypes
\****************************************************************************/
void usage(void);
char * outputFile(char *inputname, char *extension);

/****************************************************************************\
|* Program entry
\****************************************************************************/
int main(int argc, char * argv[])
	{
	/************************************************************************\
	|* Get the commandline arguments
	\************************************************************************/
	if (argc < 2 || argString(argc, argv, "-h", NULL) != NULL)
		usage();

	int visual = argFlag(argc, argv, "-v", 0);
		visual = (visual == 0) ? 1 : 4;
		
	char *inFile 	= argString(argc, argv, "-i", NULL);
	char *outFile	= argString(argc, argv, "-o", NULL);
	char *pFile		= argString(argc, argv, "-p", NULL);
	char *aFile		= argString(argc, argv, "-a", NULL);
	int convert		= argFlag(argc, argv, "-c", 0);

	if (inFile == NULL)
		usage();
	if (outFile == NULL)
		outFile = strdup(outputFile(inFile, ".out.ppm"));
		
	/************************************************************************\
	|* Read in the palette file
	\************************************************************************/
	rgb palette[256];
	if (pFile)
		{
		if (!paletteReadPPM(pFile, palette))
			{
			fprintf(stderr, "Cannot read in palette PPM file\n");
			exit(-3);
			}
		}
	else if (aFile)
		{
		if (!paletteReadAct(aFile, palette))
			{
			fprintf(stderr, "Cannot read in palette ACT file\n");
			exit(-3);
			}
		pFile = aFile;
		}
		
	/************************************************************************\
	|* Read in the PPM file
	\************************************************************************/
	int W,H;
	rgb *orig = readPPM(inFile, &W, &H);
	if (orig == NULL)
		exit(-2);
	
	/************************************************************************\
	|* We want to average 8 pixels (and optionally replicate 4x) in the X
	|* direction, and 2 pixels in the Y direction
	\************************************************************************/
	int w	= (visual == 4) ? (W/2) : W/8;
	int h	= H/2;
	int dx 	= (visual == 4) ? 2 : 8;
	
	/************************************************************************\
	|* Reserve memory for the resulting image
	\************************************************************************/
	rgb *out = (rgb *) malloc(w*h*sizeof(rgb));
	if (out == NULL)
		{
		fprintf(stderr, "Couldn't allocate memory for %dx%d pixels\n", w, h);
		exit(-3);
		}

	uint8_t *hues = (uint8_t *) malloc(w*h);
	uint8_t *lums = (uint8_t *) malloc(w*h);
	
	/************************************************************************\
	|* Loop over the original image and create the output
	\************************************************************************/
	uint8_t hue, lum;
	int pCount = 0;
	for (int y=0; y<h; y++)
		for (int x=0; x<w; x+=visual)
			{
			rgbi sum = {0,0,0};
			for (int yy=y*2; yy<y*2+2; yy++)
				for (int xx=x*dx; xx<x*dx+8; xx++)
					{
					rgb *pixel = orig + yy * W + xx;
					sum = addPixel(sum, *pixel);
					}
				
			rgb pix = avgPixel(sum, 16);
			rgb *fill = out + y * w  + x;
			
			if (pFile)
				{
				pix = paletteFind(palette, pix, &hue, &lum);
				hues[pCount] 	= hue;
				lums[pCount++]	= lum;
				}
				
			for (int v=0; v<visual; v++)
				*fill ++ = pix;
			}
		
	/************************************************************************\
	|* If we have a conversion request, write that out too
	\************************************************************************/
	if (convert && pFile)
		{
		char * cFile = outputFile(inFile, ".bgp");
		printf("Write %d x %d\n", w, h);
		writeApacBGP(cFile, w, h, hues, lums);
		}
		
	/************************************************************************\
	|* Write a PPM file with the output data
	\************************************************************************/
	FILE *fp = fopen(outFile, "wb");
	fprintf(fp, "P6\n%d %d\n255\n", w, h);
	fwrite(out, h, w*3, fp);
	fclose(fp);
		
	/************************************************************************\
	|* Write a PGM file with the output hues
	\************************************************************************/
	fp = fopen("hues.pgm", "wb");
	fprintf(fp, "P5\n%d %d\n255\n", w, h);
	fwrite(hues, h, w, fp);
	fclose(fp);
		
	/************************************************************************\
	|* Write a PGM file with the output luninances
	\************************************************************************/
	fp = fopen("lum.pgm", "wb");
	fprintf(fp, "P5\n%d %d\n255\n", w, h);
	fwrite(lums, h, w, fp);
	fclose(fp);

	return 0;
	}

/****************************************************************************\
|* Take an input filename and produce an output filename with the same stem
|* but a different extension
\****************************************************************************/
char * outputFile(char *inputname, char *extension)
	{
	static char buf[1024];
	memset(buf, 0, 1024);
	
	char *lastDot = strrchr(inputname, '.');
	if (lastDot)
		memcpy(buf, inputname, lastDot-inputname);
	else
		strcpy(buf, inputname);
	strcat(buf, extension);
	return buf;
	}

/****************************************************************************\
|* Usage
\****************************************************************************/
void usage(void)
	{
	fprintf(stderr,
		"\nUsage: tobgp [options] -i <file.ppm> [-o <out.ppm>]\n"
		"\n"
		"where [options] are from:\n"
		"\n"
		"  -h  : show this help\n"
		"  -v  : show a viewable image (ie: stretch X by 4)\n"
		"  -p  : load a palette and transform to that palette\n"
		"  -c  : also write conversion to BGP\n"
		"\n"
		);
	exit(0);
	}
