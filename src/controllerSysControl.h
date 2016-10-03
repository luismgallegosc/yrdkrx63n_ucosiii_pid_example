/**
    \file controllerSysControl.h
    \brief Header file for the system control library.
    \date Dec 5, 2014
    \author Luis M. Gallegos C.
 */

#ifndef PIDSYSCONTROL_H_
#define PIDSYSCONTROL_H_

/******************************************************************************
*                                ENUMERATIONS                                 *
******************************************************************************/

/** PID controller mode values. */
enum PIDControlMode {
    PID_MAN,    /**< Manual mode. */
    PID_AUTO    /**< Automatic mode. */
};

/** PID power modes.*/
enum PIDPowerMode {
    PID_ON,     /**< Controller on. */
    PID_OFF     /**< Controller off. */
};



/******************************************************************************
*                              TYPE DEFINITIONS                               *
******************************************************************************/

/** PID control structure. */
typedef struct PIDControl_struct {
    uint8_t active;         /**< Controller active flag. */
    uint8_t mode;           /**< Controller mode. */
    int16_t sp;             /**< Setpoint raw value. (0 - 1023)*/
    int16_t op;             /**< Controller output raw value. (0 - 1023)*/
    int16_t pv;             /**< Process variable raw value. (0 - 1023)*/
    int16_t er;             /**< Error raw value. (0 - 1023)*/
    int8_t spPercent;       /**< Setpoint percent value. */
    int8_t spPercentView;   /**< Displayed setpoint percent value. */
    int8_t opPercent;       /**< Controller output percent value. */
    int8_t opPercentView;   /**< Displayed controller output percent value. */
    int8_t pvPercent;       /**< Process variable percent value. */
} PIDControl;

/** General controller control structure. */
typedef struct {
    uint8_t selection;  /**< Selected menu value: on/off, man/auto, OP, SP. */
    uint8_t mode;       /**< Controller mode, view or edit*/
} ControllerControl;


/******************************************************************************
*                             FUNCTION PROTOTYPES                             *
******************************************************************************/

/**
    \brief Initialize controller and peripherals.
    \return None.
 */
void initController (void);

/**
    \brief Controller actions when on/off option is selected.
    \param action Value for action to be taken, indicated by button press.
    \return None.
 */
void onOffSelectionControl (uint8_t action);

/**
    \brief Controller actions when manual/automatic option is selected.
    \param action Value for action to be taken, indicated by button press.
    \return None.
 */
void manAutSelectionControl (uint8_t action);

/**
    \brief Controller actions when setpoint option is selected.
    \param action Value for action to be taken, indicated by button press.
    \return None.
 */
void spSelectionControl (uint8_t action);

/**
    \brief Controller actions when controller output option is selected.
    \param action Value for action to be taken, indicated by button press.
    \return None.
 */
void opSelectionControl (uint8_t action);

/**
    \brief Convert raw value to percent value.
    \param raw Raw value.
    \return Percent value.
 */
int8_t toPercent (int16_t raw);

/**
    \brief Convert percent value to raw value.
    \param percent Percent value.
    \return Raw value.
 */
int16_t toRaw (int8_t percent);

#endif /* PIDSYSCONTROL_H_ */
