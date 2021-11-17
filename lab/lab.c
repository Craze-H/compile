#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include <stdbool.h>

char last_ch, now_ch;
char last_token[105], now_token[105];
int now_token_len;
void get_no_blank_char();
int main(){
    while((last_ch = getchar()) != EOF) {
        if(last_ch != ' ' && last_ch != '\n'){
            break;
        }
    }
    now_token[now_token_len++] = last_ch;
}
void get_no_blank_char(){
    now_ch = getchar();
    while (now_ch != ' ' && now_ch != '\n') {
        last_ch = now_ch;
        now_token[now_token_len++] = now_ch;
    }
}