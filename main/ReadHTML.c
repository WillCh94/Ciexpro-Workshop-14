/*
 * ReadHTML.c
 *
 *  Created on: 16 dic. 2020
 *      Author: William
 */



#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>


//char *FileTXT[];
char *FileBuffer;
uint16_t n;



int GetFileSize( char *FileDirection ){

	FILE *ptr_file;
	int16_t N;

	ptr_file = fopen( FileDirection, "r" );


	if (!ptr_file) N = 0;
	else N = sizeof( ptr_file );

	fclose(ptr_file);

	return N;

}



void ReadFile( char *FileDirection ){

	FILE *ptr_file;

	//char FileBuffer[];

	ptr_file = fopen(  FileDirection , "r" );

	n = GetFileSize( FileDirection );

	FileBuffer = ( char * ) malloc( n );

	if (!ptr_file || n == 0 ) printf("error: No se pudo leer el Archivo o éste está vacío");
	else while( fgets(FileBuffer, n, ptr_file ) != NULL ) printf("%s", FileBuffer);

	fclose(ptr_file);

}




