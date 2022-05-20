#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// No da lista ligada
typedef struct No{
    char c[30];
    struct No *nxt;
} No;

typedef struct No * p_no;

p_no lcreate(){
    return NULL;
}

// Destruir a lista ligada
void ldestroy(p_no list){
    if(list != NULL)
        ldestroy(list->nxt);
    free(list);
}

// insiro no inicio da lista e retorno o novo no sentinela
p_no linsert(p_no list, char *s){
    p_no new_node = malloc(sizeof(No));
    strcpy(new_node->c, s);
    new_node->nxt = list;
    return new_node;
}

// procuro um item na lista
int lfind(p_no list, char *s){
	if(list != NULL){
		if(strcmp(s, list->c) == 0)
			return 1;
		return lfind(list->nxt, s);
	}
	return 0;
}

p_no h_table[2000], error_table[2000];

// calculo o hashing de uma determinada string
int h_calc(char *s){
	int h = 0;
	for(int i = 0;i < strlen(s);i++)
		h += ((s[i] - 'a') * 9463) % 2000, h %= 2000;
	return h;
}

// coloco na lista de indice h a string s, onde h eh o hashing de s
void table_insert(char *s){
	int h = h_calc(s);
	h_table[h] = linsert(h_table[h], s);
}

// faco a mesma coisa da ultima funcao so q em outra tabela
void error_table_insert(char *s){
	int h = h_calc(s);
	error_table[h] = linsert(error_table[h], s);
}

// procuro na tabela
int table_find(char *s){
	int h = h_calc(s);
	return lfind(h_table[h], s);
}

// procuro se tem um erro
int error_find(char *s){
	int h = h_calc(s);
	return lfind(error_table[h], s);
}

int main(){
	// inicio as tabelas
	for(int i = 0;i < 2000;i++)
		h_table[i] = lcreate(), error_table[i] = lcreate();

	int n, q;
	scanf("%d %d", &n, &q);
	while(n--){
		char s[30];
		scanf("%s", s);

		// coloco na tabela a palavra sem erros
		table_insert(s);

		char t[30];
		// calculo todas as possibilidades de uma troca de letras e coloco na tabela de erros
		for(int i = 0;i < strlen(s);i++){
			strcpy(t, s);
			for(int k = 0;k < 26;k++){
				t[i] = k + 'a';
				error_table_insert(t);
			}
		}

		// calculo todas as possibilidades de um esquecimento de letra e coloco na tabela de erros
		for(int i = 0;i < strlen(s);i++){
			int t_cnt = 0;
			for(int j = 0;j < strlen(s);j++)
				if(j != i)
					t[t_cnt++] = s[j];
			t[t_cnt] = '\0';
			error_table_insert(t);
		}

		// calculo colocar uma letra nova em todoas as posicoes possiveis e coloco na tabela de erros
		for(int i = 0;i < strlen(s) + 1;i++){
			int t_cnt = 0;
			for(int j = 0;j < strlen(s) + 1;j++){
				if(j < i)
					t[t_cnt++] = s[j];
				else if (j == i)
						t_cnt++;
				else
					t[t_cnt++] = s[j - 1];
				t[t_cnt] = '\0';
			}
			for(int k = 0;k < 26;k++){
				t[i] = (k + 'a');
				error_table_insert(t);
			}
		}
	}

	while(q--){
		char s[30];
		scanf("%s", s);

		if(table_find(s))
			puts("verde");
		else if(error_find(s))
			puts("amarelo");
		else
			puts("vermelho");
	}

	for(int i = 0;i < 2000;i++)
		ldestroy(h_table[i]), ldestroy(error_table[i]);
}