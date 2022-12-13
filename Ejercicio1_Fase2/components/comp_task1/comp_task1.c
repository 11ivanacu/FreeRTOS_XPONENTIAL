#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/adc.h"
#include "freertos/queue.h"

#include "comp_task1.h"

const char *tagBlink = "tagBlink";
#define led 32

void vTask1(void *pvParameters)
{

	gpio_reset_pin(led);
	gpio_set_direction(led, GPIO_MODE_OUTPUT);
	QueueHandle_t localQueue = *(QueueSetHandle_t*) pvParameters;
	int receivedValue = 0;

	while(1){
		if (uxQueueMessagesWaiting(localQueue)!=0){
			if (!xQueueReceive(localQueue, &receivedValue, pdMS_TO_TICKS(100))) {
				ESP_LOGE(tagBlink, "Error receiving value from queue");
			}
			else{
						ESP_LOGI(tagBlink, "Value received %i from queue", receivedValue);
				}
		}

		gpio_set_level(led, 1);
		ESP_LOGI(tagBlink,"Estado del led: %d", gpio_get_level(led));
		vTaskDelay(pdMS_TO_TICKS(500));

		gpio_set_level(led, 0);
		ESP_LOGI(tagBlink,"Estado del led: %d", gpio_get_level(led));
		vTaskDelay(pdMS_TO_TICKS(500));

	}
}
