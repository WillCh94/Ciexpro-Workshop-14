/*
 * principal.c
 *
 *  Created on: 15 dic. 2020
 *      Author: William
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "nvs_flash.h"


#include "WebHTTPCalls.h"
#include "WIFIconfig.h"
#include "UARTconfig.h"
#include "ReadHTML.h"
#include "UserValidation.h"



xQueueHandle ColaUART;
#define Pila 1024


void app_main(void){

	nvs_flash_init();
	iniciarUART();
	//iniciarWiFi();

	xTaskCreatePinnedToCore( &ValidacionDeUsuario, "Tarea_para_UART0", Pila * 2, NULL, 4, NULL, 1 );
	xTaskCreatePinnedToCore( &ConectarServidorHTTP, "tarea_serverHTTP", Pila * 2, NULL, 4, NULL, 0 );


}






