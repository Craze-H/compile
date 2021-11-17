#include"getword.h"
int now_pos;
void returnPrint(){
	printf("define dso_local i32 @%s(){\n", words[now_pos + 1].name);
	printf("\tret i32 %lld\n", words[now_pos + 6].num);
	printf("}");
}
bool returnRead(){
	int tmp_pos = now_pos;
	if(tmp_pos + 9 != words_len){
		return false;
	}
	if(words[tmp_pos].id == 9){
		tmp_pos += 1;
		if(words[tmp_pos].id == 1 && strcmp(words[tmp_pos].name, "main") == 0){
			tmp_pos += 1;
			if(words[tmp_pos].id == 14 && words[tmp_pos + 1].id == 15 && words[tmp_pos + 2].id == 16 && words[tmp_pos + 3].id == 8){
				tmp_pos += 4; 
				if(words[tmp_pos].id == 2 && words[tmp_pos + 1].id == 13 && words[tmp_pos + 2].id == 17){
					returnPrint();
					return true;
				}
			}
		}
	}
}
int main(){
	getsym();
	now_pos = 1;
	/*for (int i = 1; i <= words_len; i++){
		if (words[i].id == 1) printf("%s(%s) ",id_map[words[i].id],words[i].name);
		else if (words[i].id == 2) printf("%s(%lld) ",id_map[words[i].id],words[i].num);
		else printf("%s ",id_map[words[i].id]);
	}*/
	if(returnRead()){
		return 0;
	}
	else {
		return -1;
	}
}
