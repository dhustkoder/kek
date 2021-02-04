#include "pal.h"
#include <string.h>
#include <stdio.h>

enum pal_return pal_file_to_buffer(const char *file, uint8_t *buffer, size_t *size_bytes, int capacity)
{
	int filesize = 0;
	int copysize = 0;
	ssize_t bytesread = 0;

	if(size_bytes)
		*size_bytes = 0;

    //todo: its possible the file could change while reading
    // this function is not designed to be used for streaming 
	FILE *fptr = fopen(file, "rb");

	if(!fptr)
		return PAL_ERROR;

	fseek(fptr, 0L, SEEK_END);
	
	filesize = ftell(fptr);
		
	if(filesize > capacity) 
		copysize = capacity;
	else
		copysize = filesize;
	
	fseek(fptr, 0L, SEEK_SET);
	bytesread = fread(buffer, sizeof(char), copysize, fptr);
	fclose(fptr);

	if((int)bytesread != copysize)
		return PAL_ERROR;

	if(filesize > capacity)
		return PAL_ERROR;

	if(size_bytes)
		*size_bytes = bytesread;

    return PAL_OK;
}

enum pal_return pal_file_get_size(const char *file, size_t *size)
{
	FILE *fptr = fopen(file, "rb");

	if(!fptr)
		return PAL_ERROR;

	fseek(fptr, 0L, SEEK_END);
	*size = ftell(fptr);
	
	fclose(fptr);

    return PAL_OK;
}
