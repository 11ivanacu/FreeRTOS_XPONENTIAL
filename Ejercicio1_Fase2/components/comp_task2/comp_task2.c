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

	while(1){
			adc_val = adc1_get_raw(ADC1_CHANNEL_4);
			ESP_LOGI(tagADC,"ADC VAL: %i",adc_val);
			vTaskDelay(pdMS_TO_TICKS(1000));
		}
}
