#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char v[110][110];
int dir[5] = {1, 0, -1, 0, 1}, pro[110][100];
int n, m, q;

int sz(char *a){
	int len;
	for(len = 0; a[len]; ++len);
	return len;
}

int dfs(int i, int j, int pos, char *word){
	if(pos == sz(word))
		return 1;
	if(v[i][j] != word[pos] || pro[i][j])
		return 0;
	pro[i][j] = 1;
	int ret = 0;
	for(int k = 0;k < 4;k++){
		int ii = dir[k], jj = dir[k + 1];

		if(i + ii > n || i + ii <= 0 || j + jj > m || j + jj <= 0) continue;
		ret |= dfs(i + ii, j + jj, pos + 1, word);
	}
	return ret;
}

int main(){
	scanf("%d %d %d", &n, &m, &q);

	for(int i = 1;i <= n;i++)
		for(int j = 1;j <= m;j++)
			scanf(" %c", &v[i][j]);

	while(q--){
		char *word;
		word = malloc(25 * sizeof(char));
		scanf("%s", word);

		int ok = 0;
		for(int i = 1;i <= n;i++)
			for(int j = 1;j <= m;j++){
				memset(pro, 0, sizeof pro);
				if(dfs(i, j, 0, word)){
					ok = 1;
					puts("sim");
					break;
				}
			}
		if(!ok)
			puts("nao");
		free(word);
	}
}