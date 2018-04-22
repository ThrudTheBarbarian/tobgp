//
//  args.c
//  tobgp
//
//  Created by Simon Gornall on 29/01/2018.
//  Copyright © 2018 Simon Gornall. All rights reserved.
//

#include <string.h>

#include "args.h"

char * argString(int argc, char **argv, char *token, char *deflt)
	{
	for (int i=1; i<argc-1; i++)
		if (strcmp(argv[i], token) == 0)
			return argv[i+1];
	
	return deflt;
	}

int argFlag(int argc, char **argv, char *token, int deflt)
	{
	for (int i=1; i<argc; i++)
		if (strcmp(argv[i], token) == 0)
			return 1;
	
	return deflt;
	}


