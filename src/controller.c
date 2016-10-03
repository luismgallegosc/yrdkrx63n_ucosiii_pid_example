/**
    \file controller.c
    \brief Implementation file for the controller library.
    \date Dec 5, 2014
    \author Luis M. Gallegos C.
 */

/******************************************************************************
*                                INCLUDE FILES                                *
******************************************************************************/

#include <stdint.h>
#include "controller.h"



/******************************************************************************
*                                  CONSTANTS                                  *
******************************************************************************/

/** Controller b coefficient 1. */
const float bc1 = 2.55025;

/** Controller b coefficient 2. */
const float bc2 = 2.828;

/** Controller b coefficient 3. */
const float bc3 = 0.404;



/******************************************************************************
*                          FUNCTION IMPLEMENTATIONS                           *
******************************************************************************/

int16_t getOP_PID (int16_t* u, int16_t* e) {
    int16_t opNew;

    float t2 = 0.0;
    float t3 = 0.0;
    float t4 = 0.0;

    t2 = bc1*e[0];
    t3 = bc2*e[1];
    t4 = bc3*e[2];

    opNew = (int16_t)(u[0] + t2 - t3 + t4);

    if (opNew > U_MAX) {
        opNew = U_MAX;
    }

    if (opNew < U_MIN) {
        opNew = U_MIN;
    }

    return opNew;
}

int16_t getOP_onOff (int16_t er) {
    int16_t opNew;

    if (er < U_MIN) {
        opNew = U_MIN;
    } else {
        opNew = U_MAX;
    }

    return opNew;
}
