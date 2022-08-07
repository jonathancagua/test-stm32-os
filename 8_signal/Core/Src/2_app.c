// Escriba un codigo en C para la CIAA que dado q7_t a= 0x040 y q7_t b=0x23,
// calcule q7_t c = a*b e imprima el resultado en hexadecimal y decimal.
// Indique su política con respecto al redondeo
// Resultado: q7: dec: 17 hex: 0x11 float:0.13281250000000000000
// el valor fue truncado el resultado era 0x1180 pero fue dejado 0x11 lo cual cambia su valor
// al que el realmente se debia obtener.
#ifdef app2
#include "arm_math.h"
//#include "arm_const_structs.h"
#include "2_app.h"
#include "usart.h"



q7_t multiQ7(q7_t a,q7_t b)
{
   q15_t ans;
   ans=a*b;
   ans<<=1;
   return ((q7_t) (ans>>8));
}

uint16_t printQ7(q7_t n ,char *buf)
{
   int i;
   float ans=(n&0x80)?-1:0;
   for(i=1;i<8;i++)
   {
      if(n&(0x80>>i)){
         ans+=1.0/(1U<<i);
      }
   }
   return sprintf(buf,"q7: dec: %i hex: 0x%x float:%.20f\r\n",n,n,ans);
}



void app2_main( void ) {
   int16_t len;
   char buf [200] = {0};

   q7_t a = 0x40;
   q7_t b = 0x23;

   while(1) {
      len=printQ7(a,buf);
      HAL_UART_Transmit(&huart1,(const uint8_t *)buf, len, 0xffff);
      len=printQ7(b,buf);
      HAL_UART_Transmit(&huart1,(const uint8_t *)buf, len, 0xffff);
      len=printQ7(multiQ7(a,b),buf);
      HAL_UART_Transmit(&huart1,(const uint8_t *)buf, len, 0xffff);
      HAL_Delay(10000);
   }
}
#endif
