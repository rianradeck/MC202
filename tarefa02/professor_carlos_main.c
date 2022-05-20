// Rian Radeck Santos Costa - 187793
// Tarefa 2 - Professor Carlos - 25/09/2020

#include <stdio.h>
#include "professor_carlos.h"

int main(){
	int n, k;
	scanf("%d %d", &n, &k);

	Turma t[60];
	for(int i = 0;i < n;i++){
		scanf("%d", &t[i].qtd);
		for(int j = 0;j < t[i].qtd;j++)
			scanf("%s %s %d %d %d", t[i].alunos[j].nome, t[i].alunos[j].sobrenome, &t[i].alunos[j].nascimento.dia, &t[i].alunos[j].nascimento.mes, &t[i].alunos[j].nascimento.ano);
	}
	while(k--){
		int op;
		scanf("%d", &op);

		if(op == 1){
			int j;
			scanf("%d", &j);
			printf("%s\n", procura_novo_na_turma(t, n, j).nome);
		}
		if(op == 4){
			printf("%s\n", procura_novo_todas_turmas(t, n).sobrenome);
		}
		if(op == 2){
			int j;
			scanf("%d", &j);
			printf("%s\n", procura_velho_na_turma(t, n, j).sobrenome);
		}
		if(op == 3){
			printf("%s\n", procura_velho_todas_turmas(t, n).nome);
		}
		if(op == 5){
			char s[15];
			scanf("%s", s);
			printf("%d\n", conta_substrings(t, n, s));
		}
		if(op == 6){
			int j;
			Aluno a;
			scanf("%d %s %s %d %d %d", &j, a.nome, a.sobrenome, &a.nascimento.dia, &a.nascimento.mes, &a.nascimento.ano);
			add_aluno(t, a, j);
			printf("%d\n", t[j].qtd);
		}
		if(op == 7){
			int j;
			scanf("%d", &j);
			remove_aluno(t, j);
			printf("%d\n", t[j].qtd);
		}
	}
}