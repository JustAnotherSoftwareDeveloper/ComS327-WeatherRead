/*
 * data_manipulation.h

 *      Author: michael
 */

#define NOELEMENT_ERROR -9001
#define TIMEFORMAT_ERROR -1985
#define ALLSAME_ELEMENT -555
#define METHODINPUT_ERROR -13
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
#ifndef READWRITE_H_
#include "readwrite.h"
#define READ_WRITE_H_
#endif
#ifndef DATA_MANIPULATION_H_
#define DATA_MANIPULATION_H_



char* histogram(char* filename,char* header,int bins,char* start,char*end);
char* minimum(char* filename, char* header,char* start,char *end);
char* maximum(char* filename, char* header,char* start,char *end);
char* average(char* filename, char* header,char* start,char *end);
int header_type(char* header);
int header_exists(char* header,char** column_headers);
enum METHOD { HISTOGRAM,MINIMUM,MAXIMUM,AVERAGE
};
long convert_time_fromin(char *info);
long convert_time_data(int date,int time);
char* select_method(char* filename,char* string);


#endif /* DATA_MANIPULATION_H_ */
