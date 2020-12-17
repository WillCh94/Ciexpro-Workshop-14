/*
 * UserValidation.c
 *
 *  Created on: 16 dic. 2020
 *      Author: William
 */



#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/queue.h"
#include <string.h>


#include "WIFIconfig.h"


#define	UsuarioValido		"UserValido"
#define ClaveValida			"ClaveValida"
#define ValidUserNickName	"Usuario Valido 1"
#define MaxCharAdm			256
#define LogOut				0
#define LogIn				1


int8_t logStatus = LogOut;
extern xQueueHandle ColaUART;



static void WordCapturer( char *Variable ){

	char Rx[1];
	char aux[ MaxCharAdm ];
	uint8_t	contador = 0;

	char Key = 0x0D;
	//char Key = {'\n'};
	//char Key = '\n';

	while ( Rx[0] != Key ){	//ENTER (0X0D) SE TRADUCE AL CARACTER RETORNO DE CARRO

		//ENTER (0X0D) SE TRADUCE AL CARACTER RETORNO DE CARRO
		if ( xQueueReceive( ColaUART, &Rx, 1000 / portTICK_RATE_MS ) == pdTRUE || Rx[0] != Key ){

			aux[ contador ] = Rx[0];

			if ( contador == 255 ) contador--;
			contador++;

			vTaskDelay( 0 / portTICK_RATE_MS );

		}

	}

	Variable = (char * ) malloc ( contador );

	for( uint8_t i = 0; i < contador ; i++ ) Variable[ i ] = aux [ i ];

}


static void UserValidator( char NombreUser,  char ClaveUser,  char *UsuarioNickName  ){

	if ( strncmp( NombreUser, UsuarioValido, strlen( NombreUser ) ) == 0 &&
			strncmp( ClaveUser, ClaveValida, strlen( ClaveUser ) ) == 0 ){

		UsuarioNickName = ( char * ) malloc ( sizeof( ValidUserNickName ) );
		for( int8_t i = 0; i < sizeof( ValidUserNickName ) ; i++ ) UsuarioNickName[ i ] = ValidUserNickName[ i ];
		logStatus = LogIn;

		char *mensj = "Bienvenido Usuario ";
		char *mensj2 = (char * ) malloc( strlen( mensj ) + strlen( UsuarioNickName ) + 1);
		mensj2 = strcpy( mensj2, mensj );
		mensj2 = strcat( mensj2, UsuarioNickName );
		uart_write_bytes( UART_NUM_0, (char*) mensj2, strlen( mensj2 ) );
		free( mensj2 );

	} else {

		char mensj[] = "Usuario o Clave errados. Vuelva a intentarlo";
		uart_write_bytes( UART_NUM_0, (char*) mensj, strlen( mensj ) );

	}

}



void ValidacionDeUsuario(void *P){

	char *UserName;
	char *PassWord;
	char *NickName;

	for(;;){

		if (logStatus == LogIn) break;

		WordCapturer( &UserName );
		WordCapturer( &PassWord );
		UserValidator( *UserName,  *PassWord,  &NickName  );

		vTaskDelay( 50000 / portTICK_PERIOD_MS );

	}


	//free( UserName );
	//free( PassWord );
	vTaskDelete( NULL );

	iniciarWiFi();


}


