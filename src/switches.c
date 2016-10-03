/**
    \file switches.c
    \brief Implementation file for the switches control library.
    \date Dec 5, 2014
    \author Luis M. Gallegos C.
 */

/******************************************************************************
*                                INCLUDE FILES                                *
******************************************************************************/

#include <stdint.h>
#include <machine.h>
#include <includes.h>
#include <stdio.h>
#include "platform.h"
#include "switches.h"



/******************************************************************************
*                          LOCAL FUNCTION PROTOTYPES                          *
******************************************************************************/

/**
    \brief Switch 1 interrupt service routine.
    \return None
 */
static void sw1_isr (void);

/**
    \brief Switch 2 interrupt service routine.
    \return None
 */
static void sw2_isr (void);

/**
    \brief Switch 3 interrupt service routine.
    \return None
 */
static void sw3_isr (void);



/******************************************************************************
*                          FUNCTION IMPLEMENTATIONS                           *
******************************************************************************/

void switches_initialize (void) {
    /* YRDK RX63N switches pins/IRQ's connections:
        Switch  Port    IRQ
        ---------------------
        SW1     P4.0    IRQ8
        SW2     P4.1    IRQ9
        SW3     P4.4    IRQ12
    */

    /* Make the pins inputs. */
    PORT4.PDR.BIT.B0 = 0;
    PORT4.PDR.BIT.B1 = 0;
    PORT4.PDR.BIT.B4 = 0;

    /* Set P40, P41, and P44 mode. */
    PORT4.PMR.BYTE  = 0x00;     /* All GPIO for now. */

#if (SWITCH_IRQ_ENABLED)
    /* Set P40, P41, and P44 for IRQx function. */
    PORT4.PMR.BYTE  = 0x13;

    /* Set P40, P41, and P44 pin function. */
    MPC.P40PFS.BYTE = 0x40;     /* P40 is used as IRQ pin. */
    MPC.P41PFS.BYTE = 0x40;     /* P41 is used as IRQ pin. */
    MPC.P44PFS.BYTE = 0x40;     /* P44 is used as IRQ pin. */

    /* Set IRQ type (falling edge). */
    ICU.IRQCR[8].BIT.IRQMD = 0x01;
    ICU.IRQCR[9].BIT.IRQMD = 0x01;
    ICU.IRQCR[12].BIT.IRQMD = 0x01;

    /* Set interrupt priority to 3. */
    IPR(ICU,IRQ8) = 0x03;
    IPR(ICU,IRQ9) = 0x03;
    IPR(ICU,IRQ12) = 0x03;

    /* Clear any pending interrupts. */
    IR(ICU,IRQ8) = 0;
    IR(ICU,IRQ9) = 0;
    IR(ICU,IRQ12) = 0;

    /* Enable the interrupts. */
    IEN(ICU,IRQ8) = 1;
    IEN(ICU,IRQ9) = 1;
    IEN(ICU,IRQ12) = 1;
#endif
}

uint8_t read_switches (void) {
    uint8_t switches_read;

    /* Read switches port, disable additional bits. */
    switches_read = (SWITCH_IN_PORT | SWITCHES_ON);

    return switches_read;
}



/******************************************************************************
*                         INTERRUPT SERVICE ROUTINES                          *
******************************************************************************/

#pragma interrupt (sw1_isr (vect=VECT(ICU,IRQ8)))
void sw1_isr (void) {
    nop();
    /* TODO: Add some debouncing! */
}

#pragma interrupt (sw2_isr (vect=VECT(ICU,IRQ9)))
void sw2_isr (void) {
    nop();
    /* TODO: Add some debouncing! */
}

#pragma interrupt (sw3_isr (vect=VECT(ICU,IRQ12)))
void sw3_isr (void) {
    nop();
    /* TODO: Add some debouncing! */
}
