# &micro;C/OS-III example: simple PID controller

Example project for implementing the &micro;C/OS-III real-time operating system (RTOS).
It was implemented in a [YRDKRX63N Renesas Demonstration Kit](https://www.renesas.com/en-us/products/software-tools/boards-and-kits/renesas-demonstration-kits/yrdkrx63n-for-rx63n.html).

This project was originally done for an Embedded Systems class project.
Its purpose was to use and test the &micro;C/OS-III RTOS, implementing tasks and other features.
The system consisted of a PID controller, where the controlled variable was the voltage level in a capacitor, which was read through the RX63N microcontroller ADC input, and altered using the RX63N DAC output.
Since the main objective was the use of the RTOS, the *process* being controlled was kept simple, just enough so the RTOS operations could have an effect on it which could be perceived.

Source code is documented using Doxygen style comments.

>**Warning**:  
    The code is lacking many protections usually included in production-ready code, like timeouts.

**Further development:**
- Use interrupts for debouncing. They are currently implemented using only a task in the RTOS.
  Using interrupts this task could be enabled every time a button is pressed, and disabled when debouncing is finished.
  Interrupts are already set up but disabled using precompiler directives.
- Build and use type definitions for enumerations.
- Change uint8_t type for bool type, where convenient.
- Transform value arrays into simple linked lists.
- Change control variables in PID control structure, from single variables to linked list structure variables.
- Create macros or functions for turning debug pins on/off.


**DOCUMENTATION ON THIS PROJECT IS PENDING**
