#include "headfile.h"
#include "key.h"

KEYn_e KEY_Scan()
{
	if(KEY1_PIN == KEY_DOWN){
		BEEP_ON;
		delay_ms(100);
		BEEP_OFF;
		while(KEY1_PIN == KEY_DOWN);
        return KEY_1;
		
		
	}
	else if(KEY2_PIN == KEY_DOWN)
    {
		BEEP_ON;
		while(KEY2_PIN==KEY_DOWN);
		return KEY_2;
		
	}
	else if(KEY3_PIN == KEY_DOWN){
		BEEP_ON;
		delay_ms(100);
		BEEP_OFF;
		if(KEY3_PIN == KEY_DOWN){
			return KEY_3;
		}
	}
	else if(KEY4_PIN == KEY_DOWN){
		BEEP_ON;
		delay_ms(100);
		BEEP_OFF;
        if(KEY4_PIN == KEY_DOWN)
        {
			return KEY_4;
		}

	}
	else if(KEY5_PIN == KEY_DOWN){
		BEEP_ON;
		while(KEY5_PIN==KEY_DOWN);
		return KEY_5;
		// delay_ms(500);
		// BEEP_OFF;

		// if(KEY5_PIN == KEY_DOWN){
		// 	return KEY_5;
		// }

	}
	return KEY_NULL;
	
}
