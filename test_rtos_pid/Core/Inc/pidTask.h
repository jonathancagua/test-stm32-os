/*
 * pid_Task.h
 *
 *  Created on: Mar 15, 2023
 *      Author: dev_fw
 */

#ifndef INC_PIDTASK_H_
#define INC_PIDTASK_H_

/*=====[Inclusions of public function dependencies]==========================*/

#include "FreeRTOS.h"
#include "task.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/

void pidControlTask( void* taskParmPtr );  // Task declaration

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

#endif /* INC_PIDTASK_H_ */
