//
//  bgp.h
//  tobgp
//
//  Created by Simon Gornall on 15/02/2018.
//  Copyright © 2018 Simon Gornall. All rights reserved.
//

#ifndef bgp_h
#define bgp_h

#include <stdio.h>
#include "ppm.h"

int writeApacBGP(char *fp, int w, int h, uint8_t *hue, uint8_t *lum);

#endif /* bgp_h */
