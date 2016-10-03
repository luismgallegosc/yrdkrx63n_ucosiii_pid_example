/**
    \file controller.h
    \brief Header file for the controller library.
    \date Dec 5, 2014
    \author Luis M. Gallegos C.
 */

#ifndef PID_H
#define PID_H

/******************************************************************************
*                                  CONSTANTS                                  *
******************************************************************************/

/** Minimum allowed value for the controller output. */
#define U_MIN 0

/** Maximum allowed value for the controller output. */
#define U_MAX 1023



/******************************************************************************
*                             FUNCTION PROTOTYPES                             *
******************************************************************************/

/**
    \brief Get new PID controller output (OP) signal.
    \param u Pointer to array of previous values of the controller output.
    \param e Pointer to array of previous values of the error.
    \return New PID controller output value
 */
int16_t getOP_PID (int16_t* u, int16_t* e);

/**
    \brief Get new on/off controller output (OP) signal.
    \param er Error value.
    \return New on/off controller output value
 */
int16_t getOP_onOff (int16_t er);

#endif /* PID_H */
