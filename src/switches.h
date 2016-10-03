/**
    \file switches.h
    \brief Header file for the switches control library.
    \date Dec 5, 2014
    \author Luis M. Gallegos C.
 */

#ifndef _SWITCHES_H_
#define _SWITCHES_H_

/******************************************************************************
*                                INCLUDE FILES                                *
******************************************************************************/

#include <os.h>
#include "iodefine.h"



/******************************************************************************
*                                  CONSTANTS                                  *
******************************************************************************/

/** Swith interrupt request enable flag.*/
#define SWITCH_IRQ_ENABLED (0)


/** Switch 1 port bit number. */
#define SWITCH_1_PORT_BIT (0)

/** Switch 2 port bit number. */
#define SWITCH_2_PORT_BIT (1)

/** Switch 3 port bit number. */
#define SWITCH_3_PORT_BIT (4)


/** Switches input port. */
#define SWITCH_IN_PORT PORT4.PIDR.BYTE


/** Switch 1 input pin. */
#define SWITCH_1_IN_PIN PORT4.PIDR.BIT.B0

/** Switch 2 input pin. */
#define SWITCH_2_IN_PIN PORT4.PIDR.BIT.B1

/** Switch 3 input pin. */
#define SWITCH_3_IN_PIN PORT4.PIDR.BIT.B4


/** Switch-1-on port value. */
#define SWITCH_1_ON (~(0x01<<SWITCH_1_PORT_BIT)) // 254           1111_1110

/** Switch-2-on port value. */
#define SWITCH_2_ON (~(0x01<<SWITCH_2_PORT_BIT)) // 253           1111_1101

/** Switch-3-on port value. */
#define SWITCH_3_ON (~(0x01<<SWITCH_3_PORT_BIT)) // 239           1110_1111

/** All-switches-on port value. */
#define SWITCHES_ON (SWITCH_1_ON & SWITCH_2_ON & SWITCH_3_ON)  // 1110_1100

/** All-switches-off port value. */
#define SWITCHES_OFF (0xFF)


/** Maximum debounce count. */
#define DEBOUNCE_MAX (3)



/******************************************************************************
*                              TYPE DEFINITIONS                               *
******************************************************************************/

/** Debounce control structure. */
typedef struct DebounceControlType_struct {
    uint8_t counter;    /**< Debounce counter. */
    uint8_t current;    /**< Current switch state. */
    uint8_t previous;   /**< Previous switch state. */
    uint8_t debounce;   /**< Debouncing active. */
} DebounceControlType;



/******************************************************************************
*                             FUNCTION PROTOTYPES                             *
******************************************************************************/

/**
    \brief Initialize switches input port and interrupts, when enabled.
    \return None
 */
void switches_initialize (void);

/**
    \brief Read switches input port.
    \return Switches status.
 */
uint8_t read_switches(void);

#endif /* _SWITCHES_H_ */
