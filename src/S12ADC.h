/**
    \file S12ADC.h
    \brief Header file for the S12ADC A/D converter library.
    \date Dec 5, 2014
    \author Luis M. Gallegos C.
 */

#ifndef _S12ADC_H_
#define _S12ADC_H_

/******************************************************************************
*                                INCLUDE FILES                                *
******************************************************************************/

#include <stdbool.h>



/******************************************************************************
*                                  CONSTANTS                                  *
******************************************************************************/

/* Values for conversion of S12ADC counts to voltage. */

/** S12ADC maximum count value. */
#define MAX_COUNTS 4095.0

/** Positive reference voltage value. */
#define VREFH0 3.3

/** Negative reference voltage value. */
#define VREFL0 0.0



/******************************************************************************
*                             FUNCTION PROTOTYPES                             *
******************************************************************************/

/**
    \brief Initialize S12ADC analog-to-digital converter.
    \return None
 */
void S12ADC_init (void);

/**
    \brief Start ADC conversion for setpoint analog input.
    \return None
 */
void spADC_start (void);

/**
    \brief Start ADC conversion for process variable analog input.
    \return None
 */
void pvADC_start (void);

/**
    \brief Check if ADC conversion is completed.
    \return Boolean value for conversion completed (true: complete).
 */
bool S12ADC_conversion_complete (void);

/**
    \brief Read conversion result register for setpoint analog input.
    \return Setpoint ADC conversion value.
 */
uint16_t spADC_read (void);

/**
    \brief Read conversion result register for process variable analog input.
    \return Process variable ADC conversion value.
 */
uint16_t pvADC_read (void);

#endif /* _S12ADC_H_ */
