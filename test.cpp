#include"getword.h"
void checkFunContent();
bool funcRead();
void Exp();
void Stmt();
void calculate();
void printRegister(int n);
int now_pos;
int main(){
#ifdef LOCAL
    freopen("in.txt", "r", stdin);
#endif
	getSym();
    //return 0;
    printf("\tret i32 0\n");
    printf("}\n");
    return 0;
    words[0].id = 0;
	/*for (int i = 1; i <= words_len; i++){
        printf("%d %d ", i, words[i].id);
		if (words[i].id == 1) printf("%s(%s) ",id_map[words[i].id],words[i].name);
		else if (words[i].id == 2) printf("%s(%lld) ",id_map[words[i].id],words[i].num);
		else printf("%s ",id_map[words[i].id]);
        puts("");
	}*/
    now_pos = get_next();
	if(words[now_pos].id == 9){
        now_pos = get_next();
        if (!funcRead()) {
            return -1;
        }
		return 0;
	}
	else {
		return -2;
	}
}

bool funcRead(){
    if(words[now_pos].id == 1 && !(strcmp(words[now_pos].name, "main"))){
        printf("define dso_local i32 @%s(){\n", words[now_pos].name);
        now_pos = get_next();
    } else {
        return false;
    }
    if (words[now_pos].id == 14){
        now_pos = get_next();
        checkFuncPar();
        now_pos = get_next();
    } else{
        return false;
    }
    if(words[now_pos].id == 16){
        now_pos = get_next();
        checkFunContent();
        if (now_pos == 0){
            return false;
        }
        if (now_pos == words_len){
            return true;
        }
        now_pos = get_next();
    }
    return true;
}

void Stmt(){
    if (words[now_pos].id == 8){
        now_pos = get_next();
        Exp();
        if (now_pos == 0){
            return;
        }
        if (words[now_pos].id == 13){
            printf("ret i32 ");
            printRegister(registerStack.top());
            puts("");
            now_pos = get_next();
        } else{
            now_pos = 0;
        }
    }
}

void checkFunContent(){
    while (words[now_pos].id != 17){
        if (now_pos == 0){
            return;
        }
        Stmt();
    }
    printf("}\n");
}

void Exp(){
    emptyRegisterStack();
    emptyOpStack();
    char last_word_key = 0;
    int lPar_num = 0;
    while (true){
        if (words[now_pos].id == 14){
            lPar_num++;
            //printf("lPar_num: %d\n",lPar_num);
            last_word_key = '(';
            opStack.push('(');
        } else if (words[now_pos].id == 15){
            //printf("lPar_num: %d\n",lPar_num);
            if (lPar_num > 0){
                //printf("COME IN!!!\n");
                lPar_num--;
                last_word_key = '0';
                while (!opStack.empty() && opStack.top() != '('){
                    calculate();
                }
                opStack.pop();
            } else{
                //judge finish
                calculate();
                break;
            }
        } else if (words[now_pos].id == 20 || words[now_pos].id == 21){
            if (last_word_key != '0'){
                if (words[now_pos].id == 20){
                    last_word_key = '+';
                    opStack.push('#');
                } else{
                    last_word_key = '-';
                    opStack.push('$');
                }
            } else{
                while (!opStack.empty() && opStack.top() != '('){
                    if (now_pos == 0){
                        break;
                    }
                    calculate();
                }
                if (now_pos == 0){
                    break;
                }
                if (words[now_pos].id == 20){
                    last_word_key = '+';
                    opStack.push('+');
                } else{
                    last_word_key = '-';
                    opStack.push('-');
                }
            }
        } else if (words[now_pos].id > 21 && words[now_pos].id < 25){
            while (!opStack.empty() && opStack.top() != '(' && opStack.top() != '+' && opStack.top() != '-'){
                calculate();
            }
            if (now_pos == 0){
                break;
            }
            switch (words[now_pos].id) {
                case 22:
                    last_word_key = '*';
                    opStack.push('*');
                    break;
                case 23:
                    last_word_key = '/';
                    opStack.push('/');
                    break;
                case 24:
                    last_word_key = '%';
                    opStack.push('%');
                    break;
            }
        } else if(words[now_pos].id == 2){
            registerStack.push(0 - words[now_pos].num);
            //printf("%d\n", registerStack.top());
            last_word_key = '0';
        }
        else{
            if (lPar_num != 0){
                now_pos = 0;
                break;
            }
            while (!opStack.empty()){
                calculate();
            }
            break;
        }
        //if (!opStack.empty()) printf("opTop: %c\n", opStack.top());
        now_pos = get_next();
    }
    if (now_pos == 0){
        return;
    }
}

void calculate(){
    char op = opStack.top();
    //printf("%c\n", op);
    opStack.pop();
    if (!registerStack.empty()){
        if (op == '#' || op == '$'){
            printf("%%%d = ", ++register_num);
            if (op == '#'){
                printf("add");
            } else{
                printf("sub");
            }
            printf(" i32 0, ");
            printRegister(registerStack.top());
            puts("");
            registerStack.pop();
            registerStack.push(register_num);
        } else{
            if (registerStack.size() >= 2){
                printf("%%%d = ", ++register_num);
                switch (op) {
                    case '+':
                        printf("add");
                        break;
                    case '-':
                        printf("sub");
                        break;
                    case '*':
                        printf("mul");
                        break;
                    case '/':
                        printf("sdiv");
                        break;
                    case '%':
                        printf("srem");
                        break;
                }
                printf(" i32 ");
                int n1 = registerStack.top();
                registerStack.pop();
                int n2 = registerStack.top();
                registerStack.pop();
                printRegister(n2);
                printf(", ");
                printRegister(n1);
                puts("");
                registerStack.push(register_num);
            } else{
                now_pos = 0;
            }
        }
    } else{
        now_pos = 0;
    }
}

void printRegister(int n){
    if (n > 0){
        printf("%%");
    }
    printf("%d", abs(n));
}
/*
1 9 Int
2 1 Ident(main)
3 14 LPar
4 15 RPar
5 16 LBrace
6 8 Return
7 2 Number(1)
8 20 Plus
9 14 LPar
10 21 Minus
11 2 Number(2)
12 15 RPar
13 22 Mult
14 14 LPar
15 2 Number(3)
16 23 Div
17 14 LPar
18 2 Number(4)
19 21 Minus
20 2 Number(5)
21 15 RPar
22 15 RPar
23 13 Semicolon
24 17 RBrace
25 -1
 */