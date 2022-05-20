#include <stdio.h>
#include <stdbool.h>
#include "professor_carlos.h"

// Retorna o tamanho da string a
int tamanho(char *a){
	int len;
	for(len = 0; a[len]; ++len);
	return len;
}

// Retorna 1 se  "a" é menor que "b" lexicograficamente, 0 se são iguais ou -1 se "b" menor que "a";
int lexicograficamente_menor(char a[], char b[]){
	int lena = tamanho(a), lenb = tamanho(b);
	for(int i = 0;i < (lena < lenb ? lena : lenb);i++)
		if(a[i] < b[i])
			return 1;
		else if(a[i] > b[i])
			return -1;
	return (lena < lenb ? 1 : (lena == lenb ? 0 : -1));
}

// Retorna 1 se a é mais novo que b ou 0 cc.
bool mais_novo(Aluno a, Aluno b){
	if(a.nascimento.ano > b.nascimento.ano)
		return 1;
	else if(a.nascimento.ano < b.nascimento.ano)
		return 0;
	else if(a.nascimento.mes > b.nascimento.mes)
		return 1;
	else if(a.nascimento.mes < b.nascimento.mes)
		return 0;
	else if(a.nascimento.dia > b.nascimento.dia)
		return 1;
	else if(a.nascimento.dia < b.nascimento.dia)
		return 0;
	else if(lexicograficamente_menor(a.nome, b.nome) == 1) // se nome de a menor que nome de b retorno que a é menor que b
		return 1;
	else if(lexicograficamente_menor(a.nome, b.nome) == -1) // se o nome de b for menor retorno b
		return 0;
	else if(lexicograficamente_menor(a.sobrenome, b.sobrenome) == 1)
		return 1;
	return 0;
}

bool mais_velho(Aluno a, Aluno b){
	if(a.nascimento.ano > b.nascimento.ano)
		return 0;
	else if(a.nascimento.ano < b.nascimento.ano)
		return 1;
	else if(a.nascimento.mes > b.nascimento.mes)
		return 0;
	else if(a.nascimento.mes < b.nascimento.mes)
		return 1;
	else if(a.nascimento.dia > b.nascimento.dia)
		return 0;
	else if(a.nascimento.dia < b.nascimento.dia)
		return 1;
	else if(lexicograficamente_menor(a.nome, b.nome) == 1) // se nome de a menor que nome de b retorno que a é menor que b
		return 1;
	else if(lexicograficamente_menor(a.nome, b.nome) == -1) // se o nome de b for menor retorno b
		return 0;
	else if(lexicograficamente_menor(a.sobrenome, b.sobrenome) == 1)
		return 1;
	return 0;
}

Aluno procura_novo_na_turma(Turma t[], int qtd_turmas, int j){
	Aluno novo = t[j].alunos[0]; // novo é a minha resposta, ele começa sendo o primeiro aluno da turma j
	for(int i = 0;i < t[j].qtd;i++) // itero por todos os alunos da turma j
		if(mais_novo(t[j].alunos[i], novo)) // se o iésimo aluno da turma j (t[j].alunos[i]) for mais novo que o "novo", novo vai ser agora esse iesimo aluno.
			novo = t[j].alunos[i];
	return novo;
}

// Essa função é basimcamente a mesma de cima só que teremos que tambem iterar por todas as turmas
// Portanto a maneira de pensar é basicamente a mesma, mas com algumas iterações a mais
Aluno procura_novo_todas_turmas(Turma t[], int qtd_turmas){
	Aluno novo = t[0].alunos[0];
	for(int j = 0;j < qtd_turmas;j++) // j-ésima turma
		for(int i = 0;i < t[j].qtd;i++) // iésimo aluno
			if(mais_novo(t[j].alunos[i], novo)) // mesma checagem da ultima funcao
				novo = t[j].alunos[i];
	return novo;
}

// Essa função é analoga a "procura_novo_na_turma", a maneira de pensar é a mesma, somente a comparação que é invertida
Aluno procura_velho_na_turma(Turma t[], int qtd_turmas, int j){
	// puts("ALO");
	Aluno velho = t[j].alunos[0];
	for(int i = 0;i < t[j].qtd;i++){
		// printf("Vendo o %s\n", t[j].alunos[i].nome);
		if(mais_velho(t[j].alunos[i], velho))
			velho = t[j].alunos[i];
	}
	return velho;
}

// Essa função é analoga a "procura_novo_todas_turmas", a maneira de pensar é a mesma, somente a comparação que é invertida
Aluno procura_velho_todas_turmas(Turma t[], int qtd_turmas){
	Aluno velho = t[0].alunos[0];
	for(int j = 0;j < qtd_turmas;j++)
		for(int i = 0;i < t[j].qtd;i++)
			if(mais_velho(t[j].alunos[i], velho))
				velho = t[j].alunos[i];
	return velho;
}

// Retorna 1 se a é uma substring de b ou 0 cc.
int compara(char *a, char *b){
	int lena = tamanho(a); // tamanho de a
	int lenb = tamanho(b); // tamanho de b
	if(lenb < lena)
		return 0;
	bool subst = 1;
	int ans = 0;
	for(int i = 0;i < lenb - lena + 1;i++){ // Vou comparar a com b comecando em i
		subst = 1;
		for(int j = i;j < lena + i;j++)
			if(b[j] != a[j - i])
				subst = 0;
		ans += subst;
	} 
	return (ans > 0 ? 1 : 0);
}

// Vamos iterar por todos os alunos de todas as turmas e verificar se a string aparece em seu nome ou sobrenome
int conta_substrings(Turma t[], int qtd_turmas, char *padrao){
	int ans = 0;
	for(int j = 0;j < qtd_turmas;j++) // j-ésima turma
		for(int i = 0;i < t[j].qtd;i++) // iésimo aluno
			ans += compara(padrao, t[j].alunos[i].nome);
	return ans;
}

int add_aluno(Turma t[], Aluno A, int j){
	t[j].alunos[t[j].qtd] = A;
	return ++t[j].qtd;
}

int remove_aluno(Turma t[], int j){
	return --t[j].qtd;
}
