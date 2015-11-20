#include <stdio.h>
#include <string.h>
#include <fcntl.h>

/* Some definitions */
#define MAXBUFSIZE  1000
#define Max_Token_Size 100
#define AB TOKEN_AB
#define TRUE   1
#define FALSE  -1

typedef enum {
	NUM,VAR,BEGIN,END,ID,COMMA,SEMICOLON,ASSIGN,PERIOD,PLUS,MINUS,MUL,DIV,LBRACE,RBRACE,delim
}TokenType;


/*----- The prototype of functions in this functions---*/
int  get_next_token(void);
int scan(void);
void save_token(TokenType);
int fail(int);
TokenType getNextToken(void);

/*--------------- The Global variables----------------*/
int  fd;			  /* File descriptor to the input file*/

char input_buf[MAXBUFSIZE];  /* Maximum buffer size */


char *start_pt;		/*The pointer pointing to the starting  position for 
                                    the current token recongnizatioin*/	  

int forward;			  /*The distance from start_pt for the 
                                    current token recongnization*/	
char token_val[Max_Token_Size];	  /* The token lexeme value*/

TokenType tokens[100];
int t_index = -1;
int sa_index = 0;


/*----------Function: save_token()--------------------
	save token into array tokens[]
------------------------------------------------------*/
void save_token(TokenType token)
{
	if(token==delim)
		return;
	t_index++;
	tokens[t_index] = token;
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
					TokenType ret;
					c_kw[0] = start_pt[forward];
					c_kw[1] = start_pt[forward+1];
					c_kw[2] = start_pt[forward+2];
					c_kw[3] = start_pt[forward+3];
					c_kw[4] = start_pt[forward+4];
					if(c_kw[0]=='v'&&c_kw[1]=='a'&&c_kw[2]=='r'){
						forward = 2;
						ret = VAR;
					}else if(c_kw[0]=='b'&&c_kw[1]=='e'&&c_kw[2]=='g'&&c_kw[3]=='i'&&c_kw[4]=='n'){
						forward = 4;
						ret = BEGIN;
					}else if(c_kw[0]=='e'&&c_kw[1]=='n'&&c_kw[2]=='d'){
						forward = 2;
						ret = END;
					}else{
						current_state=fail(current_state);
						break;
					}
					for(i=0; i<=forward; i++)
						token_val[i]=start_pt[i];
					token_val[i]='\0';
					return ret;
				}
				break;

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
	Get the return token and call save_token to save it
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
	printf("\n******lexical analysis******\n");
	while( ( ret=get_next_token() ) >= 0 ){
		save_token(ret); 
		start_pt = start_pt + forward +1;
	}
	
	if (ret == -1 ){
		printf("Sucessfuly finish scan.\n");
		ret = 1;
	}	
	else{
		printf("Error: Non-recongnizable character in the input file.\n");
		ret = 0;
	}	
	close(fd);
	return ret;
}

/*-------------Function: getNextToken() ---------------------------
	Get a token from token[] for syntax analysis, return enum TokenType
-----------------------------------------------------------------*/	
TokenType getNextToken(){
	if(!(sa_index>t_index))
		return tokens[sa_index];
}
/*-------------Function: sa() ---------------------------
	start syntax analysis
-----------------------------------------------------------------*/
int sa(){
	printf("\n******syntax analysis******\n");
 	if(PROG()==TRUE){
                printf("program parsed successfully .\n");
		return 1;
	}
        else{
                printf("syntax error \n");
		return -1;
	}
}
/*-------------set of functions for syntax analysis ---------------
	function for syntax analysis:
PROG()	 	| PROG -> PROG_BODY .
PROG_BODY()	| PROG_BODY -> var ID_LIST; STATEMENT_LIST
ID_LIST()	| ID_LIST -> id ID_SUB_LIST
ID_SUB_LIST() 	| ID_SUB_LIST -> ,id ID_SUB_LIST | e
STMT_LIST() 	| STATEMENT_LIST -> begin STATEMENT end
STMT()	 	| STATEMENT -> id=E; SUB_STATEMENT
SUB_STMT() 	| SUB_STATEMENT -> id=E; SUB_STATEMENT | STATEMENT_LIST SUB_STATEMENT | e
E()	 	| E -> (E) SUB_E | num SUB_E | id SUB_E
SUB_E()	 	| SUB_E -> +E SUB_E | -E SUB_E | *E SUB_E| /E SUB_E | e
-----------------------------------------------------------------*/
int PROG(){
	printf("Program begins\n");
	if(PROG_BODY()==TRUE){
		if(getNextToken()==PERIOD){
			if(sa_index!=t_index)
				return FALSE;
			printf("Program ends\n");
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

int PROG_BODY(){
	printf("	Program body begins\n");
	if(getNextToken()==VAR){
		printf("		declaration begins\n");
		sa_index++;
		if(ID_LIST()==TRUE){
			if(getNextToken()==SEMICOLON){
				printf("		declaration ends\n");
				sa_index++;
				if(STMT_LIST()==TRUE){
					printf("	Program body ends\n");
					return TRUE;
				}
				return FALSE;
			}
			return FALSE;
		}
		return FALSE;
	}

	return FALSE;
}

int ID_LIST(){//ID_LIST()	| ID_LIST -> id ID_SUB_LIST
	if(getNextToken()==ID){
		sa_index++;
		if(ID_SUB_LIST()==TRUE){
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}
int ID_SUB_LIST(){//ID_SUB_LIST() 	| ID_SUB_LIST -> ,id ID_SUB_LIST | e
	if(getNextToken()==COMMA){
		sa_index++;
		if(getNextToken()==ID){
			sa_index++;
			if(ID_SUB_LIST()==TRUE){
				return TRUE;
			}
			return FALSE;
		}
		return FALSE;
	}
	return TRUE;
}
int STMT_LIST(){//STMT_LIST() 	| STATEMENT_LIST -> begin STATEMENT end
	if(getNextToken()==BEGIN){
		printf("		statement begins\n");
		sa_index++;
		if(STMT()==TRUE){
			if(getNextToken()==END){
				printf("		statement ends\n");
				sa_index++;
				return TRUE;
			}
			return FALSE;
		}
		return FALSE;
	}
	return FALSE;
}
int STMT(){//STMT()	 	| STATEMENT -> id=E; SUB_STATEMENT
	if(getNextToken()==ID){
		printf("			assignment begins\n");
		sa_index++;
		if(getNextToken()==ASSIGN){
			sa_index++;
			if(E()==TRUE){
				if(getNextToken()==SEMICOLON){
					printf("			assignment ends\n");
					sa_index++;
					if(SUB_STMT()==TRUE){
						return TRUE;
					}
					return FALSE;
				}
				return FALSE;
			}
			return FALSE;
		}
		return FALSE;
	}
	return FALSE;

}
int SUB_STMT(){//SUB_STMT() 	| SUB_STATEMENT -> id=E; SUB_STATEMENT | STATEMENT_LIST SUB_STATEMENT | e
	if(getNextToken()==ID){
		printf("			assignment begins\n");
		sa_index++;
		if(getNextToken()==ASSIGN){
			sa_index++;
			if(E()==TRUE){
				if(getNextToken()==SEMICOLON){
					sa_index++;
					printf("			assignment ends\n");
					if(SUB_STMT()==TRUE){
						return TRUE;
					}
					return FALSE;
				}
			return FALSE;
			}
		return FALSE;
		}
	return FALSE;
	}
	if(STMT_LIST()==TRUE){
		if(SUB_STMT()==TRUE){
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}
int E(){//E()	 	| E -> (E) SUB_E | num SUB_E | id SUB_E
	if(getNextToken()==LBRACE){
		sa_index++;
		if(E()==TRUE){
			if(getNextToken()==RBRACE){
				sa_index++;
				if(SUB_E()==TRUE){
					return TRUE;
				}
				return FALSE;
			}	
			return FALSE;
		}
		return FALSE;	
	}
	if(getNextToken()==NUM){
		sa_index++;
		if(SUB_E()==TRUE){
			return TRUE;
		}
		return FALSE;
	}
	if(getNextToken()==ID){
		sa_index++;
		if(SUB_E()==TRUE){
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}
int SUB_E(){//SUB_E()	 	| SUB_E -> +E SUB_E | -E SUB_E | *E SUB_E| /E SUB_E | e
	if(getNextToken()==PLUS){
		sa_index++;
		if(E()==TRUE){
			if(SUB_E()==TRUE){
				return TRUE;
			}
			return FALSE;
		}
		return FALSE;
	}
	if(getNextToken()==MINUS){
		sa_index++;
		if(E()==TRUE){
			if(SUB_E()==TRUE){
				return TRUE;
			}
			return FALSE;
		}
		return FALSE;
	}
	if(getNextToken()==MUL){
		sa_index++;
		if(E()==TRUE){
			if(SUB_E()==TRUE){
				return TRUE;
			}
			return FALSE;
		}
		return FALSE;
	}
	if(getNextToken()==DIV){
		sa_index++;
		if(E()==TRUE){
			if(SUB_E()==TRUE){
				return TRUE;
			}
			return FALSE;
		}
		return FALSE;
	}
	return TRUE;
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
	
	/* Process lexical analysis
	if (scan() > 0)	
		return 1;
	else
		return -1;*/
	if(scan() == 1){
		sa();
		return 1;
	}
	else
		return -1;
}
	

