

#include <stdint.h>
#include "led.h"
int main(void)
{
	led_init();
    /* Loop forever */
	for(;;){
		led_on();
		for(int i=0;i<90000;i++);
		led_off();
		for(int i=0;i<90000;i++);
	}
}
