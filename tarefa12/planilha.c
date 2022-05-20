#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 512

typedef struct Vertex{
	int adj[N], sign[N], sz, val;
} Vertex;

Vertex g[N * N];

// Verifica os ciclos, se eu cheguei em um cara que ja passei no caminho atual retorno q eh um ciclo
int path[N * N], path_sz;
int check_cycle(int v){
	int ret = 0;
	path[path_sz++] = v;
	for(int i = 0;i < g[v].sz;i++){
		int u = g[v].adj[i];
		for(int j = 0;j < path_sz;j++)
			if(path[j] == u)
				return 1;
		ret |= check_cycle(u);
	}
	path_sz--;
	return ret;
}

// Pego recursivamente os valores dos meus dependentes
// Como para cada vertice eu salvo os seus dependentes e os sinais deles fica bem simples
int get_value(int v){
	int ret = g[v].val;
	for(int i = 0;i < g[v].sz;i++){
		int u = g[v].adj[i];
		ret += g[v].sign[i] * get_value(u);
	}
	return ret;
}

char name[256];
char cell[N][N][256];

int main(){
	// Inicializo os vertices como n tendo nenhum dependente e o valor deles mesmo sendo 0
	for(int i = 0;i < N;i++)
		g[i].sz = g[i].val = 0;
	int n, m;

	scanf("%s %d %d", name, &m, &n);

	FILE *f = fopen(name, "r");

	int I = 0, J = 0;
	char aux[256];
	// Leio e guardo em cell[i][j] a string que aparece naquela celula
	while(fscanf(f, "%[^\n]%*c", aux) != EOF){
		char s[256];
		int cnt = 0;
		J = 0;
		for(int k = 0;k <= strlen(aux);k++){
			if(aux[k] == ',' || k == strlen(aux)){
				s[cnt] = '\0';
				strcpy(cell[I][J], s);
				cnt = 0;
				J++;
			} else {
				s[cnt++] = aux[k];
			}
		}
		I++;
	}

	fclose(f);

	// Vou construir os vertices
	for(int i = 0;i < n;i++){
		for(int j = 0;j < m;j++){
			// pos eh a posicao do vertice no vetor de vertices
			int pos = m * i + j, open = 0;
			
			// Se a celula nao possui uma expressao eu coloco o valor dela
			if(cell[i][j][0] != '=')
				g[pos].val = atoi(cell[i][j]);
			else{
				// se for uma expressao eu vou resolver essa expressao
				for(int k = 2;k < strlen(cell[i][j]);k += 2){
					#define s cell[i][j]

					// Se o cara atual da expressao eh outra celula coloco que eu dependo dela
					if('A' <= s[k] && s[k]  <= 'Z'){
						g[pos].sign[g[pos].sz] = (s[k - 2] == '-' ? -1 : 1);
						
						char aux_s[10];
						int aux_cnt = 1;
						while('0' <= s[k + aux_cnt] && s[k + aux_cnt] <= '9')
							aux_s[aux_cnt - 1] = s[k + aux_cnt], aux_cnt++;
						aux_s[aux_cnt - 1] = '\0';
						g[pos].adj[g[pos].sz] = (s[k] - 'A') + m * (atoi(aux_s) - 1);
						g[pos].sz++;
						// Vou para o proximo cara da expressao
						while(s[k + 1] != ' ')
							k++;
					} else if(s[k] == '-' && s[k + 2] == '('){
						// Se eu tenho um - antes de um parenteses, todos os caras dentro desse parenteses
						// Invertem o sinal, mas somente os que estao dentro desse parentes, se tiver um elemento
						// que esta dentro de outro parenteses, que esta dentro do meu, nao faco nada com esse elemento
						// Em geral, troco de sinal todas as subexpressoes e os elementos imediatamente dentro desse parenteses
						int aux = open + 1;
						for(int l = k + 4;aux > open;l += 2){
							if(s[l] == ')'){
								aux--;
							}
							else if('A' <= s[l] && s[l] <= 'Z'){
								if(s[l - 2] == '(' && aux == open + 1){
									s[strlen(s) + 2] = '\0';
									for(int h = strlen(s) - 1;h >= l;h--)
										s[h + 2] = s[h];
									s[l] = '-', s[l + 1] = ' ';
									l += 2;
								};
								while(s[l + 1] != ' ')
									l++;
							} else if(s[l] == '('){
								if(aux == open + 1 && s[l - 2] == '('){
									s[strlen(s) + 2] = '\0';
									for(int h = strlen(s) - 1;h >= l;h--)
										s[h + 2] = s[h];
									s[l] = '-', s[l + 1] = ' ';
									l += 2;

									while(s[l + 1] != ' ')
										l++;
								}
								aux++;
							} else if(s[l] == '+' && aux == open + 1){
								s[l] = '-';
							} else if(s[l] == '-' && aux == open + 1){
								s[l] = '+';
							} else {
								while(s[l + 1] != ' ')
									l++;
							}
						}
					} else if(s[k] == '('){
						open++;
					} else if(s[k] == ')'){
						open--;
					}
					#undef s
				}
			}
		}
	}

	// Processo as querries
	char op, q[20];
	while(scanf(" %c %s", &op, q) != EOF){
		int pos;

		// Calculo a posicao da celula da querry
		char aux_q[10];
		int aux_cnt = 1;
		while('0' <= q[aux_cnt] && q[aux_cnt] <= '9')
			aux_q[aux_cnt - 1] = q[aux_cnt], aux_cnt++;
		aux_q[aux_cnt - 1] = '\0';
		pos = (q[0] - 'A') + m * (atoi(aux_q) - 1);

		if(op == 'S'){
			// Atualizo o valor do meu vertice
			int new_val;
			scanf("%d", &new_val);
			printf("%s: %d -> %d\n", q, g[pos].val, new_val);
			g[pos].val = new_val;
		} else {
			// Vejo se nao tem ciclo e pego a resposta
			if(check_cycle(pos))
				printf("%s: #ERRO#\n", q);
			else
				printf("%s: %d\n", q, get_value(pos));
		}
	}
}