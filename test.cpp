#include "register.h"
void checkFunContent();
bool funcRead();
void Exp();
void Stmt();
void calculate();
void printRegister(int n);
void Decl();
void reservedFunc(int reservedMode);
int now_pos;
int main(){
#ifdef LOCAL
    freopen("in.txt", "r", stdin);
#endif
	getSym();
    words[0].id = 0;
    emptyRegisterStack();
    emptyOpStack();
    printf("declare i32 @getint()\n"
           "declare i32 @getarray(i32*)\n"
           "declare i32 @getch()\n"
           "declare void @putint(i32)\n"
           "declare void @putch(i32)\n"
           "declare void @putarray(i32,i32*)\n");
    now_pos = get_next();
	if(words[now_pos].id == 9){
        now_pos = get_next();
        if (!funcRead()) {
            return now_pos - 1;
        }
		return 0;
	}
	else {
		return -2;
	}
}

bool funcRead(){
    if(words[now_pos].id == 1 && !(strcmp(words[now_pos].name, "main"))){
        lVarVector.emplace_back(0, "main", "func", false);
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
        if (now_pos <= 0){
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
        if (now_pos <= 0){
            return;
        }
        printf("ret i32 ");
        printRegister(registerStack.top());
        puts("");
        register_num++;
    } else if (words[now_pos].id == 1){
        int reservedMode = checkReserved(words[now_pos].name);
        if (reservedMode != -1){
            reservedFunc(reservedMode);
        } else {
            int lVarVector_pos = getLVar(words[now_pos].name);
            int tmp_pos = now_pos;
            now_pos = get_next();
            if (words[now_pos].id == 12){
                if (lVarVector[lVarVector_pos].isConst){
                    now_pos = -1;
                    return;
                }
                now_pos = get_next();
                Exp();
                if (now_pos <= 0){
                    return;
                }
                printf("store i32 ");
                printRegister(registerStack.top());
                registerStack.pop();
                printf(", i32* %%%d\n", lVarVector[lVarVector_pos].register_order);
            } else{
                now_pos = tmp_pos;
                Exp();
            }
        }
    }
    if (words[now_pos].id == 13){
        now_pos = get_next();
    } else{
        now_pos = 0;
    }
}

void checkFunContent(){
    while (words[now_pos].id != 17){
        if (now_pos <= 0){
            return;
        }
        if (words[now_pos].id == 11){
            constFlag = true;
            now_pos = get_next();
            if (words[now_pos].id == 9){
                now_pos = get_next();
                Decl();
                constFlag = false;
            }
        } else if (words[now_pos].id == 9){
            constFlag = false;
            now_pos = get_next();
            Decl();
        } else{
            Stmt();
        }
    }
    printf("}\n");
}

void Decl(){
    int tmp_pos = 0;
    while (words[now_pos].id != 13){
        if (words[now_pos].id == 1){
            tmp_pos = now_pos;
            if (checkRepeat(tmp_pos)){
                now_pos = -1;
                return;
            }
            now_pos = get_next();
            if (words[now_pos].id == 12){
                storeFlag = true;
                now_pos = get_next();
                Exp();
            }// else
            if (words[now_pos].id == 28){
                lVarVector.emplace_back(++register_num, words[tmp_pos].name, "int", constFlag);
                printf("%%%d = alloca i32\n", register_num);
                if (storeFlag){
                    printf("store i32 ");
                    if (!registerStack.empty()){
                        printRegister(registerStack.top());
                    }
                    printf(", i32* %%%d\n", register_num);
                }
                now_pos = get_next();
            }
        }
        if (now_pos <= 0){
            return;
        }
    }
    lVarVector.emplace_back(++register_num, words[tmp_pos].name, "int", constFlag);
    printf("%%%d = alloca i32\n", register_num);
    if (storeFlag){
        printf("store i32 ");
        if (!registerStack.empty()){
            printRegister(registerStack.top());
        }
        printf(", i32* %%%d\n", register_num);
    }
    now_pos = get_next();
}

void reservedFunc(int reservedMode){
    now_pos = get_next();
    switch (reservedMode) {
        case 0:
        case 1:
            if (words[now_pos].id == 14){
                now_pos = get_next();
                //opStack.push('(');
                if (words[now_pos].id == 15){
                    printf("%%%d = call i32 @%s()\n", ++register_num, reserved[reservedMode]);
                    registerStack.push(register_num);
                    now_pos = get_next();
                    return;
                }
            }
            now_pos = -1;
            return;
        case 3:
        case 4:
            if (words[now_pos].id == 14){
                now_pos = get_next();
                opStack.push('(');
                Exp();
                if (now_pos <= 0){
                    return;
                } else{
                    if (words[now_pos].id == 15){
                        printf("call void @%s(i32 ", reserved[reservedMode]);
                        printRegister(registerStack.top());
                        printf(")\n");
                        now_pos = get_next();
                        return;
                    }
                }
            }
            now_pos = -1;
            return;
        default:
            now_pos = -1;
            return;
    }
}

void Exp(){
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
            if (last_word_key != '0' && last_word_key != '1'){
                if (words[now_pos].id == 20){
                    last_word_key = '+';
                    opStack.push('#');
                } else{
                    last_word_key = '-';
                    opStack.push('$');
                }
            } else{
                while (!opStack.empty() && opStack.top() != '('){
                    if (now_pos <= 0){
                        break;
                    }
                    calculate();
                }
                if (now_pos <= 0){
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
            if (now_pos <= 0){
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
            if (last_word_key == '0' || last_word_key == '1'){
                now_pos = 0;
                break;
            }
            registerStack.push(0 - words[now_pos].num);
            //printf("%d\n", registerStack.top());
            last_word_key = '0';
        } else if (words[now_pos].id == 1){
            int reservedMode = checkReserved(words[now_pos].name);
            if (reservedMode != -1){
                reservedFunc(reservedMode);
                return;
            }
            bool found = false;
            for (int i = lVarVector.size() - 1; i >= 0 ; --i) {
                if (strcmp(lVarVector[i].name, words[now_pos].name) == 0){
                    if (constFlag && !lVarVector[i].isConst){
                        now_pos = -1;
                        return;
                    }
                    found = true;
                    registerStack.push(lVarVector[i].register_order);
                    break;
                }
            }
            if (!found){
                now_pos = -1;
                break;
            }
            last_word_key = '1';
        } else{
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
    if (now_pos <= 0){
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

void printRegister(int n) {
    if (n > 0) {
        printf("%%");
    }
    printf("%d", abs(n));
}