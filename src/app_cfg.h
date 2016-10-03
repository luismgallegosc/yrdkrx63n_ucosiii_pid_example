/**
    \file app_cfg.h
    \brief Configuration file for the uC/OS-III application.
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

#ifndef __APP_CFG_H__
#define __APP_CFG_H__

/******************************************************************************
*                               TASK PRIORITIES                               *
******************************************************************************/

#define APP_TASK_START_PRIO         1u
#define CONTROLLER_TASK_PRIO        2u
#define SWITCH_DEBOUNCE_TASK_PRIO   3u
#define REFRESH_LCD_TASK_PRIO       4u



/******************************************************************************
*                              TASK STACK SIZES                               *
******************************************************************************/

#define APP_TASK_START_STK_SIZE         512u
#define CONTROLLER_TASK_STK_SIZE        512u
#define SWITCH_DEBOUNCE_TASK_STK_SIZE   512u
#define REFRESH_LCD_TASK_STK_SIZE       512u

#endif /* __APP_CFG_H__ */
