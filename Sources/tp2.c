/*
 * tp2.c
 *
 *  Created on: 26 oct. 2016
 *      Author: Julien
 */

#include "tp2.h"
#include "Menu.h"
#include "My_terminal.h"
#include "1Wire/onewire.h"
#include "DS18B20Driver.h"
#include "gpio1.h"

void read_sensor_temperature()
{
	/*OneWire_Reset();
	uint8_t rom[8] = {0,0,0,0,0,0,0,0};
	OneWire_Write(DS18B20_CMD_READ_ROM);
	for(int i = 0 ; i < 8 ; i++)
		OneWire_Read(&rom[i]);

	for(int i = 0 ; i < 8 ; i++)
		PRINTF("%0x ", rom[i]);

	PRINTF("\n\r");*/

	float temp = DS18B20_readTemperature();

	PRINTF("temp : %f\n\r", temp);

	/*uint16_t temp = DS18B20_readTemperature();
	PRINTF("temp : %u\n\r", temp);*/
}

void set_gpio_off()
{
	GPIO_DRV_WritePinOutput(relais, 0);
	PRINTF("relais off\n\r");
}

void set_gpio_on()
{
	GPIO_DRV_WritePinOutput(relais, 1);
	PRINTF("relais on\n\r");
}

void tp2_run()
{
	OneWire_Init();

	Menu_init("TP2");
	//Menu_add_item("read temp", read_sensor_temperature);
	Menu_add_item("relais off", set_gpio_off);
	Menu_add_item("relais on", set_gpio_on);
	Menu_print();

	for(;;)
	{
		char userInput = 0;
		Terminal_ReadChar(&userInput);
		Menu_choose(userInput-'0');
	}

}


