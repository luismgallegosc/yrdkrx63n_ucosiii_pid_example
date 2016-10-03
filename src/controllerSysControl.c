/**
    \file controllerSysControl.c
    \brief Implementation file for the system control library.
    \date Dec 5, 2014
    \author Luis M. Gallegos C.
 */

/******************************************************************************
*                                INCLUDE FILES                                *
******************************************************************************/

#include <stdint.h>
#include "platform.h"
#include "S12ADC.h"
#include "menu.h"
#include "controllerSysControl.h"



/******************************************************************************
*                             EXTERNAL VARIABLES                              *
******************************************************************************/

extern void* sysControl;
extern PIDControl pid;
extern ControllerControl controller;



/******************************************************************************
*                          FUNCTION IMPLEMENTATIONS                           *
******************************************************************************/

void initController (void) {
    int16_t pvADC_counts;

    /* Start the A/D converter. */ 
    pvADC_start();

    /* Wait for the conversion to complete. */
    while (S12ADC_conversion_complete() == false) {
    }

    /* Read PV from ADC. */
    pvADC_counts = (int16_t)pvADC_read();

    /* Discard first two bits to have same resolution as DAC. */
    pvADC_counts = pvADC_counts >> 2;

    /* Convert raw data to percent. */
    pid.pvPercent = toPercent(pvADC_counts);

    /* Save raw data. */
    pid.pv = pvADC_counts;

    /* Turn LED13 off. */
    LED13 = LED_OFF;

    /* Turn LED7 on. */
    LED7 = LED_ON;
}

void onOffSelectionControl (uint8_t action) {
    static uint8_t activeTemp = PID_OFF;

    if (controller.mode == VIEW) { // VIEW mode
        switch(action) {
            case 1:
                /* Nothing happens. */
            break;
            case 2:
                activeTemp = pid.active;

                /* Change to edit mode. */
                controller.mode = EDIT;
            break;
            case 3:
                /* Update selection. */
                controller.selection = MAN_AUT_SEL;

                /* Update PID system control pointer. */
                sysControl = manAutSelectionControl;
            break;
        }
    } else { // EDIT mode
        switch(action) {
            case 1:
                activeTemp = PID_ON;
            break;
            case 2:
                if (activeTemp == PID_ON) {
                    /* Turn controller on. */
                    /* Turn LED4 on. */
                    LED4 = LED_ON;

                } else {
                    /* Turn controller off. */
                    /* Turn LED4 off. */
                    LED4 = LED_OFF;
                } 

                pid.active = activeTemp;

                /* Change to view mode. */
                controller.mode = VIEW;
            break;
            case 3:
                activeTemp = PID_OFF;
            break;
        }
    }
}

void manAutSelectionControl (uint8_t action) {
    static uint8_t modeTemp = PID_MAN;

    if (controller.mode == VIEW) // VIEW mode
        switch(action) {
            case 1:
                /* Update selection. */
                controller.selection = ON_OFF_SEL;

                /* Update PID system control pointer. */
                sysControl = onOffSelectionControl;
            break;
            case 2:
                modeTemp = pid.mode;

                /* Change to edit mode. */
                controller.mode = EDIT;
            break;
            case 3:
                /* Update selection. */
                controller.selection = SP_SEL;

                /* Update PID system control pointer. */
                sysControl = spSelectionControl;
            break;
        }
    else { // EDIT mode
        switch(action) {
            case 1:
                modeTemp = PID_MAN;
            break;
            case 2:
                if (modeTemp == PID_MAN) {
                    /* Switch controller to manual. */
                    /* Turn LED7 on. */
                    LED7 = LED_ON;

                    /* Turn LED13 off. */
                    LED13 = LED_OFF;

                    pid.opPercent = toPercent(pid.op);
                } else {
                    /* Switch controller to automatic. */
                    /* Turn LED13 on. */
                    LED13 = LED_ON;

                    /* Turn LED7 off. */
                    LED7 = LED_OFF;

                    pid.spPercent = toPercent(pid.sp);
                }

                pid.mode = modeTemp;

                /* Change to view mode. */
                controller.mode = VIEW;
            break;
            case 3:
                modeTemp = PID_AUTO;
            break;
        }
    }
}

void spSelectionControl (uint8_t action) {
    int16_t spTemp;
    // static int8_t spPercentTemp;

    if (controller.mode == VIEW) { // VIEW mode
        switch(action) {
            case 1:
                /* Update selection. */
                controller.selection = MAN_AUT_SEL;

                /* Update PID system control pointer. */
                sysControl = manAutSelectionControl;
            break;
            case 2:
                /* Edit only allowed in automatic mode. */
                if (pid.mode == PID_AUTO) {
                    /* Change to edit mode. */
                    controller.mode = EDIT;
                    // spPercentTemp = pid.spPercent;
                }
            break;
            case 3:
                /* Update selection. */
                controller.selection = OP_SEL;

                /* Update PID system control pointer. */
                sysControl = opSelectionControl;
            break;
        }
    } else { // EDIT mode
        switch(action) {
            case 1:
                if (pid.spPercent < 100)
                    pid.spPercent++;
            break;
            case 2:
                /* Convert percent to raw data. */
                spTemp = toRaw(pid.spPercent);

                /* Update controller setting. */
                pid.sp = spTemp;

                /* Change to view mode. */
                controller.mode = VIEW;

                /* Save new percent value. */
                // pid.spPercent = spPercentTemp;
            break;
            case 3:
                if (pid.spPercent > 0)
                    pid.spPercent--;
            break;
        }
    }
}

void opSelectionControl (uint8_t action) {
    int16_t opTemp;
    // static int8_t opPercentTemp;

    if (controller.mode == VIEW) { // VIEW mode
        switch(action) {
            case 1:
                /* Update selection. */
                controller.selection = SP_SEL;

                /* Update PID system control pointer. */
                sysControl = spSelectionControl;
            break;
            case 2:
                /* Edit only allowed in manual mode. */
                if (pid.mode == PID_MAN) {
                    /* Change to edit mode. */
                    controller.mode = EDIT;
                    // pid.opPercentView = pid.opPercent;
                }
            break;
            case 3:
                /* Nothing happens. */
            break;
        }
    } else { // EDIT mode
        switch(action) {
            case 1:
                if (pid.opPercent < 100)
                    pid.opPercent++;
            break;
            case 2:
                /* Convert percent to raw data. */
                opTemp = toRaw(pid.opPercent);

                /* Update controller setting. */
                pid.op = opTemp;

                /* Change to view mode. */
                controller.mode = VIEW;

                /* Save new percent value. */
                // pid.opPercent = pid.opPercentView;
            break;
            case 3:
                if (pid.opPercent > 0)
                    pid.opPercent--;
            break;
        }
    }
}

int8_t toPercent (int16_t raw) {
    int32_t temp;

    temp = 100*raw;

    return temp/1023;
}

int16_t toRaw (int8_t percent) {
    int32_t temp;

    temp = 1023*percent;

    return temp/100;
}
