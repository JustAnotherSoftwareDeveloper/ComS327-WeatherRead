/*
 * Michael Sila
 */
#include "weatherio.h"
#include "readwrite.h"
#include "data_manipulation.h"
#include <string.h>
int main(int argc, char** argv) {
	int testc=4;
	char* testv[4]={(char *)malloc(sizeof(char)*50),(char *)malloc(sizeof(char)*50),(char *)malloc(sizeof(char)*50),(char *)malloc(sizeof(char)*50)};
	testv[0]="t";
	int i=0;
	testv[1]="df=AmesWeatherData.csv";
	testv[2]="if=in0.txt";
	testv[3]="of=out.txt";
	char** returnval=parse_info(testc,testv);
	main_process(returnval);
	return 0;
}

