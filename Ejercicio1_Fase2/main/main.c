#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/adc.h"
#include "freertos/queue.h"

#include "comp_task1.h"
#include "comp_task2.h"
#include "comp_task3.h"

const char *tag="Main";

int memory_task=1024;
QueueHandle_t GlobalQueue=0;		//Declara una cola única


esp_err_t create_task(void);

void app_main(void)
{
	 create_task();
	 GlobalQueue = xQueueCreate(10,sizeof(int));

}


esp_err_t create_task(void){
	struct controlador{
			TaskHandle_t ct1;
			TaskHandle_t ct2;
		};
	struct controlador *control =malloc(sizeof(*control));
	control->ct1=NULL;
	control->ct2=NULL;

	xTaskCreatePinnedToCore(
			vTask1,
			"Task1_Blink",
			memory_task*2,
			&GlobalQueue,
			1,
			&control->ct1,
			1);
	xTaskCreatePinnedToCore(
			vTask2,
			"Task2_ADC",
			memory_task*2,
			&GlobalQueue,
			1,
			&control->ct2,
			1);

	xTaskCreatePinnedToCore(
			vTask3,
			"button",
			memory_task*3,
			(void *) control,
			1,
			NULL,
			1);


	return ESP_OK;
}

