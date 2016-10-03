/**
    \file myDebug.c
    \brief Implementation file for custom debugging functions.
    \date Dec 5, 2014
    \author Luis M. Gallegos C.
 */

/******************************************************************************
*                                INCLUDE FILES                                *
******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include "platform.h"
#include "myDebug.h"



/******************************************************************************
*                          FUNCTION IMPLEMENTATIONS                           *
******************************************************************************/

void myDebug_init (void) {
    /* Set debugging pins to output mode. */
    PORTE.PDR.BIT.B5 = 1;
    PORTE.PDR.BIT.B6 = 1;

    /* Set debugging pins to low. */
    PORTE.PODR.BIT.B5 = 0;
    PORTE.PODR.BIT.B6 = 0;
}
