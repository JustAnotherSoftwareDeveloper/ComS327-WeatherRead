/**
 * Michael Sila
 */
#include "weatherio.h"


int validate_format(char *filename) {
	//Check if file ends in CSV
	FILE* data_io=malloc(sizeof(char)*256*50004);
	char io_helper;
	int line_numb;
	int numb_columns;
	int c,i,h_helper,f_helper; //helper integers
	int csv=check_fileformat(filename);
	if (csv!=0) {
		return csv;
	}
	numb_columns=header_columns(filename);
	if (numb_columns<1) {return -10; }
	char **Column_Info=(char**) malloc(sizeof(char**)*(numb_columns+1));

	c=read_header(filename,Column_Info);
	if (c<0) {return c;}
	f_helper=open_file(filename,&data_io);
	line_numb=0;

	while ((io_helper=getc(data_io))!='\n') {
		//Increments Pointer past first line
	}
	i=1;
	while((io_helper=getc(data_io))!=EOF) { //Removes the first char. If A names has literally one
		ungetc(io_helper,data_io);
		i++;
		if (i>MAX_LINES) { return i;}
		void **rowdata=malloc(sizeof(void*)*(numb_columns+1));
		rowdata[0]=(int*) malloc(sizeof(int*));
		*(int *)rowdata[0]=numb_columns;
		c=read_row(data_io,Column_Info,rowdata);
		if (c!=0) {return i;}
		free_data(rowdata,numb_columns+1);

	}
	fclose(data_io);
	return 0;
}
/*
 * Reads Header File. Calls on header_columns for size. Returns a C-String of the header file
 */
int read_header(char *filename, char **header_names) {
	int numb_columns=header_columns(filename);
	int read_col=1;
	char c;
	FILE* data_io;
	int i=open_file(filename,&data_io); //Opens File again. Doesn't need checking because header_index will take care of that
	header_names[0]= (char *) malloc(sizeof(char)*10);
	sprintf(header_names[0],"%d",numb_columns);
	header_names[1]= (char *) malloc(sizeof(char)*65); //Allocates Memory for the incoming c string
	char *p=header_names[1];
	while ((c=getc(data_io))!='\n') {
		if (c==',') {
			*p='\0';
			read_col++;
			header_names[read_col]=(char*) malloc(sizeof(char*)*numb_columns);
			p=header_names[read_col];

		}
		else if ((c!='\n')) {
			*p++=c;
		}
		else {
			*p='\0'; //terminate c_string
		}
	}
	//free(header_names[header_index]);
	fclose(data_io);
	if ((read_col+1)==numb_columns+1) {
		return numb_columns;
	}
	return COLUMN_ERROR;




}
/*
 * This method counts the number of columns in the header line
 */
int header_columns(char* filename) {
	FILE* data_io; //File to Read
	int num_commas=0; //Counts by commas.
	char c;
	int file_open=open_file(filename,&data_io);
	if (file_open!=0) { return file_open; }
	while ((c=getc(data_io))!='\n') {
		if (c==',') {num_commas++;} //If File is empty then will result in one column. !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
	fclose(data_io);
	return num_commas+1;
}
/**
 * This method Reads a single row and gets the data from it
 */
int read_row(FILE *stream,char **headers, void **data) {
	int i,header_index,m,k;
	Type h_type;
	char data_row[256];
	char j;
	char data_subset[65];
	for(k=0;k<65;k++) {
		data_subset[k]='\0';
	}
	fgets(data_row,sizeof(data_row),stream); //reads file
	int header_size=atoi(headers[0]);
	for (i=1; i<header_size+1; i++) {
		h_type=column_type(headers[i]);
		switch(h_type) {
			case INT:
				data[i]=(int*)malloc(sizeof(int*));
				break;
			case FLOAT:
				data[i]=(float*)malloc(sizeof(float*));
				break;
			case CHAR:
				data[i]=(char*)malloc(sizeof(char*));
				break;
			case STRING:
				data[i]=(char *)malloc(65*sizeof(char*));
				break;
			case UNKNOWN:
				fprintf(stderr,"Header: %s of UNKNOWN TYPE",headers[i]);
				return -1;
			default:
				return -1;
		}
	}
		//ready to read the string
		k=0;
		header_index=1;
		for(i=0; data_row[i]!='\0'; i++){
			j=data_row[i];
			if (j!=',') {
				if(j==' '&&strcmp("IDentification Name",headers[header_index])!=0) { //if there is whitespace, but not in IDentification name
					data_subset[k]='\0';
				}
				else{
					data_subset[k]=j;
				}
				k++;

			}
			else {
				h_type=column_type(headers[header_index]); //Check header type currently being red
				m=matches_type(headers[header_index],data_subset);
				if (m!=0) {
					fprintf(stderr,"Data for header %s is not correct: %s",headers[header_index],data_subset);
					return -1;
				}
				switch(h_type) {
					case INT:
						*(int *)data[header_index]=atoi(data_subset);
						break;
					case FLOAT:
						*(float *)data[header_index]=atof(data_subset);
						break;
					case CHAR:
						strcpy((char *)data[header_index],data_subset);
						break;
					case STRING:
						strcpy((char *)data[header_index],data_subset);
						break;
					default:
						fprintf(stderr,"Header Type Check Failed for %s",headers[header_index]);
						return -1;
				}
				header_index++;
				for(k=0;k<65;k++) {
					data_subset[k]='\0';
				}
				k=0;
			}
		}
		if (data_subset[strlen(data_subset)-1]=='\n') {
			data_subset[strlen(data_subset)-1]='\0'; //removes newline
		}
		if (strlen(data_subset)>0) {
			h_type=column_type(headers[header_index]); //Check header type currently being red
			m=matches_type(headers[header_index],data_subset);
			if (m!=0) { return -1;}
			switch(h_type) {
				case INT:
					*(int *)data[header_index]=atoi(data_subset);
					break;
				case FLOAT:
					*(float *)data[header_index]=atof(data_subset);
					break;
				case CHAR:
					strcpy((char *)data[header_index],data_subset);
					break;
				case STRING:
					strcpy((char *)data[header_index],data_subset);
					break;
				default:
					return -1;
			}
			header_index++;
		}
		else {
			return i;
		}
		if (header_index-1==header_size) { return 0;}
		else { return -1;}
	}
/*
 * This method will perform a simple check to see if the file ends in ".csv".
 */
int check_fileformat(char *filename) {
	//Check if format is *.csv
		int i,j; //for Loops
		char expected,actual;
		char file_end[5]={'.','c','s','v'};
		int string_size;
		string_size=strlen(filename);
		//Check if File Name is Correct
		for(i=string_size-1,j=3; j>=0; i--,j--) {
			expected=filename[i];
			actual=file_end[j];
			if (expected!=actual) { return FORMAT_ERROR; }
		}
		return 0;
}
/*
 * This method will open a file for reading, and return an error if file could not be opened.
 */
int open_file(char *filename, FILE** data_io) {
	*data_io=fopen(filename,"r");
	if (*data_io==0) {
		return FOPEN_ERROR;
	}
	else {
		return 0;
	}
}
/*
 * This method will match a header name with an appropriate type
 * If the header is not found, then the type will return unknown
 */
Type column_type(const char *column) {
	int i;
	const char* names[20]={"IDentification Name","USAF","NCDC","Date","HrMn","I","Type","Wind Dir","Wind Dir Q","Wind Dir I","Wind Spd","Wind Spd Q","Temp","Temp Q","Dewpt","Dewpt Q","Slp","Slp Q","RHx","QCP"};
	const Type types[20]={STRING,INT,INT,INT,INT,CHAR,STRING,INT,INT,CHAR,FLOAT,INT,FLOAT,INT,FLOAT,INT,FLOAT,INT,INT,STRING}; //Types that correspond to Header
	for(i=0; i<20; i++) {
		if(strcmp(column,names[i])==0) {return types[i];} //Will return matching type
	}
	return UNKNOWN;

}
/*
 * Checks type of header against
 */
int matches_type(const char *column, const char *data) {
	int i,j,k;
	j=0;
	int len=strlen(data);
	//if (strlen(data)==0) {return -1;} According to test t10.csv, this could be valid
	char* column_names[20]={"IDentification Name","USAF","NCDC","Date","HrMn","I","Type","Wind Dir","Wind Dir Q","Wind Dir I","Wind Spd","Wind Spd Q","Temp","Temp Q","Dewpt","Dewpt Q","Slp","Slp Q","RHx","QCP"};
	char* WIvalues[10]={"9","A","B","C","H","N","Q","R","T","V"};
	char* type_vals[38]={"99999","AERO","AUST","AUTO","BOGUS","BRAZ","COOPD","COOPS","CRB","CRN05","CRN15","FM-12","FM-13","FM-14","FM-15","FM-16",
			"FM-18","GREEN","MESOS","MEXIC","NSRDB","PCP15","PCP60","S-S-A","SA-AU","SAO","SAOSP","SHEF","SMARS","SOD","SOM","SURF","SY-AE",
			"SY-AU","SY-MT","SY-SA","WBO","WNO"
	};
	char* temp_q[16]={"0","1","2","3","4","5","6","7","9","A","C","I","M","P","R","U"};
	char* dewpt_q[16]={"0","1","2","3","4","5","6","7","9","A","C","I","M","P","R","U"};
	int header_index=-1; //Will return that if header not found
	int data_l=strlen(data);
	int data_num;
	float data_f;
	for (i=0; i<20; i++) {
		if(strcmp(column,column_names[i])==0) { header_index=i;}
	}
	switch(header_index) { //Yes, I know some of these can be combined.
		case 0: //Identification Name
			if (data_l>30) { return -1;}
			return 0; //Not much checking I can do with that.
			break;
		case 1: //USAF
			data_num=atoi(data);
			if ((data_num<0)||(data_num>999999)) {return -1; } //checks type falls into correct range
			return 0;
			break;
		case 3: //Case Date
			data_num=atoi(data);
			if (data_num<0||data_num>99999999) {return -1;} //correct range
			return 0;
			break;
		case 4: //Case HRMN
			data_num=atoi(data);
			if (data_num<0||data_num>9999) { return -1;}
			return 0;
			break;
		case 5: //I
			if (data_l!=1) {return -1;}
			char* values[22]={"1","2","3","4","5","6","7","8","A","B","C","D","E","F","G","H","I","J","K","L","M","N"};
			for (i=0; i<22; i++) {
				if (strcmp(data,values[i])==0) {return 0;}
			}
			return -1;
			break;
		case 6: //Type
			for (i=0; i<38; i++) {
				if (strcmp(data,type_vals[i])==0) {return 0;}
			}
			return -1;
			break;
		case 7: //Wind Dir
			data_num=atoi(data);
			if ((0>data_num)||(data_num>999)) {return -1;}
			return 0;
			break;
		case 8: //Wind Dir Q
			data_num=atoi(data);
			if (0>data_num||data_num>9) { return -1;}
			return 0;
			break;
		case 9: //Wind Dir I

			for(i =0; i<10; i++) {
					if(strcmp(data,WIvalues[i])==0) { return 0;}
			}
			return -1;
			break;
		case 10: //Wind Spd
			data_f=atof(data);
			if (data_f<0||data_f>9999) {return -1;}
			return 0;
			break;
		case 11: //Wind Spd Q
			data_num=atoi(data);
			if (data_num<0||data_num>9) {return -1;}
			return 0;
			break;
		case 12: //Temp
			data_f=atof(data);
			if (data_f<-99999||data_f>99999) {return -1;}
			return 0;
			break;
		case 13: //Temp Q
			for (i=0; i<16; i++) {
				if (strcmp(data,temp_q[i])==0) { return 0;}
			}
			return -1;
			break;
		case 14: //Dewpt
			for(j=0; j<data_l; j++) {
				if (data_l>5) { return -1;}
				if(data[j]!='.'&&data[j]!='-'&&data[j]!='+') {
					k=(int) data[j]-'0';
					if (k<0||k>10) { return -1;}
				}
			}
			data_f=atof(data);
			if (data_f<-9999||data_f>9999) { return -1;}
			return 0;
			break;
		case 15: //Dewpt q
			for (i=0; i<16; i++) {
				if (strcmp(data,dewpt_q[i])==0) { return 0;}
			}
			return -1;
			break;
		case 16:  //Slp
			data_f=atof(data);
			if (data_f<0||data_f>99999) { return -1;}
			return 0;
			break;
		case 17: //Slp Q
			data_num=atoi(data);
			if (data_num<0||data_num>99) { return -1;}
			return 0;
			break;
		case 18: //RHx
			data_num=atoi(data);
			if (data_num<0||data_num>999) { return -1;}
			return 0;
			break;
		case 2:
			//data_num=atoi(data);
			if (data_l>5) { return -1;}
			return 0;
			break;
		case 19:
			if (data_l>4) {return -1;}
			return 0;
			break;
		default:
			return -1; //Header not right name
			break;
		}
		return -1; //I programmed something wrong
		}
/*
 * Frees the void** array
 */
void free_data(void** data, int size) {
	int i;
	for(i=0; i<size; i++) {
		free(data[i]);
	}
	free(data);
	}

