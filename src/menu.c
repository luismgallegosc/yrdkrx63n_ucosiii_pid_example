/**
    \file menu.c
    \brief Implementation file for controlling the menu displayed in the LCD.
    \date Dec 5, 2014
    \author Luis M. Gallegos C.
 */

/******************************************************************************
*                                INCLUDE FILES                                *
******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include "platform.h"
#include "lcd.h"
#include "controllerSysControl.h"
#include "menu.h"



/******************************************************************************
*                             EXTERNAL VARIABLES                              *
******************************************************************************/

extern void* menuControl;
extern CursorControl cursor;
extern PIDControl pid;
extern ControllerControl controller;



/******************************************************************************
*                          FUNCTION IMPLEMENTATIONS                           *
******************************************************************************/

void printMainMenu (void) {
    lcd_clear();

    lcd_display(LCD_XY(1,1), "PID CONTROL ");
    lcd_display(LCD_XY(1,ON_OFF_SEL),  "> ON   OFF  ");
    lcd_display(LCD_XY(1,MAN_AUT_SEL), "  MAN  AUT  ");

    lcd_display(LCD_XY(1,SP_SEL), "   SP:   0 %");
    lcd_display(LCD_XY(1,OP_SEL), "   OP:   0 %");
    lcd_display(LCD_XY(1,PV_SEL), "   PV:   0 %");

    lcd_display_inverted(OFF_POS, "OFF");
    lcd_display_inverted(MAN_POS, "MAN");
}

void onOffSelectionMenu (uint8_t action) {
    if (cursor.mode == VIEW) { // VIEW mode
        switch(action) {
            case 1:
                /* Nothing happens. */
            break;
            case 2:
                /* Change to edit mode. */
                cursor.mode = EDIT;

                /* Change cursor display. */
                lcd_display_inverted(LCD_XY(1, ON_OFF_SEL), ">");
            break;
            case 3:
                /* Clear current cursor. */
                lcd_display(LCD_XY(1, ON_OFF_SEL), " ");

                /* Print new cursor. */
                lcd_display(LCD_XY(1, MAN_AUT_SEL), ">");

                /* Update selection. */
                cursor.selection = MAN_AUT_SEL;

                /* Update menu control pointer. */
                menuControl = manAutSelectionMenu;
            break;
        }
    } else { // EDIT mode
        switch(action) {
            case 1:
                /* Display OFF non-inverted. */
                lcd_display(OFF_POS, "OFF");

                /* Display ON inverted. */
                lcd_display_inverted(ON_POS, "ON");
            break;
            case 2:
                /* Change to view mode. */
                cursor.mode = VIEW;

                /* Display non-inverted cursor. */
                lcd_display(LCD_XY(1, ON_OFF_SEL), ">");
            break;
            case 3:
                /* Display ON non-inverted. */
                lcd_display(ON_POS, "ON");

                /* Display OFF inverted. */
                lcd_display_inverted(OFF_POS, "OFF");
            break;
        }
    }
}

void manAutSelectionMenu (uint8_t action) {
    if (cursor.mode == VIEW) { // VIEW mode
        switch(action) {
            case 1:
                /* Clear current cursor. */
                lcd_display(LCD_XY(1, MAN_AUT_SEL), " ");

                /* Print new cursor. */
                lcd_display(LCD_XY(1, ON_OFF_SEL), ">");

                /* Update selection. */
                cursor.selection = ON_OFF_SEL;

                /* Update menu control pointer. */
                menuControl = onOffSelectionMenu;
            break;
            case 2:
                /* Change to edit mode. */
                cursor.mode = EDIT;

                /* Change cursor display. */
                lcd_display_inverted(LCD_XY(1, MAN_AUT_SEL), ">");
            break;
            case 3:
                /* Clear current cursor. */
                lcd_display(LCD_XY(1, MAN_AUT_SEL), " ");

                /* Print new cursor. */
                lcd_display(LCD_XY(1, SP_SEL), ">");

                /* Update selection. */
                cursor.selection = SP_SEL;

                /* Update menu control pointer. */
                menuControl = spSelectionMenu;
            break;
        }
    } else { // EDIT mode
        switch(action) {
            case 1:
                /* Display AUT non-inverted. */
                lcd_display(AUT_POS, "AUT");

                /* Display MAN inverted. */
                lcd_display_inverted(MAN_POS, "MAN");
            break;
            case 2:
                /* Change to view mode. */
                cursor.mode = VIEW;

                /* Display non-inverted cursor. */
                lcd_display(LCD_XY(1, MAN_AUT_SEL), ">");
            break;
            case 3:
                /* Display MAN non-inverted. */
                lcd_display(MAN_POS, "MAN");

                /* Display AUT inverted. */
                lcd_display_inverted(AUT_POS, "AUT");
            break;
        }
    }
}

void spSelectionMenu (uint8_t action) {
    if (cursor.mode == VIEW) { // VIEW mode
        switch(action) {
            case 1:
                /* Clear current cursor. */
                lcd_display(LCD_XY(1, SP_SEL), " ");

                /* Print new cursor. */
                lcd_display(LCD_XY(1, MAN_AUT_SEL), ">");

                /* Update selection. */
                cursor.selection = MAN_AUT_SEL;

                /* Update menu control pointer. */
                menuControl = manAutSelectionMenu;
            break;
            case 2:
                if (pid.mode == PID_AUTO) {
                    /* Change to edit mode. */
                    cursor.mode = EDIT;

                    /* Change cursor display. */
                    lcd_display_inverted(LCD_XY(1, SP_SEL),">");
                }
            break;
            case 3:
                /* Clear current cursor. */
                lcd_display(LCD_XY(1, SP_SEL), " ");

                /* Print new cursor. */
                lcd_display(LCD_XY(1, OP_SEL), ">");

                /* Update selection. */
                cursor.selection = OP_SEL;

                /* Update menu control pointer. */
                menuControl = opSelectionMenu;
            break;
        }
    } else { // EDIT mode
        switch(action) {
            case 1:
            case 3:
                /* Nothing happens. */
            break;
            case 2:
                /* Display non-inverted cursor. */
                lcd_display(LCD_XY(1, SP_SEL), ">");

                /* Change to view mode. */
                cursor.mode = VIEW;
            break;
        }
    }
}

void opSelectionMenu (uint8_t action) {
    if (cursor.mode == VIEW) { // VIEW mode
        switch(action) {
            case 1:
                /* Clear current cursor. */
                lcd_display(LCD_XY(1, OP_SEL), " ");

                /* Print new cursor. */
                lcd_display(LCD_XY(1, SP_SEL), ">");

                /* Update selection. */
                cursor.selection = SP_SEL;

                /* Update menu control pointer. */
                menuControl = spSelectionMenu;
            break;
            case 2:
                if (pid.mode == PID_MAN) {
                    /* Change to edit mode. */
                    cursor.mode = EDIT;

                    /* Change cursor display. */
                    lcd_display_inverted(LCD_XY(1, OP_SEL),">");
                }
            break;
            case 3:
                /* Nothing happens. */
            break;
        }
    } else { // EDIT mode
        switch(action) {
            case 1:
            case 3:
                /* Nothing happens. */
            break;
            case 2:
                /* Display non-inverted cursor. */
                lcd_display(LCD_XY(1, OP_SEL), ">");

                /* Change to view mode. */
                cursor.mode = VIEW;
            break;
        }
    }
}

void printSP (void) {
    char spString[4];
    int8_t percent;

    if (pid.mode == PID_AUTO) {
        sprintf(spString, "%3d", pid.spPercent);

        if ((controller.mode == EDIT) && (controller.selection == SP_SEL)) {
            lcd_display_inverted(SP_POS, (const uint8_t*)spString);
        } else {
            lcd_display(SP_POS, (const uint8_t*)spString);
        }
    } else {
        /* Convert raw to percent. */
        percent = toPercent(pid.sp);

        pid.spPercent = percent;

        /* Format value. */
        sprintf(spString, "%3d", percent);

        lcd_display(SP_POS, (const uint8_t*)spString);
    }
}

void printOP (void) {
    char opString[4];
    int8_t percent;

    if (pid.mode == PID_MAN) {
        sprintf(opString, "%3d", pid.opPercent);

        if ((controller.mode == EDIT) && (controller.selection == OP_SEL))
            lcd_display_inverted(OP_POS, (const uint8_t*)opString);
        else
            lcd_display(OP_POS, (const uint8_t*)opString);
    } else {
        /* Convert raw to percent. */
        percent = toPercent(pid.op);

        pid.opPercent = percent;

        /* Format value. */
        sprintf(opString, "%3d", percent);

        lcd_display(OP_POS, (const uint8_t*)opString);
    }
}

void printPV (void) {
    char pvString[4];
    int8_t percent;

    /* Convert raw to percent. */
    percent = toPercent(pid.pv);
    pid.pvPercent = percent;

    /* Format value. */
    sprintf(pvString, "%3d", percent);

    lcd_display(PV_POS, (const uint8_t*)pvString);
}
