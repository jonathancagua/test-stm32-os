/*
 * timebase.h
 *
 *  Created on: Jul 2, 2022
 *      Author: dev_fw
 */


#ifndef TIMEBASE_H_
#define TIMEBASE_H_

#include <stdint.h>
void tick_init(void);
uint32_t tick_get(void);
void tick_delay(uint32_t delay);
#endif /* TIMEBASE_H_ */
