#include"getword.h"
int main(){
	getsym();
	for (int i = 1; i <= words_len; i++){
		if (words[i].id == 1) printf("%s(%s) ",id_map[words[i].id],words[i].name);
		else if (words[i].id == 2) printf("%s(%lld) ",id_map[words[i].id],words[i].num);
		else printf("%s ",id_map[words[i].id]);
	}
	return 0;
}
