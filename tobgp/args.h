//
//  args.h
//  tobgp
//
//  Created by Simon Gornall on 29/01/2018.
//  Copyright © 2018 Simon Gornall. All rights reserved.
//

#ifndef args_h
#define args_h

#include <stdio.h>

char * argString(int argc, char **argv, char *token, char *deflt);
int    argFlag(int argc, char **argv, char *token, int deflt);

#endif /* args_h */
