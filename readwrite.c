/*
 * readwrite.c
 *
 *      Author: michael
 */

#include "readwrite.h"
/*
 * Returns a specific error message depending on the type of file
 */
char* error_out(int t) {
	char* error_message=(char*)calloc(65,sizeof(char));
	switch(t) {
		case 1:
			sprintf(error_message,"Error Writing to File. Your guess is as good as mine\n");
			break;
		case -3:
			sprintf(error_message,"Type being read in Incorrect. Operations can only be performed on Ints or floats\n");
			break;
		case -5:
			sprintf(error_message,"An essential header (either date,time,or header specified,was not found in the data file)\n");
			break;
		case -9001:
			sprintf(error_message,"There was no element that met search criteria\n");
			break;
		case -1985:
			sprintf(error_message,"Time in input incorrectly formatted\n");
			break;
		case -11:
			sprintf(error_message,"Number of Columns Less than 3 (minimum required for method to work)\n");
			break;
		case -555:
			sprintf(error_message,"All Elements in Data are the same\n");
			break;
		case -17:
			sprintf(error_message,"Input Data Line of Incorrect Format\n");
			break;
		default:
			sprintf(error_message,"Something went wrong\n");
			break;
	}
	return error_message;
}
/*
 * Writes a line to the file
 */
int writeline(char* towrite,FILE* stream) {
	int returnval=fputs(towrite,stream);
	free(towrite);
	return returnval;
}
/*
 * Reads a line from the file
 */
char* readline(FILE* stream) {
	char* info=malloc(sizeof(char)*256);
	fscanf(stream,"%s",info);
	return info;
}
char** parse_info(int argc,char** argv) {
	if(1>argc||argc>4) { return argv;}
	char** from_console=(char**) malloc(sizeof(char*)*3);

	char temp[3];
	char arguments[3][3]={"df=","if=","of="};
	int i,j,l;
	for(i=0;i<3;i++) {
		from_console[i]=(char*)malloc(sizeof(char)*30); //allocates memory
	}
	int csvfile=0;
	int infile=0;
	int outfile=0;
	for(i=1;i<argc;i++) { //For each argument
		temp[0]=argv[i][0];
		temp[1]=argv[i][1]; //Yes this can also crash if you put in a command line argument of length <3
		temp[2]=argv[i][2];
		for(j=0; j<3;j++) { //for each file type
			l=(temp[0]==arguments[j][0])+(temp[1]==arguments[j][1])+(temp[2]==arguments[j][2]);
			if (l==3) {
				switch(j) {
					case 0:
						csvfile=1;
						break;
					case 1:
						infile=1;
						break;
					case 2:
						outfile=1;
						break;
				}
				strcpy(from_console[j],argv[i]+3);
			}
		}
	}
	if (infile==0) {
		from_console[1]="STDIN"; //If you use a file called STDIN for input you're gonna have a bad time. You're also dicks
	}
	if (outfile==0) {
		from_console[2]="STDOUT"; //You said that you wouldn't be a jerk about the tests in class, so i'm trusting you on that
	}
	if (csvfile!=0) {
		return from_console;
	}
	else {
		printf("Please Enter a data file name\n");
		scanf("%s",from_console[0]);
		return from_console;
	}
}
int main_process(char** info) {
	FILE* in;
	FILE* out;
	char *line,*towrite;
	int f;
	//Setting File pointers to correct values
	if (strcmp(info[2],"STDOUT")==0) {
		out=stdout;
	}
	else {
		out=fopen(info[2],"w");
	}
	if(strcmp(info[1],"STDIN")==0) {
		in=stdin;
	}
	else {
		in=fopen(info[1],"r");
		if (out==0) {
			fprintf(out,"Error Opening input File\n");
			return -1;
		}

	}
	//Data File
	f=validate_format(info[0]);
	if (f!=0) {
		fprintf(out,"Error Opening Data File\n");
		return -1;

	}

	line=(char*)calloc(65,sizeof(char));
	towrite=(char*)calloc(512,sizeof(char));
	fgets(line,60,in);
	int i=0;
	int compq=strcmp("quit",line);
	int compb=strcmp("",line);
	while((compq!=0)&&compb!=0) { //So if you type 'q' || QUIT || Quit tough luck
		towrite=select_method(info[0],line);
		fprintf(out,"%s",towrite);
		free(line);
		free(towrite);
		line=(char*)calloc(65,sizeof(char));
		towrite=(char*)calloc(65,sizeof(char));
		fgets(line,65,in);
		compq=strcmp("quit",line);
		compb=strcmp("",line);
		i++;

	}
	fclose(in);
	fclose(out);
	return 0;
}
