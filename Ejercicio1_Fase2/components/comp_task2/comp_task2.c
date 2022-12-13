#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/adc.h"
#include "freertos/queue.h"
#include "comp_task2.h"

const char *tagADC = "tagADC";			//sms para proposito de la tarea del adc
int	adc_val=0;

void vTask2(void *pvParameters){
	adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11 );		//PIN 32, ATENUACION EN 3.3
	adc1_config_width(ADC_WIDTH_BIT_12);

	QueueHandle_t localQueue= *(QueueHandle_t*) pvParameters;

	while(1){
			adc_val = adc1_get_raw(ADC1_CHANNEL_6);
			ESP_LOGI(tagADC,"ADC VAL: %i",adc_val);
			if(adc_val>=2500){
			if (!xQueueSend(localQueue, &adc_val, pdMS_TO_TICKS(100))) {
				ESP_LOGE(tagADC, "Error sending %i to queue", adc_val);
				}
			}
			vTaskDelay(pdMS_TO_TICKS(1000));
		}
}
