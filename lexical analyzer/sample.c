#include <stdio.h>
#include <string.h>
#include <fcntl.h>

/* Some definitions */
#define MAXBUFSIZE  1000
#define Max_Token_Size 100
#define AB TOKEN_AB

typedef enum {
	AB, A, B
}TokenType;


/*----- The prototype of functions in this functions---*/
int  get_next_token(void);
int scan(void);
void print_token(TokenType);
int fail(int);

/*--------------- The Global variables----------------*/
int  fd;			  /* File descriptor to the input file*/

char input_buf[MAXBUFSIZE];  /* Maximum buffer size */


char *start_pt;		/*The pointer pointing to the starting  position for 
                                    the current token recongnizatioin*/	  

int forward;			  /*The distance from start_pt for the 
                                    current token recongnization*/	
char token_val[Max_Token_Size];	  /* The token lexeme value*/


/*----------Function: print_token()--------------------
	Print token based on definition and requirment
------------------------------------------------------*/
void print_token(TokenType token)
{
	switch(token){
		case AB: 
			printf("AB\t\t\t| %s\n",token_val);
			break;
		case A:
			printf("A\t\t\t| %s\n",token_val);
			break;
		case B:
			printf("B\t\t\t| %s\n",token_val);
			break;
	}
}

/*---------Function: fail()----------------------------
   Based on the current state, to decide the next state
	current state < 1, next state <- 1
	current state < 2, next state <- 2 
	current state < 3, next state <- 3 
-------------------------------------------------------*/
int fail(int cstate)
{
	int next_state;
	forward = 0;

	if( cstate < 2 )
		next_state = 2;
	
	return next_state;
}


/*--------Function: get_next_token()-----------------------
   Return the next token. 
   Note: Return -1 if normal finish;
         Return -2 if there is non-recongnizd a character 
---------------------------------------------------------*/
int  get_next_token()
{
	char c;
	int i;
	int current_state = 0;
	forward = 0;

	while(1){
		c = start_pt[forward];
	
		switch (current_state ){
			case 0:
				if(c=='a'){
					forward++;
					current_state = 1;
				}else{
					current_state=fail(current_state);
				}
				break;

			case 1:
				if(c=='a'){
					forward++;
					current_state = 1;
				}else if ( c == 'b'){
					for(i=0; i<=forward; i++)
						token_val[i]=start_pt[i];
					token_val[i]='\0';
					return AB;
				}else{
					current_state=fail(current_state);
				}
				break;
	
			case 2:
				if(c=='a'){
					for(i=0; i<=forward; i++)
						token_val[i]=start_pt[i];
					token_val[i]='\0';
					return A;
				}else if(c=='b'){
					for(i=0; i<=forward; i++)
						token_val[i]=start_pt[i];
					token_val[i]='\0';
					return B;
				}else	if(c=='\0')
					return -1;  /* succeed*/
				else
					return -2; /* fail*/
				break;

			default:
				printf("Wrong state (should not occur)\n");
				return -1;
		}				
	}
}

/*-------------Function: scan() ---------------------------
	Get the return token and call print_token to print it
----------------------------------------------------------*/
int scan()
{
	int  ret, rbytes;

	start_pt=input_buf;
	
	/* Read the input file into a buffer */
      /* Only consider the simple case when the buffer is big enough*/
      rbytes = read( fd, input_buf, MAXBUFSIZE );

	if( rbytes  < 0 ){
		printf("Could not read from the file\n");
		close(fd);
		return -1;
	}else if (rbytes == 0 ){
		printf("The input file is empty.\n");	
		close(fd);
		return -1;
	}else{
		input_buf[rbytes-1]='\0';  /*the file ends with \0*/
	}

	printf("Token Name\t\t|Token Value \n");
	printf("--------------------------------------------\n");
	while( ( ret=get_next_token() ) >= 0 ){
		print_token(ret); 
		start_pt = start_pt + forward +1;
	}

	if (ret == -1 )
		printf("\nSucessfuly finish scan.\n");
	else
		printf("\nError: Non-recongnizable character in the input file.\n");
	
	close(fd);
	return 1;
}
		
int main(int argc, char *argv[])
{
	/* Check the number of input*/
	if( argc != 2){
		printf("Usage: scan input_file \n");
		return -1;
	}

	/* Open the source fie*/
	if( (fd = open(argv[1], O_RDONLY)) < 0 ){
		printf("Can not open the file (File_Name:%s).\n", argv[1]);
		return -1;
	}
	
	/* Process lexical analysis*/
	if (scan() > 0)	
		return 1;
	else
		return -1;
}
	

