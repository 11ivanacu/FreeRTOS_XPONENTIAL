#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "driver/adc.h"


#define led 33
#define btn 32
#define STACK_SIZE 1024*2

xSemaphoreHandle Key1=0,Key2=0;

const char *tag="Main";

int	adc_val=0;
uint8_t count = 0;

esp_err_t init_gpios(void);
esp_err_t create_task(void);
esp_err_t set_adc(void);

void Task1( void * pvParameters);
void Task2( void * pvParameters);
void Task3( void * pvParameters);


void app_main(void)
{
	 Key1=xSemaphoreCreateBinary();
	 Key2=xSemaphoreCreateBinary();
	 init_gpios();
	 create_task();
	 set_adc();
}


esp_err_t init_gpios(void)
{
	gpio_reset_pin(led);
	gpio_set_direction(led, GPIO_MODE_OUTPUT);

	gpio_reset_pin(btn);
	gpio_set_direction(btn, GPIO_MODE_INPUT);
	gpio_pullup_en(btn);
	return ESP_OK;
}

esp_err_t set_adc(void){

	adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11 );		//PIN 32, ATENUACION EN 3.3
	adc1_config_width(ADC_WIDTH_BIT_12);
	return ESP_OK;
}

esp_err_t create_task(void){
	static uint8_t ucParameterToPass;
	TaskHandle_t xHandle = NULL;

	xTaskCreate( Task1,
				"Task1",
				STACK_SIZE,
				&ucParameterToPass,
				1,
				&xHandle);

	xTaskCreate( Task2,
				"Task2",
				STACK_SIZE,
				&ucParameterToPass,
				1,
				&xHandle);

	xTaskCreate( Task3,
				"vTaskB",
				STACK_SIZE,
				&ucParameterToPass,
				1,
				&xHandle);


	return ESP_OK;
}


void Task1( void * pvParameters )
{
	while(1){
		if (xSemaphoreTake(Key1, portMAX_DELAY)) {
		adc_val = adc1_get_raw(ADC1_CHANNEL_4);
		ESP_LOGI(tag,"ADC VAL: %i",adc_val);
		vTaskDelay(pdMS_TO_TICKS(1000));
		}
	}
}

void Task2( void * pvParameters )
{
	while(1){
		if (xSemaphoreTake(Key2, portMAX_DELAY)) {
		gpio_set_level(led,1);
		vTaskDelay(pdMS_TO_TICKS(500));

		gpio_set_level(led,0);
		vTaskDelay(pdMS_TO_TICKS(500));
		}
	}
}


void Task3( void * pvParameters )
{
	while(1){

		if(gpio_get_level(btn) == 0 ){
			count++;
			vTaskDelay(pdMS_TO_TICKS(500));
		}

		if(count == 3){
			count=0;
		}

		ESP_LOGI(tag,"Count: %i",count);
		switch (count) {
			case 0:
				ESP_LOGE(tag, "Task 1 is giving the key");
				xSemaphoreGive(Key1);

				ESP_LOGE(tag, "Task 2 is giving the key");
				xSemaphoreGive(Key2);
				break;
			case 1:
				ESP_LOGE(tag, "Task 1 is giving the key");
				xSemaphoreGive(Key1);
				break;
			case 2:
				ESP_LOGE(tag, "Task 2 is giving the key");
				xSemaphoreGive(Key2);
				break;
			default:
				break;
		}

		vTaskDelay(pdMS_TO_TICKS(500));


	}
}

