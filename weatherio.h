/*
 * Class containing function headers and
 *
 *
 *
 */
#define GENERR -1 //Generic Error
#define NUMCOMMA_ERROR -2 /*Different Amount of Elements in Row than expected */
#define DATAERR -3 /* Data Type Different than Expected */
#define FOPEN_ERROR -4/* Generic IO ERROR */
#define FORMAT_ERROR -10 /* FileName format error*/
#define CATEGORYERR -5 /* Header Category Not recognized */
#define COLUMN_ERROR -11 /*Header Columns Read does not match header columns counted */
#define HEADER_FORMAT_ERROR -12
#define OVERFLOW_ERROR -100 /*Max Lines/Length Reached*/
#define MAX_LINES 50000 //Max Amount of Lines in File
#define MAX_LENGTH 256 //Max Length of Each Line
#define HEADER_TYPE_ERROR -6

#ifndef STRING_H_
#define STRING_H_
#include <string.h>
#endif
#ifndef STDIO_H_
#define STDIO_H_
#include <stdio.h>
#endif
#ifndef STDLIB_H_
#include <stdlib.h>
#define STDLIB_H_
#endif


#ifndef WEATHERIO_H_
#define WEATHERIO_H_

int validate_format(char *);
int header_columns(char *);
int read_header(char *,char **);
int read_row(FILE *,char **, void **);
int check_fileformat(char *);
int open_file(char *,FILE**);
enum TYPE { UNKNOWN,STRING,FLOAT,INT,CHAR
};
typedef enum TYPE Type;
Type column_type(const char *column);
int matches_type(const char *column, const char *data);
void free_data(void** data, int size);
char *trimwhitepace(char *);

#endif /* WEATHERIO_H_ */
