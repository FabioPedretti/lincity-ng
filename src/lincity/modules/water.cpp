/* ---------------------------------------------------------------------- *
 * water.c
 * This file is part of lincity.
 * Lincity is copyright (c) I J Peters 1995-1997, (c) Greg Sharp 1997-2001.
 * (c) Corey Keasling, 2004
 * ---------------------------------------------------------------------- */

#include "water.h"

#include <string>                   // for basic_string

#include "modules.h"

void mps_water(int x, int y)
{
    int i;
    const char *p;

    i = 0;
    mps_store_sdd(i++, waterConstructionGroup.name, x, y);
    i++;

    if ( world(x,y)->flags & FLAG_IS_LAKE )
    {   p = N_("Lake");}
    else if ( world(x,y)->flags & FLAG_IS_RIVER )
    {   p = N_("River");}
    else
    {   p = N_("Pond");}
    mps_store_title(i++, p);
/*
#ifdef DEBUG
    mps_store_sd(10, "x", x);
    mps_store_sd(11, "y", y);
    mps_store_sd(12, "altitude", world(x,y)->ground.altitude);

    fprintf(stderr, "water x %i, y %i, Alt %i\n", x, y, world(x,y)->ground.altitude);
#endif
*/
}

/** @file lincity/modules/water.cpp */
