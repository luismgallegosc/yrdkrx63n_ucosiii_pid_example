/**
    \file includes.h
    \brief Header file for additional headers to be included in main app.
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

#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#include <cpu_core.h>
#include <os.h>
#include <bsp.h>

#include <math.h>
#include "lcd.h"
#include "yrdkrx62n_rspi_api.h"
#include "S12ADC.h"
#include "switches.h"
#include "DAC.h"
#include "menu.h"
#include "controllerSysControl.h"
#include "controller.h"
#include "myDebug.h"

#endif /* __INCLUDES_H__ */
