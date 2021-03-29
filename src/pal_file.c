#include "pal.h"
#include <string.h>
#include <stdio.h>

PALReturn pal_file_to_buffer(const char *file, uint8_t *buffer, size_t *size_bytes, size_t capacity)
{
	size_t filesize = 0;
	size_t copysize = 0;
	size_t bytesread = 0;

	if(size_bytes)
		*size_bytes = 0;

    //todo: its possible the file could change while reading
    // this function is not designed to be used for streaming 
	FILE *fptr = fopen(file, "rb");

	if(!fptr)
		return PAL_ERROR;

	fseek(fptr, 0L, SEEK_END);
	
    int offset = ftell(fptr);

    if(offset < 0)
    {
	    fclose(fptr);
        return PAL_ERROR;
    }

	filesize = (size_t)offset;

		
	if(filesize > capacity) 
		copysize = capacity;
	else
		copysize = filesize;
	
	fseek(fptr, 0L, SEEK_SET);
	bytesread = fread(buffer, sizeof(char), copysize, fptr);
	fclose(fptr);

	if(bytesread != copysize)
		return PAL_ERROR;

	if(filesize > capacity)
		return PAL_ERROR;

	if(size_bytes)
		*size_bytes = bytesread;

    return PAL_OK;
}

PALReturn pal_file_get_size(const char *file, size_t *size)
{
	FILE *fptr = fopen(file, "rb");

	if(!fptr)
		return PAL_ERROR;

	fseek(fptr, 0L, SEEK_END);
	*size = ftell(fptr);
	
	fclose(fptr);

    return PAL_OK;
}
