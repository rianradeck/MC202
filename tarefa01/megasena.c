#include <stdio.h>
#include <string.h>

int main(){
	int m, hits[1010];
	_Bool g[1010][7][11], ans[61];
	double n;
	memset(ans, 0, sizeof ans);
	memset(hits, 0, sizeof hits);

	scanf("%d %lf", &m, &n);
	for(int i = 1;i <= m;i++)
		for(int l = 1;l <= 6;l++)
			for(int c = 1, temp;c <= 10;c++)
				scanf("%d", &temp), g[i][l][c] = temp;
	for(int i = 1, x;i <= 6;i++)
		scanf("%d", &x), ans[x] = 1;

	for(int i = 1;i <= m;i++)
		for(int l = 1;l <= 6;l++)
			for(int c = 1;c <= 10;c++)
				if(g[i][l][c] && ans[(l - 1) * 10 + c])
					hits[i]++;

	int six = 0, five = 0, four = 0;
	for(int i = 1;i <= m;i++)
		if(hits[i] == 6)
			six++;
		else if(hits[i] == 5)
			five++;
		else if(hits[i] == 4)
			four++;

	double sixcut = 0.62 * n / six;
	double fivecut= 0.19 * n / five;
	double fourcut= 0.19 * n / four;

	for(int i = 1;i <= m;i++)
		if(hits[i] == 6)
			printf("%.2lf\n", sixcut);
		else if(hits[i] == 5)
			printf("%.2lf\n", fivecut);
		else if(hits[i] == 4)
			printf("%.2lf\n", fourcut);
		else
			puts("0.00");
}