#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/queue.h"
#include "comp_task3.h"

#define btn 33

const char *tagInput = "INPUT";
uint8_t count = 0;

void vTask3(void *pvParameters)
{

	struct controlador{
		TaskHandle_t ct1;
		TaskHandle_t ct2;
	};
	struct controlador *control = (struct controlador*) pvParameters;

	gpio_reset_pin(btn);
	gpio_set_direction(btn, GPIO_MODE_INPUT);
	gpio_pullup_en(btn);

	while(1){

			if(gpio_get_level(btn) == 0 ){
				count++;
				vTaskDelay(pdMS_TO_TICKS(500));
			}

			if(count == 3){
				count=0;
			}

			ESP_LOGI(tagInput,"Count: %i",count);
			switch (count) {
				case 0:
					vTaskResume(control -> ct1);
					vTaskResume(control -> ct2);
					ESP_LOGE(tagInput, "Task 1 y 2 ON");
					break;

				case 1:
					vTaskResume(control -> ct1);
					vTaskSuspend(control -> ct2);
					ESP_LOGE(tagInput, "Task 1 ON");
					break;
				case 2:
					vTaskSuspend(control -> ct1);
					vTaskResume(control -> ct2);
					ESP_LOGE(tagInput, "Task 2 ON");
					break;
				default:
					break;
			}

			vTaskDelay(pdMS_TO_TICKS(500));
		}

}




