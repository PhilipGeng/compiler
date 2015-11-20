#include <stdio.h>
#include <string.h>
#include <fcntl.h>

/* Some definitions */
#define MAXBUFSIZE  1000
#define Max_Token_Size 100
#define AB TOKEN_AB

typedef enum {
	NUM,KEYWORD,ID,COMMA,SEMICOLON,ASSIGN,PERIOD,PLUS,MINUS,MUL,DIV,LBRACE,RBRACE,delim
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
		case NUM: 
			printf("NUM\t\t\t| %s\n",token_val);
			break;
		case KEYWORD: 
			printf("KEYWORD\t\t\t| %s\n",token_val);
			break;
		case ID: 
			printf("ID\t\t\t| %s\n",token_val);
			break;
		case COMMA: 
			printf("COMMA\t\t\t| %s\n",token_val);
			break;
		case SEMICOLON: 
			printf("SEMICOLON\t\t| %s\n",token_val);
			break;
		case ASSIGN: 
			printf("ASSIGN\t\t\t| %s\n",token_val);
			break;
		case PERIOD: 
			printf("PERIOD\t\t\t| %s\n",token_val);
			break;
		case PLUS: 
			printf("PLUS\t\t\t| %s\n",token_val);
			break;
		case MINUS:
			printf("MINUS\t\t\t| %s\n",token_val);
			break;
		case MUL:
			printf("MUL\t\t\t| %s\n",token_val);
			break;
		case DIV: 
			printf("DIV\t\t\t| %s\n",token_val);
			break;
		case LBRACE: 
			printf("LBRACE\t\t\t| %s\n",token_val);
			break;
		case RBRACE: 
			printf("RBRACE\t\t\t| %s\n",token_val);
			break;
/*		case delim:
			printf("DELIM\t\t\t| %s\n",token_val);
			break;
*/
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

	if( cstate < 3 )
		next_state = 3;
	
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
	
		switch (current_state){
			case 0:
				if(c == ' '||c=='\t'||c=='\n'){//deal with white space
					for(i=0; i<=forward; i++)
						token_val[i]=start_pt[i];
					token_val[i]='\0';
					return delim;
				}
				else if(c>48&&c<58){//is 1-9
					forward++;
					current_state = 1;
				}
				else{
					char c_kw[5];
					c_kw[0] = start_pt[forward];
					c_kw[1] = start_pt[forward+1];
					c_kw[2] = start_pt[forward+2];
					c_kw[3] = start_pt[forward+3];
					c_kw[4] = start_pt[forward+4];
					if(c_kw[0]=='v'&&c_kw[1]=='a'&&c_kw[2]=='r'){
						forward = 2;
					}else if(c_kw[0]=='b'&&c_kw[1]=='e'&&c_kw[2]=='g'&&c_kw[3]=='i'&&c_kw[4]=='n'){
						forward = 4;
					}else if(c_kw[0]=='e'&&c_kw[1]=='n'&&c_kw[2]=='d'){
						forward = 2;
					}else{
						current_state=fail(current_state);
						break;
					}
					for(i=0; i<=forward; i++)
						token_val[i]=start_pt[i];
					token_val[i]='\0';
					return KEYWORD;
				}
				break;
	/*			if(c=='a'){
					forward++;
					current_state = 1;
				}else if(c == ' '||c=='\t'||c=='\n'){
					for(i=0; i<=forward; i++)
						token_val[i]=start_pt[i];
					token_val[i]='\0';
					return delim;
				}
				else
				{
					current_state=fail(current_state);
				}
				break;
*/
			case 1:
				if(c>47&&c<58){//is 0-9
					forward++;
					current_state = 1;
				}
				else if(c=='.'){
					forward++;
					current_state = 2;
				}else{
					current_state=fail(current_state);
				}
				break;
/*				if(c=='a'){
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
*/	
			case 2:
				if(c>47&&c<58){//is 0-9
					while(1){
						forward++;
						c = start_pt[forward];
						if(!(c>47&&c<58))
							break;
					}
					forward--;
					for(i=0; i<=forward; i++)
						token_val[i]=start_pt[i];
					token_val[i]='\0';
					return NUM;
				}
				else{
					current_state=fail(current_state);
				}
				break;
			/*
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
					return -1;  // succeed
				else
					return -2; // fail
				break;
*/
			case 3:
				switch(c){
					case ',':
						for(i=0; i<=forward; i++)
							token_val[i]=start_pt[i];
						token_val[i]='\0';
						return COMMA;
					case ';':
						for(i=0; i<=forward; i++)
							token_val[i]=start_pt[i];
						token_val[i]='\0';
						return SEMICOLON;
					case '=':
						for(i=0; i<=forward; i++)
							token_val[i]=start_pt[i];
						token_val[i]='\0';
						return ASSIGN;
					case '.':
						for(i=0; i<=forward; i++)
							token_val[i]=start_pt[i];
						token_val[i]='\0';
						return PERIOD;
					case '+':
						for(i=0; i<=forward; i++)
							token_val[i]=start_pt[i];
						token_val[i]='\0';
						return PLUS;
					case '-':
						for(i=0; i<=forward; i++)
							token_val[i]=start_pt[i];
						token_val[i]='\0';
						return MINUS;
					case '*':
						for(i=0; i<=forward; i++)
							token_val[i]=start_pt[i];
						token_val[i]='\0';
						return MUL;
					case '/':
						for(i=0; i<=forward; i++)
							token_val[i]=start_pt[i];
						token_val[i]='\0';
						return DIV;
					case '(':
						for(i=0; i<=forward; i++)
							token_val[i]=start_pt[i];
						token_val[i]='\0';
						return LBRACE;
					case ')':
						for(i=0; i<=forward; i++)
							token_val[i]=start_pt[i];
						token_val[i]='\0';
						return RBRACE;
					case '\0':
						return -1;
					default:
						if(c>48&&c<58){//is number 1-9
							while(1){
								forward++;
								c = start_pt[forward];
								if(!(c>47&&c<58))//not 0-9
									break;
							}
							forward--;
							for(i=0; i<=forward; i++)
								token_val[i]=start_pt[i];
							token_val[i]='\0';
							return NUM;
						}else if((c>64&&c<91)||(c>96&&c<123)){//is letter
							while(1){
								forward++;
								c = start_pt[forward];
								if(!((c>64&&c<91)||(c>96&&c<123)||(c>47&&c<58)))//not 0-9 nor letter
									break;
							}
							forward--;
							for(i=0; i<=forward; i++)
								token_val[i]=start_pt[i];
							token_val[i]='\0';
							return ID;
						
						}else{
							return -2;
						}

				} 
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
	

