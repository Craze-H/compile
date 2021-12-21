#include "register.h"
void BlockItem();
bool funcRead();
void Exp();
void Stmt();
void calculate();
void printRegister(int n);
void Decl();
void reservedFunc(int reservedMode);
void Cond();
void LOrExp();
void LAndExp();
void EqExp();
void RelExp();
void GlobalExp();
void constCalculate();
bool CompUnit();

int now_pos;
int lPar_num = 0;
int lBrace_num = 0;

std::vector<int> numberVec;
std::vector<char> opVec;

int main(){
#ifdef LOCAL
    freopen("in.txt", "r", stdin);
    setbuf(stdout, nullptr);
#endif
	getSym();
    typeStack.emplace_back(0);
    /*for (int i = 1; i <= words_len; i++){
        if (words[i].id == 1) printf("%d %s(%s) \n", i, id_map[words[i].id],words[i].name);
        else if (words[i].id == 2) printf("%d %s(%lld) \n", i, id_map[words[i].id],words[i].num);
        else printf("%d %s \n", i, id_map[words[i].id]);
    }*/
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
	if(CompUnit()){
		return 0;
	}
	else {
		return -2;
	}
}

void GlobalExp(){
    char last_ch = 0;
    while (true){
        if (words[now_pos].id == 14){
            lPar_num++;
            last_ch = '(';
            opVec.emplace_back('(');
        } else if (words[now_pos].id == 15){
            if (lPar_num > 0){
                lPar_num--;
                last_ch = '0';
                while (!opVec.empty() && opVec.back() != '('){
                    constCalculate();
                }
                opVec.pop_back();
            } else{
                while (!opVec.empty()){
                    constCalculate();
                }
                break;
            }
        } else if (words[now_pos].id == 20 || words[now_pos].id == 21){
            if (last_ch != '0'){
                if (words[now_pos].id == 20){
                    last_ch = '+';
                    opVec.emplace_back('#');
                } else{
                    last_ch = '-';
                    opVec.emplace_back('$');
                }
            } else{
                while (!opVec.empty() && opVec.back() != '('){
                    if (now_pos <= 0){
                        break;
                    }
                    constCalculate();
                }
                if (now_pos <= 0){
                    break;
                }
                if (words[now_pos].id == 20){
                    last_ch = '+';
                    opVec.emplace_back('+');
                } else{
                    last_ch = '-';
                    opVec.emplace_back('-');
                }
            }
        } else if (words[now_pos].id > 21 && words[now_pos].id < 25){
            while (!opVec.empty() && opVec.back() != '(' && opVec.back() != '+' && opVec.back() != '-'){
                if (now_pos <= 0){
                    break;
                }
                constCalculate();
            }
            if (now_pos <= 0){
                break;
            }
            switch (words[now_pos].id) {
                case 22:
                    last_ch = '*';
                    opVec.emplace_back('*');
                    break;
                case 23:
                    last_ch = '/';
                    opVec.emplace_back('/');
                    break;
                case 24:
                    last_ch = '%';
                    opVec.emplace_back('%');
                    break;
            }
        } else if(words[now_pos].id == 2){
            if (last_ch == '0'){
                now_pos = -4;
                break;
            }
            numberVec.emplace_back(words[now_pos].num);
            last_ch = '0';
        } else if (words[now_pos].id == 1){
            int reservedMode = checkReserved(words[now_pos].name);
            if (reservedMode != -1){
                now_pos = -4;
                return;
            }
            bool found = false;
            for (int i = lVarVector.size() - 2; i >= 0 ; --i) {
                if (strcmp(lVarVector[i].name, words[now_pos].name) == 0){
                    if (!lVarVector[i].isConst){
                        now_pos = -1;
                        return;
                    }
                    found = true;
                    numberVec.emplace_back(lVarVector[i].constValue);
                    break;
                }
            }
            if (!found){
                now_pos = -1;
                break;
            }
            last_ch = '0';
        } else{
            if (lPar_num != 0){
                now_pos = -4;
                break;
            }
            while (!opVec.empty()){
                constCalculate();
            }
            lVarVector.back().constValue = numberVec.back();
            break;
        }
        now_pos = get_next();
    }
}

void constCalculate(){
    char op = opVec.back();
    opVec.pop_back();
    if (numberVec.empty()){
        now_pos = -4;
    } else{
        if (op == '#'){
            return;
        } else if (op == '$'){
            numberVec.back() = -numberVec.back();
        } else{
            if (numberVec.size() < 2){
                now_pos = -4;
                return;
            } else{
                int second = numberVec.back();
                numberVec.pop_back();
                int first = numberVec.back();
                numberVec.pop_back();
                switch (op) {
                    case '+':
                        numberVec.emplace_back(first + second);
                        break;
                    case '-':
                        numberVec.emplace_back(first - second);
                        break;
                    case '*':
                        numberVec.emplace_back(first * second);
                        break;
                    case '/':
                        numberVec.emplace_back(first / second);
                        break;
                    default:
                        numberVec.emplace_back(first % second);
                }
            }
        }
    }
}

bool CompUnit(){
    while (true){
        if (words[now_pos].id == 11){
            constFlag = true;
            now_pos = get_next();
        }
        if (words[now_pos].id == 9){
            now_pos = get_next();
            while (true){
                if (words[now_pos].id == 1){
                    if (checkRepeat(now_pos)){
                        return false;
                    }
                    lVarVector.emplace_back(0, words[now_pos].name, "int", constFlag);
                    lVarVector.back().isGlobal = true;
                    now_pos = get_next();
                    if (words[now_pos].id == 14){
                        localFlag = false;
                        break;
                    }
                    if (words[now_pos].id == 12){
                        now_pos = get_next();
                        GlobalExp();
                        if (now_pos <= 0){
                            return false;
                        }
                    }
                    if (words[now_pos].id == 13){
                        printf("@%s = dso_local global i32 %d\n", lVarVector.back().name, lVarVector.back().constValue);
                        now_pos = get_next();
                        break;
                    } else if (words[now_pos].id == 28){
                        printf("@%s = dso_local global i32 %d\n", lVarVector.back().name, lVarVector.back().constValue);
                        now_pos = get_next();
                    } else{
                        now_pos = -4;
                        return false;
                    }
                }
            }
            if (words[now_pos].id == 14){
                localFlag = false;
                break;
            }
        }
        constFlag = false;
    }
    constFlag = false;
    if (!funcRead()) {
        return now_pos - 1;
    }
    if (now_pos == words_len){
        printf("}\n");
    }
    return true;
}

bool funcRead(){
    if(!(strcmp(words[now_pos - 1].name, "main"))){
        printf("define dso_local i32 @%s(){\n", words[now_pos - 1].name);
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
        lBrace_num++;
        BlockItem();
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
    if (words[now_pos].id == 16){
        lBrace_num++;
        now_pos = get_next();
        BlockItem();
        if (now_pos <= 0){
            return;
        }
    } else if (words[now_pos].id == 3){
        now_pos = get_next();
        if (words[now_pos].id == 14){
            now_pos = get_next();
            Cond();
            if (now_pos <= 0){
                return;
            }
            if (words[now_pos].id == 15){
                /*if (registerStack.top() > 0 && typeStack[registerStack.top()] == 32){
                    printf("%%%d = zext i32 ", ++register_num);
                    printRegister(registerStack.top());
                    registerStack.pop();
                    registerStack.push(register_num);
                    printf(" to i1\n");
                    typeStack.emplace_back(1);
                }*/
                judgeStack.emplace_back(++judge_num);
                printf("br i1 %%%d, label %%%d, label %%else%d\n\n", registerStack.top(), ++register_num, judgeStack.back());
                typeStack.emplace_back(0);
                printf("%d:\n", register_num);
                now_pos = get_next();
                Stmt();
                printf("br label %%if%d\n\n", judgeStack.back());
                int else_flag = register_num;
                printf("else%d:\n", judgeStack.back());
                if (words[now_pos].id == 4){
                    now_pos = get_next();
                    Stmt();
                }
                printf("br label %%if%d\n\n", judgeStack.back());
                printf("if%d:\n", judgeStack.back());
                judgeStack.pop_back();
            } else{
                now_pos = -2;
            }
        }
    } else{
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
                    if (lVarVector[lVarVector_pos].register_order == 0) {
                        printf(", i32* @%s\n", lVarVector[lVarVector_pos].name);
                    } else{
                        printf(", i32* %%%d\n", lVarVector[lVarVector_pos].register_order);
                    }

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

}

void Cond(){
    LOrExp();
    if (registerStack.top() > 0 && typeStack[registerStack.top()] == 32){
        printf("%%%d = icmp eq i32 1, ", ++register_num);
        typeStack.emplace_back(1);
        printRegister(registerStack.top());
        puts("");
        registerStack.pop();
        registerStack.push(register_num);
    }
}

void LOrExp(){
    LAndExp();
    if (now_pos <= 0){
        return;
    }
    if (words[now_pos].id == 31){
        now_pos = get_next();
        LOrExp();
        printf("%%%d = or i1 ", ++register_num);
        printRegister(registerStack.top());
        registerStack.pop();
        printf(", ");
        printRegister(registerStack.top());
        registerStack.pop();
        puts("");
        registerStack.push(register_num);
        typeStack.emplace_back(1);
    }
}

void LAndExp(){
    EqExp();
    if (now_pos <= 0){
        return;
    }
    if (words[now_pos].id == 30){
        now_pos = get_next();
        LAndExp();
        printf("%%%d = and i1 ", ++register_num);
        printRegister(registerStack.top());
        registerStack.pop();
        printf(", ");
        printRegister(registerStack.top());
        registerStack.pop();
        puts("");
        registerStack.push(register_num);
        typeStack.emplace_back(1);
    }
}

void EqExp(){
    RelExp();
    if (now_pos <= 0){
        return;
    }
    if (words[now_pos].id == 29 || words[now_pos].id == 34){
        int tmp_pos = now_pos;
        now_pos = get_next();
        EqExp();
        printf("%%%d = icmp ", ++register_num);
        if (words[tmp_pos].id == 29){
            printf("eq");
        } else{
            printf("ne");
        }
        printf(" i32 ");
        printRegister(registerStack.top());
        registerStack.pop();
        printf(", ");
        printRegister(registerStack.top());
        registerStack.pop();
        puts("");
        registerStack.push(register_num);
        typeStack.emplace_back(1);
    }
}

void RelExp(){
    Exp();
    if (now_pos <= 0){
        return;
    }
    int tmp_pos = now_pos;
    if (words[now_pos].id != 25 && words[now_pos].id != 26 &&words[now_pos].id != 32 && words[now_pos].id != 33){
        return;
    }
    now_pos = get_next();
    EqExp();
    printf("%%%d = icmp ", ++register_num);
    switch (words[tmp_pos].id) {
        case 25:
            printf("slt");
            break;
        case 26:
            printf("sgt");
            break;
        case 32:
            printf("sle");
            break;
        case 33:
            printf("sge");
            break;
        default:
            return;
    }
    printf(" i32 ");
    printRegister(registerStack.top());
    registerStack.pop();
    printf(", ");
    printRegister(registerStack.top());
    registerStack.pop();
    puts("");
    registerStack.push(register_num);
    typeStack.emplace_back(1);
}

void BlockItem(){
    lVarVector.emplace_back(0, "{", "{", false);
    while (lBrace_num != 0){
        if (now_pos <= 0){
            return;
        }
        if (words[now_pos].id == 17){
            lBrace_num--;
            while (strcmp(lVarVector.back().name, "{") != 0){
                lVarVector.pop_back();
            }
            lVarVector.pop_back();
            now_pos = get_next();
            break;
        } else if (words[now_pos].id == 11){
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
                typeStack.emplace_back(32);
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
    typeStack.emplace_back(32);
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
                    typeStack.emplace_back(32);
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
                lPar_num++;
                Exp();
                if (now_pos <= 0){
                    return;
                } /*else{
                    if (words[now_pos].id == 15){*/
                        printf("call void @%s(i32 ", reserved[reservedMode]);
                        printRegister(registerStack.top());
                        printf(")\n");
                        //now_pos = get_next();
                        return;
                    /*}
                }*/
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
    while (true){
        if (words[now_pos].id == 27){
            if (last_word_key == '0' || last_word_key == '1'){
                now_pos = -2;
                return;
            }
            last_word_key = '!';
            opStack.push('!');
        } else if (words[now_pos].id == 14){
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
                while (!opStack.empty()){
                    calculate();
                }
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
            while (!opStack.empty() && opStack.top() != '(' && opStack.top() != '+' && opStack.top() != '-' && opStack.top() != '!'){
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
                    if ((constFlag || localFlag) && !lVarVector[i].isConst){
                        now_pos = -1;
                        return;
                    }
                    found = true;
                    if (lVarVector[i].isGlobal){
                        printf("%%%d = load i32, i32* @%s\n", ++register_num, lVarVector[i].name);
                    } else{
                        printf("%%%d = load i32, i32* %%%d\n", ++register_num, lVarVector[i].register_order);
                    }
                    registerStack.push(register_num);
                    typeStack.emplace_back(32);
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
                //printf("!!!%c, %d\n", opStack.top(), opStack.size());
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
    //printf("!!!%c, %d\n", op, opStack.size());
    opStack.pop();
    if (!registerStack.empty()){
        if (op == '#' || op == '$' || op == '!'){
            printf("%%%d = ", ++register_num);
            if (op == '#'){
                printf("add");
                printf(" i32 0, ");
                typeStack.emplace_back(32);
            } else if (op == '$'){
                printf("sub");
                printf(" i32 0, ");
                typeStack.emplace_back(32);
            } else{
                printf("icmp eq");
                printf(" i32 0, ");
                typeStack.emplace_back(1);
                /*if (!typeStack.empty()){
                    printf("??%d %d\n", registerStack.top(), typeStack[10]);
                }*/
            }
            printRegister(registerStack.top());
            puts("");
            registerStack.pop();
            registerStack.push(register_num);
            if (op == '!'){
                if (registerStack.top() > 0 && typeStack[registerStack.top()] == 1){
                    printf("%%%d = zext i1 ", ++register_num);
                    printRegister(registerStack.top());
                    registerStack.pop();
                    registerStack.push(register_num);
                    printf(" to i32\n");
                    typeStack.emplace_back(32);
                }
            }
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
                typeStack.emplace_back(32);
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