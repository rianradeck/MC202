#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ix, iy -> inicio
// fx, fy -> final 
typedef struct Passenger{
	char name[20];
	double rating, profit;
	int ix, iy, fx, fy;
} Passenger;

// n -> numero de elementos na heap
// sz -> tamanho maximo da heap
typedef struct PQ{
	Passenger *p;
	int n, sz;
} PQ;

typedef struct PQ * priority_queue;

// Crio e retorno um passageiro com os parametros inicializados
Passenger new_passanger(char *name, double rating, int ix, int iy, int fx, int fy){
	Passenger passenger;
	strcpy(passenger.name, name);
	passenger.rating = rating;
	passenger.ix = ix;
	passenger.iy = iy;
	passenger.fx = fx;
	passenger.fy = fy;
	return passenger;
}

priority_queue initialize(int sz){
	priority_queue pq = malloc(sizeof (PQ));
	pq->p = malloc(sz * sizeof (Passenger));
	pq->n = 0;
	pq->sz = sz;
	return pq;
}

void swap(Passenger *a, Passenger *b){
	Passenger c = *a;
	*a = *b;
	*b = c;
}

#define parent(i) ((i - 1) / 2)
#define ls(i) (2 * i + 1)
#define rs(i) (2 * i + 2)

// Sobe na heap a partir da posicao K balanceando (troca ele peo pai, se ele for maior que o pai)
void up_heap(priority_queue pq, int k){
	if(k > 0 && pq->p[parent(k)].rating < pq->p[k].rating){
		swap(&pq->p[parent(k)], &pq->p[k]);
		up_heap(pq, parent(k));
	}
}

// Desce na heap a partir da posicao K balanceando (troca ele pelo maior filho recursivamente, se o maior filho for maior que ele)
void down_heap(priority_queue pq, int k){
	int max_rating = 0;
	if(ls(k) < pq->n){
		max_rating = ls(k);
		if(rs(k) < pq->n && pq->p[ls(k)].rating < pq->p[rs(k)].rating)
			max_rating = rs(k);
		if(pq->p[k].rating < pq->p[max_rating].rating){
			swap(&pq->p[k], &pq->p[max_rating]);
			down_heap(pq, max_rating);
		}
	}
}

// Coloco na ultima posicao da heap e balanceio
void insert(priority_queue pq, Passenger passenger){
	pq->p[pq->n] = passenger;
	pq->n++;
	up_heap(pq, pq->n - 1);
}

// Coloco um cara vazio no topo da heap, troco com o ultimo e balanceio a arvore. Retorno o topo (antes de coloca-lo vazio)
// Resumindo, retorno e removo o maior da fila
Passenger pull_max(priority_queue pq){
	Passenger passenger = pq->p[0];
	pq->p[0] = new_passanger("", 0, 0, 0, 0, 0);
	swap(&pq->p[0], &pq->p[pq->n - 1]);
	pq->n--;
	down_heap(pq, 0);
	return passenger;
}

// removo o maior da fila, de maneira semelhante a ultima funcao, porem nao retorno nada
void pop_max(priority_queue pq){
	pq->p[0] = new_passanger("", 0, 0, 0, 0, 0);
	swap(&pq->p[0], &pq->p[pq->n - 1]);
	pq->n--;
	down_heap(pq, 0);
}

int main(){
	// Crio uma fila de prioridade vazia com tamanho maximo de 550
	priority_queue pq = initialize(550);

	// x e y sao a posicao atual do motorista, creio que as outras variaveis sao autoexplicativas
	int x = 0, y = 0, total_dist = 0;
	double total_brute_profit = 0, total_expenses = 57;
	// Crio a variavel traveler para monitorar o passageiro que esta viajando atualmente, inicia "zerado"
	Passenger traveler = new_passanger("", 0, 0, 0, 0, 0);

	char op;
	while(scanf("%c", &op)){
		if(op == 'T'){
			// Imprimo o resultado e libero a memoria alocada dinamicamente
			printf("\nJornada finalizada. Aqui esta o seu rendimento de hoje\nKm total: %d\nRendimento bruto: %.2lf\nDespesas: %.2lf\nRendimento liquido: %.2lf\n", total_dist, total_brute_profit, total_expenses, (3.0*total_brute_profit/4.0) - total_expenses);
			free(pq->p);
			free(pq);
			exit(0);
		}
		if(op == 'A'){
			// se a operacao eh de adicionar alguem na fila, crio um passageiro com os parametros fornecidos e o coloco na fila
			char name[20];
			double rating;
			int ix, iy, fx, fy;
			scanf("%s %lf %d %d %d %d", name, &rating, &ix, &iy, &fx, &fy);
			insert(pq, new_passanger(name, rating, ix, iy, fx, fy));

			// depois vejo se eu nao estou levando ninguem na viagem, se nao comeco coloco o viajante como sendo esse passageiro e ja o removo da fila
			// se ja estiver levando alguem, nao faco nada, ja que ele vai esperar sua vez
			if(strcmp(traveler.name, "") == 0){
				traveler = new_passanger(name, rating, ix, iy, fx, fy);
				for(int i = 0;i <= pq->n;i++){
					// para remover da fila procuro o no que tem o nome do viajante em O(15*n) -> O(n)
					// depois disso coloco um rating "infinito" nele e balanceio a heap, assim o cara que desejo remover estara no topo
					// apos isso somente removo com a funcao pop_max
					if(strcmp(pq->p[i].name, traveler.name) == 0){
						pq->p[i].rating = 1000;
						up_heap(pq, i);
						pop_max(pq);
						break;
					}
				}
			}

			printf("Cliente %s foi adicionado(a)\n", name);
		}
		if(op == 'F'){
			Passenger passenger = traveler;

			// Faco os calculos com o passageiro atual e computo que sua viagem foi finalizada
			// Lembre-se que o viajante ja nao esta na minha fila de prioridade, pois todo viajante eh removido da fila
			// seja no if da linha 135, que quando estou em espera de um passageiro, ou na linha 164 quando falo que o viajante eh o topo
			// removendo ele ao mesmo tempo com a funcao pull_max
			int passanger_dist = abs(passenger.ix - x) + abs(passenger.iy - y);
			int trip_dist = abs(passenger.fx - passenger.ix) + abs(passenger.fy - passenger.iy);
			total_expenses += (trip_dist + passanger_dist) * 0.4104;
			total_brute_profit += 1.4*trip_dist;
			x = passenger.fx, y = passenger.fy;
			total_dist += trip_dist + passanger_dist;
			printf("A corrida de %s foi finalizada\n", passenger.name);

			// se minha fila ainda tiver gente o viajante agora eh o topo da fila
			// senao fico em espera novamente, sendo o meu viajante o "vazio"
			if(pq->n > 0)
				traveler = pull_max(pq);
			else if(pq->n == 0)
				traveler = new_passanger("", 0, 0, 0, 0, 0);
		}
		if(op == 'C'){
			// se alguem cancela a corrida, o removo da fila de maneira similar ao for da linha 131
			char name[20];
			scanf("%s", name);
			printf("%s cancelou a corrida\n", name);
			total_brute_profit += 7;
			for(int i = 0;i <= pq->n;i++){
				if(strcmp(pq->p[i].name, name) == 0){
					pq->p[i].rating = 1000;
					up_heap(pq, i);
					pop_max(pq);
					break;
				}
			}
		}
	}
}