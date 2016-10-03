/**
    \file myDebug.h
    \brief Header file for custom debugging functions.
    \date Dec 5, 2014
    \author Luis M. Gallegos C.
 */

#ifndef MYDEBUG_H
#define MYDEBUG_H

/******************************************************************************
*                                  CONSTANTS                                  *
******************************************************************************/

/** Debugging active flag. */
#define MY_DEBUG_ACTIVE 1

/** Debug-pin-on value. */
#define MY_DEBUG_ON (1)

/** Debug-pin-off value. */
#define MY_DEBUG_OFF (0)

/** Debug pin 1. */
#define MY_DEBUG_1 PORTE.PODR.BIT.B5

/** Debug pin 2. */
#define MY_DEBUG_2 PORTE.PODR.BIT.B6



/******************************************************************************
*                             FUNCTION PROTOTYPES                             *
******************************************************************************/

/**
    \brief Initialize debugging pins.
    \return None
 */
void myDebug_init (void);

#endif /* MYDEBUG_H */
