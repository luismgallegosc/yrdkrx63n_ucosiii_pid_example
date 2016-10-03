/**
    \file S12ADC.c
    \brief Implementation file for the S12ADC A/D converter library.
    \date Dec 5, 2014
    \author Luis M. Gallegos C.
 */

/******************************************************************************
*                                INCLUDE FILES                                *
******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "platform.h"
#include "s12adc.h"



/******************************************************************************
*                          FUNCTION IMPLEMENTATIONS                           *
******************************************************************************/

void S12ADC_init (void) {
#ifdef PLATFORM_BOARD_RDKRX63N
    SYSTEM.PRCR.WORD = 0xA50B;      /* Protect off */
#endif

    /* Power up the S12ADC. */
    MSTP(S12AD) = 0;

    /* Set up port 42 (AN002) as analog input source. The potentiometer is
       used as analog reference for the control system. */
    PORT4.PODR.BIT.B2 = 0;    /* Clear I/O pin data register to low output. */
    PORT4.PDR.BIT.B2  = 0;    /* Set I/O pin direction to input. */
    PORT4.PMR.BIT.B2  = 0;    /* Set I/O pin mode register to GPIO mode. */
    MPC.P42PFS.BYTE = 0x80;   /* Set port to analog input, no interrupt. */

    /* Set up port 43 (AN003) as analog input source. Input is through header
       JN1, pin 12. It is used as value for the process variable. */
    PORT4.PODR.BIT.B3 = 0;    /* Clear I/O pin data register to low output. */
    PORT4.PDR.BIT.B3  = 0;    /* Set I/O pin direction to input. */
    PORT4.PMR.BIT.B3  = 0;    /* Set I/O pin mode register to GPIO mode. */
    MPC.P43PFS.BYTE = 0x80;   /* Set port to analog input, no interrupt. */

#ifdef PLATFORM_BOARD_RDKRX63N
    SYSTEM.PRCR.WORD = 0xA500;      /* Protect on */
#endif

    /* ADCSR: A/D Control Register
    b7    ADST     0 A/D conversion start, Stop a scan conversion process
    b6    ADCS     0 Scan mode select, Single-scan mode
    b5    Reserved 0 Always read as 0. The write value should always be 0.
    b4    ADIE     0 Disables conversion complete IRQ to ICU
    b3:b2 CKS      0 A/D conversion clock select = PCLK/8
    b1    TRGE     0 Disables conversion to start w/ trigger
    b0    EXTRG    0 Trigger select, start by a timer source or software
    */
    S12AD.ADCSR.BYTE = 0x00;

    /* ADANS0: A/D Channel Select Register 0
    b15:b0 ANS0 Selects analog inputs of the channels AN000 to AN015 that are
                subjected to A/D conversion.
                spADC_start() and pvADCstart() functions set this register
                before starting conversion.
    */
    S12AD.ADANS0.WORD = 0x0004; /* Read AN002, connected to potentiometer. */

    /* ADANS1: A/D Channel Select Register 1
    b15:b5 Reserved Always read as 0. The write value should always be 0.
    b4:b0  ANS1     Selects analog inputs of the channels AN016 to AN020 that
                    are subjected to A/D conversion
    */
    S12AD.ADANS1.WORD = 0x0000;

    /* ADADS0: A/D-converted Value Addition Mode Select Register 0
    b15:b0 ADS0 A/D-Converted Value Addition Channel Select for AN000 to AN015.
    */
    S12AD.ADADS0.WORD = 0x0000;

    /* ADADS1: A/D-converted Value Addition Mode Select Register 1
    b15:b5 Res. Always read as 0. The write value should always be 0.
    b4:b0  ADS1 A/D-Converted Value Addition Channel Select for AN016 to AN020.
    */
    S12AD.ADADS1.WORD = 0x0000;

    /* ADADC: A/D-Converted Value Addition Count Select Register
    b1:b0   ADC  00 = 1 time conversion (same as normal conversion)
    */
    S12AD.ADADC.BYTE = 0x00;    /* 1-time conversion. */

    /* ADCER: A/D Control Extended Register
    b15     ADRFMT  0  Right align the data in the result registers.
    b5      ACE     0  Disables automatic clearing of ADDRn after it is read.
    */
    S12AD.ADCER.WORD = 0x0000;  /* Right align data, automatic clearing off. */

    /* ADSTRGR: A/D Start Trigger Select Register
    b7:b4   Reserved     Always read/write 0.
    b3:b0   ADSTRS    0  Software trigger or ADTRG0#.
    */
    S12AD.ADSTRGR.BYTE = 0x00;
}

void spADC_start (void) {
    /* ADANS0: A/D Channel Select Register 0
    b15:b0 ANS0 Selects analog inputs of the channels AN000 to AN015 that are
                subjected to A/D conversion.
    */
    S12AD.ADANS0.WORD = 0x0004; /* Read AN002, connected to potentiometer. */

    /* Start the A/D converter. */
    S12AD.ADCSR.BIT.ADST = 1;
}

void pvADC_start (void) {
    /* ADANS0: A/D Channel Select Register 0
    b15:b0 ANS0 Selects analog inputs of the channels AN000 to AN015 that are
                subjected to A/D conversion.
    */
    S12AD.ADANS0.WORD = 0x0008; /* Read AN003, connected to JN1, 12. */

    /* Start the A/D converter. */
    S12AD.ADCSR.BIT.ADST = 1;
}

bool S12ADC_conversion_complete (void) {
    /* The ADST returns to zero when the conversion is complete. */
    return S12AD.ADCSR.BIT.ADST == 0;
}

uint16_t spADC_read (void) {
    uint16_t adc_result;

    /* Read the result register for AN002, connected to potentiometer. */
    adc_result = S12AD.ADDR2;

    return adc_result;
}

uint16_t pvADC_read (void) {
    uint16_t adc_result;

    /* Read the result register for AN003, connected to JN1, 12. */
    adc_result = S12AD.ADDR3;

    return adc_result;
}
