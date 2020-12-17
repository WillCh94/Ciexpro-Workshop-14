/*
 * UARTconfig.c
 *
 *  Created on: 16 dic. 2020
 *      Author: William
 */



#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include <string.h>


#define BufferSize 1024
#define Pila 1024


extern xQueueHandle ColaUART;
static QueueHandle_t UART0queue;



static void TareaEventosUART(void *Parametro){

	uart_event_t evento;
	uint8_t* datoRX = (uint8_t*) malloc( BufferSize );

	for(;;){

		if ( xQueueReceive( UART0queue, (void*) &evento, ( portTickType ) portMAX_DELAY ) ){

			if( evento.type == UART_DATA ){

				uart_read_bytes( UART_NUM_0, datoRX, evento.size, portMAX_DELAY );
				xQueueSend( ColaUART, datoRX, 0 / portTICK_RATE_MS );

			}

		}

	}

	free( datoRX );
	datoRX = NULL;
	vTaskDelete( NULL );

}


void iniciarUART(){

	uart_config_t configUART0 = {

		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,

	};

	uart_param_config(UART_NUM_0, &configUART0);
	uart_set_pin(UART_NUM_0, (-1), (-1), (-1), (-1));
	uart_driver_install(UART_NUM_0, BufferSize * 2, 0, 20, &UART0queue, 0);

	xTaskCreatePinnedToCore(TareaEventosUART, "Tarea_para_UART0", Pila * 5, NULL, 4, NULL, 1);

}







