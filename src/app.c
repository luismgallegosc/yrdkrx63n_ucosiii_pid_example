/**
    \file app.c
    \brief Main application file.
           Made from a template from uC/OS-III.
    \date Dec 5, 2014
    \author Luis M. Gallegos C.
 */

/******************************************************************************
*                                  uC/OS-III                                  *
*                            The Real-Time Kernel                             *
*                                                                             *
*                (c) Copyright 2011, Micrium, Inc., Weston, FL                *
*                             All Rights Reserved                             *
******************************************************************************/

/******************************************************************************
*                                INCLUDE FILES                                *
******************************************************************************/

#include <includes.h>
#include <stdio.h>



/******************************************************************************
*                              TYPE DEFINITIONS                               *
******************************************************************************/

/** Screen control structure. */
typedef struct ScreenControl_struct {
    uint8_t update;     /**< Screen update flag. */
    uint8_t action;     /**< Requested action, indicated by button press. */
} ScreenControl;



/******************************************************************************
*                               LOCAL VARIABLES                               *
******************************************************************************/

/* Declare task control blocks. */
static OS_TCB AppTaskStartTCB;
static OS_TCB ControllerTaskTCB;
static OS_TCB SwitchDebounceTaskTCB;
static OS_TCB RefreshLCDTaskTCB;

/* Declare task CPU stacks. */
static CPU_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];
static CPU_STK ControllerTaskStk[CONTROLLER_TASK_STK_SIZE];
static CPU_STK SwitchDebounceTaskStk[SWITCH_DEBOUNCE_TASK_STK_SIZE];
static CPU_STK RefreshLCDTaskStk[REFRESH_LCD_TASK_STK_SIZE];

/** System control function pointer. */
void  (*sysControl)(uint8_t) = onOffSelectionControl;

/** Menu control function pointer. */
void (*menuControl)(uint8_t) = onOffSelectionMenu;


/** Screen control structure variable. */
ScreenControl screen = {0, 0};

/** Cursor control structure variable. */
CursorControl cursor = {ON_OFF_SEL, VIEW};

/** PID control structure variable. */
PIDControl pid  = {PID_OFF, PID_MAN, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/** General controller control structure variable. */
ControllerControl controller = {ON_OFF_SEL, VIEW};


/** Array of current and previous setpoint values. */
int16_t spArray[3] = {0, 0, 0};

/** Array of current and previous controller output values. */
int16_t opArray[3] = {0, 0, 0};

/** Array of current and previous process variable values. */
int16_t pvArray[3] = {0, 0, 0};

/** Array of current and previous error values. */
int16_t erArray[3] = {0, 0, 0};



/******************************************************************************
*                             FUNCTION PROTOTYPES                             *
******************************************************************************/

/**
    \brief Startup task.
    \param p_arg Argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
    \return None.
    \note The first line of code is used to prevent a compiler warning because
          'p_arg' is not used. The compiler should not generate any code for
          this statement.
          \par
          Ticker must be initialized only once multitasking has started.
 */
static void AppTaskStart (void *p_arg);

/**
    \brief PID controller task.
    \details Performs the PID control algorithm.
    \param p_arg Argument passed to 'ControllerTask()' by 'OSTaskCreate()'.
    \return None.
    \note The first line of code is used to prevent a compiler warning because
          'p_arg' is not used. The compiler should not generate any code for
          this statement.
 */
static void ControllerTask (void *p_arg);

/**
    \brief Switch debounce task.
    \details Reads the input from switches and does some debouncing.
    \param p_arg Argument passed to 'SwitchDebounceTask()' by 'OSTaskCreate()'.
    \return None.
    \note The first line of code is used to prevent a compiler warning because
          'p_arg' is not used. The compiler should not generate any code for
          this statement.
 */
static void SwitchDebounceTask (void *p_arg);

/**
    \brief Refresh LCD task.
    \details Refreshes the LCD screen with new values from variables and
             switch input.
    \param p_arg Argument passed to 'RefreshLCDTask()' by 'OSTaskCreate()'.
    \return None.
    \note The first line of code is used to prevent a compiler warning because
          'p_arg' is not used. The compiler should not generate any code for
          this statement.
 */
static void RefreshLCDTask (void *p_arg);

/**
    \brief Insert new value into an array of int16_t values.
    \param array Pointer to array of values in which to insert new value.
    \param data New value to insert.
    \return None.
 */
void insertArray (int16_t* array, int16_t data);



/******************************************************************************
*                          FUNCTION IMPLEMENTATIONS                           *
******************************************************************************/

int main (void) {
    OS_ERR  err;

    /* Initialize RX serial peripheral interface. */
    YRDKRX62N_RSPI_Init();

#if (MY_DEBUG_ACTIVE)
    /* Initialize debug ports. */
    myDebug_init();
#endif

    /* Initialize peripherals. */
    lcd_initialize();
    switches_initialize();
    S12ADC_init();
    DAC_init();
    printMainMenu();

    /* Disable all interrupts. */
    CPU_IntDis();

    /* Initialize uC/OS-III. */
    OSInit(&err);

    /* Create the start task. */
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10u,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 0u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    /* Start multitasking (i.e. give control to uC/OS-III). */
    OSStart(&err);

    while (1) {
        ;
    }

    return (0);
}

static void AppTaskStart (void *p_arg) {
   (void)&p_arg;

    OS_ERR err;

    BSP_Init();         /* Initialize BSP functions. */
    CPU_Init();         /* Initialize the uC/CPU services. */
    OS_CPU_TickInit();  /* Initialize uC/OS periodic time source (SysTick). */
#if (OS_CFG_STAT_TASK_EN > 0u)
    /* Compute CPU capacity with no task running. */
    OSStatTaskCPUUsageInit(&err);
#endif

    /* Create PID controller task. */
    OSTaskCreate((OS_TCB     *)&ControllerTaskTCB,
                 (CPU_CHAR   *)"Controller Task",
                 (OS_TASK_PTR ) ControllerTask,
                 (void       *) 0,
                 (OS_PRIO     ) CONTROLLER_TASK_PRIO,
                 (CPU_STK    *)&ControllerTaskStk[0],
                 (CPU_STK_SIZE) CONTROLLER_TASK_STK_SIZE / 10u,
                 (CPU_STK_SIZE) CONTROLLER_TASK_STK_SIZE,
                 (OS_MSG_QTY  ) 0u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    /* Create switch debounce task. */
    OSTaskCreate((OS_TCB     *)&SwitchDebounceTaskTCB,
                 (CPU_CHAR   *)"Switch Debounce Task",
                 (OS_TASK_PTR ) SwitchDebounceTask,
                 (void       *) 0,
                 (OS_PRIO     ) SWITCH_DEBOUNCE_TASK_PRIO,
                 (CPU_STK    *)&SwitchDebounceTaskStk[0],
                 (CPU_STK_SIZE) SWITCH_DEBOUNCE_TASK_STK_SIZE / 10u,
                 (CPU_STK_SIZE) SWITCH_DEBOUNCE_TASK_STK_SIZE,
                 (OS_MSG_QTY  ) 0u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    /* Create LCD refresh task. */
    OSTaskCreate((OS_TCB     *)&RefreshLCDTaskTCB,
                 (CPU_CHAR   *)"Refresh LCD Task",
                 (OS_TASK_PTR ) RefreshLCDTask,
                 (void       *) 0,
                 (OS_PRIO     ) REFRESH_LCD_TASK_PRIO,
                 (CPU_STK    *)&RefreshLCDTaskStk[0],
                 (CPU_STK_SIZE) REFRESH_LCD_TASK_STK_SIZE / 10u,
                 (CPU_STK_SIZE) REFRESH_LCD_TASK_STK_SIZE,
                 (OS_MSG_QTY  ) 0u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    /* Delete task because its work is complete. */
    OSTaskDel(&AppTaskStartTCB, &err);

    while (DEF_ON) {
        ; /* Should not get here! */
    }
}

static void ControllerTask (void *p_arg) {
    (void)&p_arg;

    OS_ERR err;

    int16_t pvADC_counts;

    initController();

    /* Task body, always written as an infinite loop. */
    while (DEF_ON) {
#if (MY_DEBUG_ACTIVE)
        /* Turn debug pin on. */
        MY_DEBUG_1 = MY_DEBUG_ON;
#endif

        if (pid.active == PID_ON) {
            /* Start PV ADC conversion. */
            pvADC_start();

            /* Wait for conversion complete. */
            while (S12ADC_conversion_complete() == false) {
            }


            /* Read PV ADC value. */
            pvADC_counts = (int16_t)pvADC_read();

            /* Discard two LSB to have the same resolution as DAC. */
            pvADC_counts = pvADC_counts >> 2;


            /* Save PV ADC value in PID control structure. */
            pid.pv = pvADC_counts;

            /* Save new PV value in data array. */
            insertArray(pvArray, pvADC_counts);

            if (pid.mode == PID_MAN) {
                /* MANUAL MODE */

                /* Save PV value in SP; SP follows PV in manual mode. */
                pid.sp = pvADC_counts;

                /* Save new SP value in data array. */
                insertArray(spArray, pvADC_counts);

                /* Error = 0, setpoint follows PV. */
                insertArray(erArray, 0);
            } else if (pid.mode == PID_AUTO){
                /* AUTOMATIC MODE */

                /* Compute new error value. */
                pid.er = pid.sp - pid.pv;

                /* Save new error value in data array. */
                insertArray(erArray, pid.er);

                /* Compute new OP value; save in PID control structure. */
                pid.op = getOP_PID(opArray, erArray);
            }

            /* Save new OP value in data array. */
            insertArray(opArray, pid.op);

            /* Set DAC output with new OP value. */
            DAC_set(pid.op);
        }

#if (MY_DEBUG_ACTIVE)
        /* Turn debug pin off. */
        MY_DEBUG_1 = MY_DEBUG_OFF;
#endif

        /* Start task delay. */
        OSTimeDlyHMSM(0u,                       // Hours
                      0u,                       // Minutes
                      0u,                       // Seconds
                      10u,                      // Milliseconds
                      OS_OPT_TIME_HMSM_STRICT,  // STRICT or NON_STRICT
                      &err);
    }
}

static void SwitchDebounceTask (void *p_arg) {
    (void)&p_arg;

    OS_ERR   err;

    /* Define and initialize new debounce control structure variable. */
    static DebounceControlType switchCtrl = {0, 0xFF, 0xFF, 0};

    /* Task body, always written as an infinite loop. */
    while (DEF_ON) {
        /* Read switches status. */
        switchCtrl.current = read_switches();

        /* If debouncing is currently in progress. */
        if (switchCtrl.debounce) {
            /* DEBOUNCE IN PROGRESS. */

            /* Compare current switch status with previous status. */
            if (switchCtrl.current == switchCtrl.previous) {
                /* SWITCH STATUS UNCHAGED. */

                /* Increment debounce counter. */
                switchCtrl.counter++;

                /* Check if max debounce counter value reached. */
                if ((switchCtrl.counter == DEBOUNCE_MAX)) {
                    /* BUTTON PRESSED AND DEBOUNCE TIME IS OVER. */

                    /* Check which button was pressed. */
                    switch (switchCtrl.current) {
                        case SWITCH_1_ON:
                            /* Perform button action. */
                            (*sysControl)(1);

                            /* Prepare action for screen control. */
                            screen.action = 1;
                        break;
                        case SWITCH_2_ON:
                            /* Perform button action. */
                            (*sysControl)(2);

                            /* Prepare action for screen control. */
                            screen.action = 2;
                        break;
                        case SWITCH_3_ON:
                            /* Perform button action. */
                            (*sysControl)(3);

                            /* Prepare action for screen control. */
                            screen.action = 3;
                        break;
                        default:
                            /* Any other value is error, do nothing. */
                            nop();
                        break;
                    }

                    /* Reset debounce counter. */
                    switchCtrl.counter = 0;

                    /* Turn off debounce-in-progress flag. */
                    switchCtrl.debounce = 0;
                }
            } else {
                /* SWITCH STATUS CHANGED WHEN DEBOUNCE WAS IN PROGRESS. */

                /* Reset debounce counter. */
                switchCtrl.counter = 0;

                /* Turn off debounce-in-progress flag. */
                switchCtrl.debounce = 0;
            }
        } else {
            /* DEBOUNCE NOT IN PROGRESS. */

            /* Compare current switch status with previous status. */
            if (switchCtrl.current != switchCtrl.previous) {
                /* SWITCH STATUS CHANGED. */

                /* Turn on debounce-in-progress flag. */
                switchCtrl.debounce = 1;

                /* Update previous switch status with current status. */
                switchCtrl.previous = switchCtrl.current;
            }
        }

        /* Start task delay. */
        OSTimeDlyHMSM(0u,                       // Hours
                      0u,                       // Minutes
                      0u,                       // Seconds
                      50u,                      // Milliseconds
                      OS_OPT_TIME_HMSM_STRICT,  // STRICT or NON_STRICT
                      &err);
    }
}

static void RefreshLCDTask (void *p_arg) {
    (void)&p_arg;

    OS_ERR   err;

    /* Task body, always written as an infinite loop. */
    while (DEF_ON) {
#if (MY_DEBUG_ACTIVE)
        /* Turn debug pin on. */
        MY_DEBUG_2 = MY_DEBUG_ON;
#endif

        /* Check if there is an action pending for the screen. */
        if (screen.action) {
            (*menuControl)(screen.action);
            screen.action = 0;
        }

        /* Update controller values on the screen. */
        printSP();      // Update setpoint.
        printOP();      // Update controller output.
        printPV();      // Update process variable.

#if (MY_DEBUG_ACTIVE)
        /* Turn debug pin off. */
        MY_DEBUG_2 = MY_DEBUG_OFF;;
#endif

        /* Start task delay. */
        OSTimeDlyHMSM(0u,                       // Hours
                      0u,                       // Minutes
                      0u,                       // Seconds
                      500u,                     // Milliseconds
                      OS_OPT_TIME_HMSM_STRICT,  // STRICT or NON_STRICT
                      &err);
    }
}

void insertArray (int16_t* array, int16_t data) {
    array[2] = array[1];
    array[1] = array[0];
    array[0] = data;
}
