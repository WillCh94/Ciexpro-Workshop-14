/*
 * webHTTP.c
 *
 *  Created on: 16 dic. 2020
 *      Author: William
 */



#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/api.h"
#include "driver/gpio.h"
#include <string.h>


#include "ReadHTML.h"


#define PUERTO	80
#define	LEDB	21
#define LEDG	22
#define LEDR	23


//Web a la que conectará el dispositivo con el ESP32

extern char *FileBuffer;

//FileBuffer = (char *) malloc ( sizeof( 12 ) );

const char FileName[] = "WebWorkshop14.http.txt";


//---------------------------------------------------------------------------



static void configuracionGPIO(){

	gpio_set_direction(LEDB, GPIO_MODE_INPUT_OUTPUT);
	gpio_set_direction(LEDG, GPIO_MODE_INPUT_OUTPUT);
	gpio_set_direction(LEDR, GPIO_MODE_INPUT_OUTPUT);

}



static void WEBLocal (struct netconn *conexion){

	struct netbuf *bufferEntrada;
	char *buffer;

	//char WebHTML = readHTHL( char* FileName );

	u16_t long_buffer;
	err_t err;
	err = netconn_recv( conexion, &bufferEntrada );

	if ( err == ERR_OK ){

		printf("---- Paquete Recibido ----\n\r");
		netbuf_data ( bufferEntrada, ( void** ) &buffer, &long_buffer );

		if ( strncmp( buffer, "GET /", 5 ) == 0 ){

			for (int i = 0; buffer[i] != 0; i++ ) printf ( "%c", buffer[i] );
			printf("\n");

			if ( strncmp ( buffer, "GET /LEDG", 9 ) == 0 ){

				gpio_set_level( LEDG, !gpio_get_level( LEDG ) );
				printf(" led Verde ");

			}
			if ( strncmp ( buffer, "GET /LEDR", 9 ) == 0 ){

				gpio_set_level( LEDR, !gpio_get_level( LEDR ) );
				printf(" led rojo ");

			}
			if ( strncmp ( buffer, "GET /LEDB", 9 ) == 0 ){

				gpio_set_level( LEDB, !gpio_get_level( LEDB ) );
				printf(" led azul ");

			}
			if ( strncmp ( buffer, "GET /LEDsOff", 12 ) == 0 ){

				gpio_set_level( LEDB, 0 );
				gpio_set_level( LEDR, 0 );
				gpio_set_level( LEDG, 0 );
				printf(" leds apagados ");

			}
			if ( strncmp ( buffer, "GET /LEDsUp", 11 ) == 0 ){

				gpio_set_level( LEDB, 1 );
				gpio_set_level( LEDR, 1 );
				gpio_set_level( LEDG, 1 );
				printf(" leds encendidos ");

			}

			//carga del código HTML de la web con la que se puede comunicar con el ESP32

			netconn_write( conexion, FileBuffer, sizeof( FileBuffer ) - 1, NETCONN_NOCOPY );

		}

	}

	netconn_close(conexion);
	netbuf_delete(bufferEntrada);
}



void ConectarServidorHTTP(void *P){

	configuracionGPIO();
	ReadFile( &FileName );


	struct netconn *conectar, *NuevaCon;
	err_t err;
	conectar = netconn_new( NETCONN_TCP );
	netconn_bind( conectar, NULL, PUERTO );		//se asignó el puerto 80
	netconn_listen( conectar );

	do {

		err = netconn_accept( conectar, &NuevaCon );

		if ( err == ERR_OK) {

			WEBLocal ( NuevaCon );
			netconn_delete( NuevaCon );

		}

	} while ( err == ERR_OK );

	netconn_close( conectar );
	netconn_delete( conectar );

}


