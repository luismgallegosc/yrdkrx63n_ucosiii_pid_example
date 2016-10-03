/**
    \file DAC.c
    \brief Implementation file for the D/A converter library.
    \date Dec 5, 2014
    \author Luis M. Gallegos C.
 */

/******************************************************************************
*                                INCLUDE FILES                                *
******************************************************************************/

#include <stdint.h>
#include "platform.h"
#include "dac.h"



/******************************************************************************
*                          FUNCTION IMPLEMENTATIONS                           *
******************************************************************************/

void DAC_init (void) {
#ifdef PLATFORM_BOARD_RDKRX63N
    SYSTEM.PRCR.WORD = 0xA50B;  /* Protect off */
#endif

    /* Power up the DAC. */
    MSTP(DA) = 0;

    /* Set up port 05 (DA1) as analog output source. */
    PORT0.PODR.BIT.B5 = 0;    /* Clear I/O pin data register to low output. */
    PORT0.PDR.BIT.B5  = 0;    /* Set I/O pin direction to input. */
    PORT0.PMR.BIT.B5  = 0;    /* First set I/O pin to GPIO mode. */
    MPC.P05PFS.BYTE = 0x80;   /* Set port to analog input, no interrupt. */

#ifdef PLATFORM_BOARD_RDKRX63N
    SYSTEM.PRCR.WORD = 0xA500; /* Protect on */
#endif

    /* DADR1: D/A Data Register 0 (with DPSEL = 0 in DADPR)
    b15:b10  Reserved  These bits are read as 0. The write value should be 0.
    b9:b0              Value to convert to analog

    Output voltage = (DADR1 / 1024) * VREFH
    */
    DA.DADR1 = 0x0000; // Initial output voltage = 0 V

    /* DACR: D/A Control Register
    b7    DAOE1    1  D/A conversion of channel 1 is enabled.
    b6    DAOE0    0  Analog output of channel 0 (DA0) is disabled.
    b5    DAE      0  D/A conversion is independently controlled on
                      channels 0 and 1.
    b4:b0 Reserved    These bits are read as 1. The write value should be 1.
    */
    DA.DACR.BYTE = 0x9F;

    /* DADPR: D/A DADRm Format Select Register
    b7    DPSEL    0  Data is flush with the right end of the D/A
                      data register.
    b6:b0 Reserved    These bits are read as 0. The write value should be 0.
    */
    DA.DADPR.BYTE = 0x00;

    /* DAADSCR: D/A A/D Synchronous Start Control Register
    b7    DAADST   0  D/A converter operation does not synchronize with 10-bit
                      A/D converter operation.
                      (measure against interference between D/A and A/D
                      conversion is disabled)
    b6:b0 Reserved    These bits are read as 0. The write value should be 0.
    */
    DA.DAADSCR.BYTE = 0x00;
}

void DAC_set (uint16_t dacNewValue) {
    /* Check if new value is in allowed range. */
    if (dacNewValue > DAC_MAX) {
        dacNewValue = DAC_MAX;
    }

    DA.DADR1 = dacNewValue;
}

uint16_t DAC_read (void) {
    uint16_t dacValue;

    /* Read the data register for DA1. */
    dacValue = DA.DADR1;

    return dacValue;
}
