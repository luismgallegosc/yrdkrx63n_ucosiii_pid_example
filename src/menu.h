/**
    \file menu.h
    \brief Header file for controlling the menu displayed in the LCD.
    \date Dec 5, 2014
    \author Luis M. Gallegos C.
 */

#ifndef MENU_H_
#define MENU_H_

/******************************************************************************
*                                  CONSTANTS                                  *
******************************************************************************/

/** Setpoint value position in LCD. */
#define SP_POS (LCD_XY(8,5))

/** Controller output value position in LCD. */
#define OP_POS (LCD_XY(8,6))

/** Process variable value position in LCD. */
#define PV_POS (LCD_XY(8,7))

/** ON option position in LCD. */
#define ON_POS (LCD_XY(3,2))

/** OFF option position in LCD. */
#define OFF_POS (LCD_XY(8,2))

/** Manual option position in LCD. */
#define MAN_POS (LCD_XY(3,3))

/** Automatic option position in LCD. */
#define AUT_POS (LCD_XY(8,3))



/******************************************************************************
*                                ENUMERATIONS                                 *
******************************************************************************/

/** Selected option/variable values. */
enum SelectionType {
    ON_OFF_SEL = 2,     /**< On/off option. */
    MAN_AUT_SEL = 3,    /**< Manual/automatic option. */
    SP_SEL = 5,         /**< Setpoint value. */
    OP_SEL = 6,         /**< Controller output value. */
    PV_SEL = 7          /**< Process variable value. */
};

/** Menu mode: view or edit. */
enum MenuModeType {
    VIEW,   /**< View mode. */
    EDIT    /**< Edit mode.. */
};



/******************************************************************************
*                              TYPE DEFINITIONS                               *
******************************************************************************/

/** Cursor control structure. */
typedef struct CursorControl_struct {
    uint8_t selection;      /**< Current selection. */
    uint8_t mode;           /**< Cursor mode: view or edit. */
} CursorControl;



/******************************************************************************
*                             FUNCTION PROTOTYPES                             *
******************************************************************************/

/**
    \brief Print main menu. Initial setting.
    \return None
 */
void printMainMenu (void);

/**
    \brief Displayed menu actions when on/off option is selected.
    \param action Value for action to be taken, indicated by button press.
    \return None
 */
void onOffSelectionMenu (uint8_t action);

/**
    \brief Displayed menu actions when manual/automatic option is selected.
    \param action Value for action to be taken, indicated by button press.
    \return None
 */
void manAutSelectionMenu (uint8_t action);

/**
    \brief Displayed menu actions when setpoint option is selected.
    \param action Value for action to be taken, indicated by button press.
    \return None
 */
void spSelectionMenu (uint8_t action);

/**
    \brief Displayed menu actions when controller output option is selected.
    \param action Value for action to be taken, indicated by button press.
    \return None
 */
void opSelectionMenu (uint8_t action);

/**
    \brief Print setpoint value.
    \return None
 */
void printSP (void);

/**
    \brief Print controller output value.
    \return None
 */
void printOP (void);

/**
    \brief Print process variable value.
    \return None
 */
void printPV (void);

#endif /* MENU_H_ */
