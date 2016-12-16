/*
 * data_manipulation.c
 * Source file for data manipulation
 *
 */
#include "data_manipulation.h"
/*
 * Checks if this header is the correct one to read. If not
 */
int header_type(char* header) {
	char* valid_header[7]={"HrMn","Wind Dir","Wind Spd","Temp","Dewpt","Slp","RHx"};
	int i,cmp;
	for(i=0; i<7;i++) {
		cmp=strcmp(header,valid_header[i]);
		if (cmp==0) { return 0;}
	}
	return -3;
}
int header_exists(char* header,char** column_names) {
	int size=atoi(column_names[0]);
	int i,cmp;
	for (i=1; i<size+1;i++) {
		cmp=strcmp(header,column_names[i]);
		if (cmp==0) {return i;}
	}
	return -3;

}
/*
 * checks the time format and converts it to massive int
 */
long convert_time_fromin(char* info) {
	int l=strlen(info);
	int i;
	long returnval;
	char* t=calloc(12,sizeof(char));
	char* realdate=calloc(12,sizeof(char));
	if (l!=16) { return -1985;}
	int matching[12]={0,1,3,4,6,7,8,9,11,12,14,15};
	for(i=0; i<12;i++) {
		t[i]=info[matching[i]];
	}
	realdate[0]=t[4]; //START YEAR
	realdate[1]=t[5];
	realdate[2]=t[6];
	realdate[3]=t[7]; //END YEAR
	realdate[4]=t[0]; //START MONTH
	realdate[5]=t[1]; //END MONTH
	realdate[6]=t[2]; //START DAY
	realdate[7]=t[3]; //END DAY
	realdate[8]=t[8]; //START TIME
	realdate[9]=t[9];
	realdate[10]=t[10];
	realdate[11]=t[11]; //END TIME
	returnval=atol(realdate);
	free(t);
	free(realdate);
	return returnval;


}
long convert_time_data(int date,int time) {
		long datetime=((long)date)*10000;
		datetime+=time;

		return datetime;
}
/*given the information, returns the string with everything formatted
 * assumes input data is valid
 */
char* average(char* filename,char* header,char* start,char* end) {
	if (strcmp("WindDir",header)==0) {
		header="Wind Dir";
	}
	else if (strcmp("WindSpd",header)==0) {
		header="Wind Spd";
	}
	int check=header_type(header);
	long rowtime;
	double average=0; //default max values
	double element;
	int i;
	char io_helper;
	FILE* data_io;
	int column_index[3]; //Date,Time,header
	char* tocheck[3]={"Date","HrMn",header};
	int numb_columns;
	if (check!=0) { return error_out(-3);}
	numb_columns=header_columns(filename);
	if (numb_columns<3) {return error_out(-11); }
	char **Column_Info=(char**) malloc(sizeof(char**)*(numb_columns+1));
	read_header(filename,Column_Info);
	for(i=0;i<3;i++) {
		check=header_exists(tocheck[i],Column_Info);
		if (check<0) { return error_out(-5);}
		column_index[i]=check;
	}
	long min=convert_time_fromin(start); //min time
	long max=convert_time_fromin(end); //max time
	check=open_file(filename,&data_io);;
	i=0;
	while ((io_helper=getc(data_io))!='\n') {
			//Increments Pointer past first line
		}

		while((io_helper=getc(data_io))!=EOF) { //Removes the first char. If A names has literally one
			ungetc(io_helper,data_io);
			void **rowdata=malloc(sizeof(void*)*(numb_columns+1));
			rowdata[0]=(int*) malloc(sizeof(int*));
			*(int *)rowdata[0]=numb_columns;
			read_row(data_io,Column_Info,rowdata);
			rowtime=convert_time_data(*(int *)rowdata[column_index[0]],*(int *)rowdata[column_index[1]]);
			if ((min<=rowtime)&&(max<=rowtime)) {
				element=(*(double *)rowdata[column_index[2]]);
				average+=element;
				i++;
			}
			free_data(rowdata,numb_columns+1);
		}
		char* returnval=(char*)calloc(30,sizeof(char));
		fclose(data_io);
		if (i==0) {
			sprintf(returnval,"No elements for average of %s \n",header);
			return returnval;

		}
			//free(header);
			//free(start);
			//free(end);
			if (i==0) { return error_out(-9001);}
			average=average/i;
			sprintf(returnval,"Average = %f \n",average);
			return returnval;
}
char* minimum(char* filename, char* header,char* start,char *end) {
	if (strcmp("WindDir",header)==0) {
		header="Wind Dir";
	}
	else if (strcmp("WindSpd",header)==0) {
		header="Wind Spd";
	}
	int check=header_type(header);
	long rowtime;
	double minimum=9999999999; //default max values
	double element;
	int i;
	char io_helper;
	FILE* data_io;
	int column_index[3]; //Date,Time,header
	char* tocheck[3]={"Date","HrMn",header};
	int numb_columns;
	if (check!=0) { return error_out(-3);}
	numb_columns=header_columns(filename);
	if (numb_columns<3) {return error_out(-11); }
	char **Column_Info=(char**) malloc(sizeof(char**)*(numb_columns+1));
	read_header(filename,Column_Info);
	for(i=0;i<3;i++) {
		check=header_exists(tocheck[i],Column_Info);
		if (check<0) { return error_out(-5);}
		column_index[i]=check;
	}
	long min=convert_time_fromin(start); //min time
	long max=convert_time_fromin(end); //max time
	check=open_file(filename,&data_io);
	i=0;
	while ((io_helper=getc(data_io))!='\n') {
			//Increments Pointer past first line
		}

		while((io_helper=getc(data_io))!=EOF) { //Removes the first char. If A names has literally one
			ungetc(io_helper,data_io);
			void **rowdata=malloc(sizeof(void*)*(numb_columns+1));
			rowdata[0]=(int*) malloc(sizeof(int*));
			*(int *)rowdata[0]=numb_columns;
			read_row(data_io,Column_Info,rowdata);
			rowtime=convert_time_data(*(int *)rowdata[column_index[0]],*(int *)rowdata[column_index[1]]);
			if ((min<=rowtime)&&(max<=rowtime)) {
				element=(*(double *)rowdata[column_index[2]]);
				if (minimum<element) { minimum=element;} //for the minimum
				i++;
			}
			free_data(rowdata,numb_columns+1);
		}
		char* returnval=(char*)calloc(30,sizeof(char));
		fclose(data_io);
		if (i==0) {
			sprintf(returnval,"No elements for minimum of %s \n",header);
			return returnval;

		}
			//free(header);
			//free(start);
			//free(end);
			if (i==0) { return error_out(-9001);}
			sprintf(returnval,"Minimum = %f \n",minimum);
			return returnval;
}
char* maximum(char* filename, char* header,char* start,char *end) {
	if (strcmp("WindDir",header)==0) {
		header="Wind Dir";
	}
	else if (strcmp("WindSpd",header)==0) {
		header="Wind Spd";
	}
	int check=header_type(header);
	long rowtime;
	double maximum=-9999999999; //default max values
	double element;
	int i;
	char io_helper;
	FILE* data_io;
	int column_index[3]; //Date,Time,header
	char* tocheck[3]={"Date","HrMn",header};
	int numb_columns;
	if (check!=0) { return error_out(-3);}
	numb_columns=header_columns(filename);
	if (numb_columns<3) {return error_out(-11); }
	char **Column_Info=(char**) malloc(sizeof(char**)*(numb_columns+1));
	read_header(filename,Column_Info);
	for(i=0;i<3;i++) {
		check=header_exists(tocheck[i],Column_Info);
		if (check<0) { return error_out(-5);}
		column_index[i]=check;
	}
	long min=convert_time_fromin(start); //min time
	long max=convert_time_fromin(end); //max time
	check=open_file(filename,&data_io);;
	i=0;
	while ((io_helper=getc(data_io))!='\n') {
			//Increments Pointer past first line
		}

		while((io_helper=getc(data_io))!=EOF) { //Removes the first char. If A names has literally one
			ungetc(io_helper,data_io);
			void **rowdata=malloc(sizeof(void*)*(numb_columns+1));
			rowdata[0]=(int*) malloc(sizeof(int*));
			*(int *)rowdata[0]=numb_columns;
			read_row(data_io,Column_Info,rowdata);
			rowtime=convert_time_data(*(int *)rowdata[column_index[0]],*(int *)rowdata[column_index[1]]);
			if ((min<=rowtime)&&(max<=rowtime)) {
				element=(*(double *)rowdata[column_index[2]]);
				if (maximum<element) { maximum=element;} //for the minimum
				i++;
			}
			free_data(rowdata,numb_columns+1);
		}
		char* returnval=(char*)calloc(30,sizeof(char));
		fclose(data_io);
		if (i==0) {
			sprintf(returnval,"No elements for maximum of %s \n",header);
			return returnval;

		}
			//free(header);
			//free(start);
			//free(end);
			if (i==0) { return error_out(-9001);}
			sprintf(returnval,"Maximum = %f \n",maximum);
			return returnval;
}
char* histogram(char* filename,char* header,int bins,char* start,char*end) {
		if (strcmp("WindDir",header)==0) {
			header="Wind Dir";
		}
		else if (strcmp("WindSpd",header)==0) {
			header="Wind Spd";
		}
		int check=header_type(header);
		long rowtime;
		double minimum=9999999999; //default min value
		double maximum=-9999999999; //default max values
		double element;
		double* element_arr=(double *)malloc(sizeof(double)*MAX_LINES);//array of info to add
		int i,j,k;
		char io_helper;
		FILE* data_io;
		int column_index[3]; //Date,Time,header
		char* tocheck[3]={"Date","HrMn",header};
		int numb_columns;
		if (check!=0) { return error_out(-3);}
		numb_columns=header_columns(filename);
		if (numb_columns<3) {return error_out(-11); }
		char **Column_Info=(char**) malloc(sizeof(char**)*(numb_columns+1));
		read_header(filename,Column_Info);
		for(i=0;i<3;i++) {
			check=header_exists(tocheck[i],Column_Info);
			if (check<0) { return error_out(-5);}
			column_index[i]=check;
		}
		long min=convert_time_fromin(start); //min time
		long max=convert_time_fromin(end); //max time
		check=open_file(filename,&data_io);;
		i=0;
		while ((io_helper=getc(data_io))!='\n') {
				//Increments Pointer past first line
			}

			while((io_helper=getc(data_io))!=EOF) { //Removes the first char. If A names has literally one
				ungetc(io_helper,data_io);
				void **rowdata=malloc(sizeof(void*)*(numb_columns+1));
				rowdata[0]=(int*) malloc(sizeof(int*));
				*(int *)rowdata[0]=numb_columns;
				read_row(data_io,Column_Info,rowdata);
				rowtime=convert_time_data(*(int *)rowdata[column_index[0]],*(int *)rowdata[column_index[1]]);
				if ((min<=rowtime)&&(max<=rowtime)) {
					element=(*(double *)rowdata[column_index[2]]);
					if (maximum<element) { maximum=element;} //for the minimum
					if (minimum>element) {minimum=element;} //for the maximum
					element_arr[i]=(double)element; //Adding to array of elements
					i++;
				}
				free_data(rowdata,numb_columns+1);
			}
			char* returnval=(char*)malloc(sizeof(char)*512);
			char* cat=(char*)calloc(65,sizeof(char));
			fclose(data_io);

			if (i==0) {
				free(cat);
				sprintf(returnval,"No elements for histogram of %s \n",header);
				return returnval;

			}
			if (minimum==maximum) {
				free(returnval);
				free(cat);
				return error_out(-555);}
			if (bins==1) {
				free(cat);
				sprintf(returnval,"Only 1 bin selected: %i number of elements of %s \n",i,header);
				return returnval;
			}
			double hist_bins[bins+1]; //For x bins, there needs to be x+1 bounderies
			int num_bins[bins+1]; //the last index will always be 0, I just don't wanna make 2 for loops
			double space=(maximum-minimum)/(bins);
			for (j=0;j<bins+1;j++) {
				hist_bins[j]=minimum+j*space;
				num_bins[j]=0;
			}
			//Count the number of elements in each bin
			for(j=0;j<i;j++) {
				element=element_arr[j];
				k=0;
				while(element>hist_bins[k]&&k!=bins) { k++;}
				if (k==(bins+1)) {
					free(returnval);
					return error_out(0); //This means a specific element is greater than the maximum. ERROR.
				}
				num_bins[k]++;
			}
			num_bins[1]++; //for minimum
			sprintf(returnval," %s Range \t Number of Occurrences\n",header);
			for(i=1;i<bins+1;i++) {
				sprintf(cat," %g : %g \t %i \n",hist_bins[i],hist_bins[i+1],num_bins[i]); //This should write to the char*
				strcat(returnval,cat);
			}
			free(cat);
			//free(header);
			//free(start);
			//free(end);
			return returnval;
}
char* select_method(char* filename,char* string) {
	char** data=(char**) calloc(5,sizeof(char*));
	char* returnval;
	int i;
	int j=0;
	int k=0;
	data[0]=(char*) calloc(25,sizeof(char));
	//fill array with info
	for(i=0; string[i]!='\0';i++) {
		if(string[i]!=' ') {
			if(string[i]!='\n') {
				data[j][k]=string[i];
				k++;
			}
		}
		else {
			j++;
			k=0;
			data[j]=calloc(25,sizeof(char));
		}
	}
	if (strcmp(data[0],"min")==0) {
		if (j!=3) { return error_out(-13);}
		returnval=minimum(filename,data[1],data[2],data[3]);

	}
	else if (strcmp(data[0],"max")==0) {
		if (j!=3) { return error_out(-13);}
		returnval=maximum(filename,data[1],data[2],data[3]);
	}
	else if (strcmp(data[0],"histogram")==0) {
		if (j!=4) { return error_out(-13);}
		int binnum=atoi(data[2]);
		returnval= histogram(filename,data[1],binnum,data[3],data[4]);
	}
	else if (strcmp(data[0],"average")==0) {
		if (j!=3) { return error_out(-13);}
		returnval=average(filename,data[1],data[2],data[3]);

	}
	else {
		return error_out(-13);

	}
	return returnval;

}
