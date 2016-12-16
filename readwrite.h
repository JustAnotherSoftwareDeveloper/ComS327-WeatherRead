/*
 * readwrite.h

 *      Author: michael
 */
#define INCORRECT_TYPE -3
#define HEADER_NOTFOUND -5
#ifndef STRING_H_
#include <string.h>
#define STRING_H_
#endif
#ifndef STDIO_H_
#include <stdio.h>
#define STDIO_H_
#endif
#ifndef STDLIB_H_
#include <stdlib.h>
#define STDLIB_H_
#endif
#ifndef WEATHERIO_H_
#include "weatherio.h"
#define WEATHERIO_H_
#endif
#ifndef DATA_MANIPULATION_H_
#define DATA_MANIPULATION_H_
#include "data_manipulation.h"
#endif
#ifndef READWRITE_H_
#define READWRITE_H_




char* error_out(int t);
int write_line(char* towrite,FILE* stream);
char* read_line(FILE* stream);
int main_process(char** info);
char** parse_input(int argc,char** argv);




#endif /* READWRITE_H_ */
