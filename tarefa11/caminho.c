#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N 1000
#define dist(I, J) (sqrt((x[I] - x[J]) * (x[I] - x[J]) + (y[I] - y[J]) * (y[I] - y[J])))

// Dfs para marcar os caras da componente do 0, que eh o inicio da jornada
void dfs(int v, int n, int pro[], int **adj){
	pro[v] = 1;
	for(int i = 0;i < n;i++)
		if(adj[v][i] && !pro[i])
			dfs(i, n, pro, adj);
}

int main(){
	double x[N], y[N];
	int **adj; // Matriz de adjacencia
	adj = malloc(N * sizeof (int *));
	for(int i = 0;i < N;i++)
		adj[i] = malloc(N * sizeof (int));
	int pro[N]; // Vetor auxiliar a minha dfs, para marcar os caras que ja visitei (zero ele antes de qualquer chamada)
	scanf("%lf %lf", &x[0], &y[0]);

	int lugia[N], lugia_cnt = 0, n = 1;
	char s[15];
	while(scanf("%lf %lf %s", &x[n], &y[n], s) > 0){
		if(strcmp(s, "Lugia") == 0)
			lugia[lugia_cnt] = n, lugia_cnt++; // guardo quais as paradas que tem lugias
		n++;
	}

	int bg = 0, nd = 1e9; // Limite de busca da resposta, vou chutar a maior distancia e ir diminuindo enquanto eu conseguir 
	while(bg < nd){
		int mid = (bg + nd) >> 1;

		// zero o meu grafo e construo ele novamente, ligo dois pontos se a distancia deles eh menor que mid
		for(int i = 0;i < n;i++)
			for(int j = 0;j < n;j++)
				adj[i][j] = 0;
		for(int i = 0;i < n;i++)
			for(int j = i + 1;j < n;j++)
				if(dist(i, j) <= mid)
					adj[i][j] = adj[j][i] = 1;

		// zero o vetor processado e rodo a dfs marcando a componente do 0
		memset(pro, 0, sizeof pro);
		dfs(0, n, pro, adj);

		int ok = 0;
		for(int i = 0;i < lugia_cnt;i++)
			if(pro[lugia[i]])
				ok = 1; // se algum lugia foi marcado significa que consigo chegar em algum lugia com tamanho no max mid entre duas paradas
		
		if(ok)
			nd = mid;
		else
			bg = mid + 1;
	}
	printf("%d\n", nd);

	for(int i = 0;i < N;i++)
		free(adj[i]);
	free(adj);
}