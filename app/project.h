/****************************************************************************************************
 * FILE:    project.h
 * BRIEF:   Project Header File
 ****************************************************************************************************/

#ifndef PROJECT_H
#define PROJECT_H

/****************************************************************************************************
 * Defines
 ****************************************************************************************************/

#ifdef __UNIT_TEST__
  #define PROJECT_INFINITE_LOOP while(0)
  #define PROJECT_STATIC
#else
  #define PROJECT_INFINITE_LOOP while(1)
  #define PROJECT_STATIC        static
#endif

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/

#ifdef __UNIT_TEST__
  #include "button_driver_test.h"
  #include "cli_callback_test.h"
  #include "cli_command_handler_callback_test.h"
  #include "led_driver_test.h"
#endif

#endif
