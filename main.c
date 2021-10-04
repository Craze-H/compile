#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
char nowch;
char token[100];
char alpha_flag,num_flag,eql_flag;
void print_token(char type);
int len;
int main(){
	while((nowch = getchar())!=EOF) {
		if(eql_flag == '=' && nowch != '=') {
			eql_flag = 0;
			printf("Assign\n");
		}
		if(!(isalpha(nowch) || isdigit(nowch) || nowch == '_')) {
			if(alpha_flag != 0 || num_flag != 0) {
				if(num_flag != 0) {
					print_token('n');
					num_flag = 0;
				}
				else {
					print_token('i');
					alpha_flag = 0;
				}
			}
			switch(nowch) {
				case ';':
					printf("Semicolon\n");
					break;
				case '(':
					printf("LPar\n");
					break;
				case ')':
					printf("RPar\n");
					break;
				case '{':
					printf("LBrace\n");
					break;
				case '}':
					printf("RBrace\n");
					break;
				case '+':
					printf("Plus\n");
					break;
				case '*':
					printf("Mult\n");
					break;
				case '/':
					printf("Div\n");
					break;
				case '<':
					printf("Lt\n");
					break;
				case '>':
					printf("Gt\n");
					break;
				case '='://注意判断非符号前先看看eql_flag 
					if(eql_flag == '=') {
						printf("Eq\n");
						eql_flag = 0;
					}
					else {
						eql_flag = '=';
					}
					break;
				case '\n':
				case '\t':
				case '\r':
				case ' ':
					break;
				default:
					printf("Err\n");
			}
		}
		else {
			if(isalpha(nowch) || nowch == '_') {
				if(num_flag != 0) {
					print_token('n');
					num_flag = 0;
				}
				alpha_flag = nowch;
				len = strlen(token);
				token[len] = nowch;//unsafe
				token[len + 1] = 0;//unsafe
			}
			else {
				if(alpha_flag == 0) {
					num_flag = nowch;
					len = strlen(token);
					token[len] = nowch;//unsafe
					token[len + 1] = 0;//unsafe
				}
				else {
					alpha_flag = nowch;
					len = strlen(token);
					token[len] = nowch;//unsafe
					token[len + 1] = 0;//unsafe
				}
			}
		}
	}
	return 0;
}
void print_token(char type) {
	token[strlen(token)] = '\0';//unsafe
	if(strcmp(token,"if") == 0) {
		token[0] = 0;
		printf("If\n");
		return;
	}
	if(strcmp(token,"else") == 0) {
		token[0] = 0;
		printf("Else\n");
		return;
	}
	if(strcmp(token,"while") == 0) {
		token[0] = 0;
		printf("While\n");
		return;
	}
	if(strcmp(token,"break") == 0) {
		token[0] = 0;
		printf("Break\n");
		return;
	}
	if(strcmp(token,"continue") == 0) {
		token[0] = 0;
		printf("Continue\n");
		return;
	}
	if(strcmp(token,"return") == 0) {
		token[0] = 0;
		printf("Return\n");
		return;
	}
	switch(type) {
		case 'i':
			printf("Ident");
			break;
		case 'n':
			printf("Number");
			break;
	}
	printf("(%s)\n",token);
	token[0] = 0;
}
