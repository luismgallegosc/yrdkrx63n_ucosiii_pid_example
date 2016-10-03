/**
    \file DAC.h
    \brief Header file for the D/A converter library.
    \date Dec 5, 2014
    \author Luis M. Gallegos C.
 */

#ifndef DAC_H
#define DAC_H

/******************************************************************************
*                                  CONSTANTS                                  *
******************************************************************************/

/** Maximum DAC value. */
#define DAC_MAX 0x3FF

/** Positive reference voltage value. */
#define VREFH 3.3

/** Negative reference voltage value. */
#define VREFL 0.0



/******************************************************************************
*                             FUNCTION PROTOTYPES                             *
******************************************************************************/

/**
    \brief Initialize digital-to-analog converter.
    \return None
 */
void DAC_init (void);

/**
    \brief Write new value to DAC data register.
    \param dacNewValue New DAC data register value.
    \return None
 */
void DAC_set (uint16_t dacNewValue);

/**
    \brief Read DAC data register.
    \return DAC data register value.
 */
uint16_t DAC_read (void);

#endif /* DAC_H */
