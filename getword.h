#include<cstdio>
#include<cstdlib>
#include<cctype>
#include<cstring>
#include<queue>
#include<string>
#include<stack>
#define LL long long int
char text[10005];
char c_get,Token[10005];
int pos = 0,L = 0;
int ti = 0;
char keyWord[][20]={"if","else","while","break","continue","return","int","void","const"};//keyWord 
char keyWordUp[][20]={"If","Else","While","Break","Continue","Return","Int","Void","Const"};
const int key_num = 9;

char id_map[][20]={"Err","Ident","Number","If","Else","While","Break",
					"Continue","Return","Int","Void","Const","Assign","Semicolon","LPar",
					"RPar","LBrace","RBrace","Lbra","Rbra","Plus","Minus","Mult","Div",
					"Remain","Gt","Lt","Not","Comma","Eq","And","Or","Ge","Le","Ne"};

/*
id表：
0	Err
1	Ident($name) 
2	Number($number)
3	If
4	Else
5 	While
6	Break
7	Continue
8	Return
9	Int
10	Void
11	Const
12	Assign =
13	Semicolon ;
14	LPar (
15	RPar )
16	LBrace {
17	RBrace }
18	Lbra [
19	Rbra ]
20	Plus +
21	Minus -
22	Mult *
23	Div /
24	Remain %
25	Gt >
26	Lt <
27	Not !
28	Comma ,
29	Eq ==
30	And &&
31	Or ||
32	Ge >=
33	Le <=
34	Ne !=

*/

//语法输出 
struct node{
	int id{};
	LL num{};
	char* name{};
	node()= default;
	explicit node(int id){
		this->id = id;
	}
	node(int id,LL num,int nothing){
		this->id = id;
		this->num = num;
	}
	node(int id,const char* name){
		this->id = id;
		this->name = new char[20];
		strcpy(this->name,name);
	}
}words[100005];
std::stack<int> registerStack;
std::stack<char> opStack;
int register_num = 0;
int words_len = 0;

//read
char get_char(){
	return ++pos <= L ? text[pos] : EOF;
}

void checkToken(){
	Token[ti] = '\0';//Unsafe
	for (int i = 0; i < key_num; i++){
		if (strcmp(Token,keyWord[i]) == 0){
			words[++words_len] = node(i + 3);//puts(keyWordUp[i]);
			return;
		}
	}
	words[++words_len] = node(1,Token);//puts(Ident);
}

bool is_hex(char ch){
	return isdigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
}

int hex(char ch){
	return ch >= '0' && ch <= '9' ? (ch - '0') : (tolower(ch) - 'a' + 10);
}

bool is_oct(char ch){
	return ch >= '0' && ch <= '7';
}

void get_number(){
	LL num = 0;
	bool flag = true;
	if (c_get == '0'){
		c_get = get_char();
		if (c_get == 'x' || c_get == 'X'){
			c_get = get_char();
			if (is_hex(c_get)){
				num = hex(c_get);
				c_get = get_char();
				while (is_hex(c_get)){
					num = num * 16 + hex(c_get);
					c_get = get_char();
				}
			}
			else flag = false;
		}
		else if (is_oct(c_get)){
			num = c_get - '0';
			c_get = get_char();
			while (is_oct(c_get)) {
				num = num * 8 + c_get - '0';
				c_get = get_char();
			}
		}
		else {
			num = 0;	
		}
	}
	else if (isdigit(c_get)){
		num = c_get - '0';
		c_get = get_char();
		while (isdigit(c_get)) {
			num = num * 10 + c_get - '0';
			c_get = get_char();
		}
	}
	else flag = false;
	if (flag) words[++words_len] = node(2,num,1);//printf("Number(%lld)\n",num);
	else words[++words_len] = node(0);//puts("Err");
}
void work(){
	if (c_get == ' ' || c_get == '\n' || c_get == '\r' || c_get == '\t'){
		c_get = get_char();
		return;
	}
	if (isalpha(c_get) || c_get == '_'){
		ti = 0;
		while (isalpha(c_get) || isdigit(c_get) || c_get == '_'){
			Token[ti++] = c_get;
			c_get = get_char();
		}
		checkToken();
		return;
	}
	else if (isdigit(c_get)){
		get_number();
		return;
	}
	else if (c_get == '='){
		c_get = get_char();
		if (c_get == '=') words[++words_len] = node(29);//puts("Eq");
		else {
			words[++words_len] = node(12);//puts("Assign");
			return;
		}
	}
	else if (c_get == '!'){
		c_get = get_char();
		if (c_get == '=') words[++words_len] = node(34);//puts("Ne");
		else {
			words[++words_len] = node(27);//puts("Not");
			return;
		}
	}
	else if (c_get == '>'){
		c_get = get_char();
		if (c_get == '=') words[++words_len] = node(32);//puts("Ge");
		else {
			words[++words_len] = node(25);//puts("Gt");
			return;
		}
	}
	else if (c_get == '<'){
		c_get = get_char();
		if (c_get == '=') words[++words_len] = node(33);//puts("Le");
		else {
			words[++words_len] = node(26);//puts("Lt");
			return;
		}
	}
	else if (c_get == '&'){
		c_get = get_char();
		if (c_get == '&') words[++words_len] = node(30);//puts("And");
		else words[++words_len] = node(0);//puts("Err");
	}
	else if (c_get == '|'){
		c_get = get_char();
		if (c_get == '|') words[++words_len] = node(31);//puts("Or");
		else words[++words_len] = node(0);//puts("Err");
	}
	else if (c_get == ';') words[++words_len] = node(13);//puts("Semicolon");
	else if (c_get == '(') words[++words_len] = node(14);//puts("LPar");
	else if (c_get == ')') words[++words_len] = node(15);//puts("RPar");
	else if (c_get == '{') words[++words_len] = node(16);//puts("LBrace");
	else if (c_get == '}') words[++words_len] = node(17);//puts("RBrace");
	else if (c_get == '[') words[++words_len] = node(18);//puts("Lbra");
	else if (c_get == ']') words[++words_len] = node(19);//puts("Rbra");
	else if (c_get == '+') words[++words_len] = node(20);//puts("Plus");
	else if (c_get == '-') words[++words_len] = node(21);//puts("Minus");
	else if (c_get == '*') words[++words_len] = node(22);//puts("Mult");
	else if (c_get == '/') words[++words_len] = node(23);//puts("Div");
	else if (c_get == '%') words[++words_len] = node(24);//puts("Remain");
	else if (c_get == ',') words[++words_len] = node(28);//puts("Comma");
	else words[++words_len] = node(0);//puts("Err");
	c_get = get_char();
}


//初始化
void get_in(){
    //printf("declare void @putch(i32)\ndefine i32 @main(){\n");
	c_get = getchar();
	while (c_get != EOF){
        if (c_get == '\n'){
            printf(" ");
        }else{printf("%c",c_get);}
		text[++L] = c_get;
        //printf("\tcall void @putch(i32 %d)\n",(int)c_get);
		c_get = getchar();
	}
}
void get_clear(){    //去注释 
	int head = -1,last = false;
	for (int i = 1; i <= L; i++){
		if (last && text[i] == '*'){
			head = i - 1;
			int end = i + 1, laststar = false;
			for (; end < L; end++){
				if (laststar && text[end] == '/') break;
				else if (text[end] == '*') laststar = true;
				else laststar = false;
			}
			for (int j = head; j <= end; j++) text[j] = ' ';
			i = end;
		}
		else if (text[i] == '/'){
			if (!last) last = true;
			else {
				head = i - 1;
				int end = i;
				for (; end < L; end++){
					if (text[end + 1] == '\n' || text[end + 1] == '\r') break;
				}
				for (int j = head; j <= end; j++) text[j] = ' ';
				i = end;
			}
		}
		else last = false;
	}
}

void init(){
	get_in();
	get_clear();
}

void getSym(){
	init();
	c_get = get_char();
	while (c_get != EOF) work();
	words[++words_len].id = -1;
}

int now = 0;
int get_next(){
	if (now < words_len) {
        ++now;
        //printf("%d %d\n",now, words[now].id);
        return now;
    } else {
        return 0;
    }
}

void checkFuncPar(){
    while(words[now].id != 15) {

    }
}

void emptyRegisterStack(){
    while (!registerStack.empty()){
        registerStack.pop();
    }
}

void emptyOpStack(){
    while (!opStack.empty()){
        opStack.pop();
    }
}
/*int main(){
	getsym();
	for (int i = 1; i <= words_len; i++){
		if (words[i].id == 1) printf("%s(%s) ",id_map[words[i].id],words[i].name);
		else if (words[i].id == 2) printf("%s(%lld) ",id_map[words[i].id],words[i].num);
		else printf("%s ",id_map[words[i].id]);
	}
	return 0;
} */
