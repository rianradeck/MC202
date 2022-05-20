#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double max(double a, double b){
	return (a > b ? a : b);
}

double min(double a, double b){
	return (a < b ? a : b);
}

int main(){
	int n, m;
	scanf("%d %d", &n, &m);

	char **name;
	name = malloc( n * sizeof (char *));
	if(name == NULL){
		printf("OUT OF MEMORY\n");
		exit(0);
	}

	double **value, *maximum, *minimum, *avg, *deviation;

	value = malloc(n * sizeof (double *));
	if(value == NULL){
		printf("OUT OF MEMORY\n");
		exit(0);
	}
	maximum = malloc(n * sizeof (double));
	if(maximum == NULL){
		printf("OUT OF MEMORY\n");
		exit(0);
	}
	minimum = malloc(n * sizeof (double)); 
	if(minimum == NULL){
		printf("OUT OF MEMORY\n");
		exit(0);
	}
	avg = malloc(n * sizeof (double));
	if(avg == NULL){
		printf("OUT OF MEMORY\n");
		exit(0);
	}
	deviation = malloc(n * sizeof (double));
	if(deviation == NULL){
		printf("OUT OF MEMORY\n");
		exit(0);
	}

	for(int i = 0;i < n;i++){
		name[i] = malloc(30 * sizeof (char));
		if(name[i] == NULL){
			printf("OUT OF MEMORY\n");
			exit(0);
		}
		value[i] = malloc(m * sizeof (double));
		if(value[i] == NULL){
			printf("OUT OF MEMORY\n");
			exit(0);
		}

		scanf("%s", name[i]);
		minimum[i] = 0x3f3f3f3f;
		maximum[i] = 0;
		avg[i] = 0;
		for(int j = 0;j < m;j++){
			scanf("%lf", &value[i][j]);
			maximum[i] = max(maximum[i], value[i][j]);
			minimum[i] = min(minimum[i], value[i][j]);
			avg[i] += value[i][j] / m;
		}
	}

	for(int i = 0;i < n;i++){
		double sum = 0;
		for(int j = 0;j < m;j++)
			sum += 1.0 * (value[i][j] - avg[i]) * (value[i][j] - avg[i]);
		deviation[i] = sqrt(sum / m);
	}

	int *bot, bot_sz = 2, bot_cnt = 0;
	bot = malloc(2 * sizeof (int));
	int *surprising, surprising_sz = 2, surprising_cnt = 0;
	surprising = malloc(2 * sizeof (int));
	int *normal, normal_sz = 2, normal_cnt = 0;
	normal = malloc(2 * sizeof (int));
	int *local, local_sz = 2, local_cnt = 0;
	local = malloc(2 * sizeof (int));
	int *irrelevant, irrelevant_sz = 2, irrelevant_cnt = 0;
	irrelevant = malloc(2 * sizeof (int));


	for(int i = 0;i < n;i++){
		printf("%s %.2lf %.2lf %.2lf %.2lf\n", name[i], maximum[i], minimum[i], avg[i], deviation[i]);
		
		if(avg[i] >= 60.0 && deviation[i] > 15.0){
			++bot_cnt;
			if(bot_cnt > bot_sz){
				int *copy;
				copy = malloc((int)(1.5 * bot_sz) * sizeof (int));
				for(int i = 0;i < bot_sz;i++)
					copy[i] = bot[i];
				free(bot);
				bot = copy;
				bot_sz = (int)(1.5 * bot_sz);
			}
			bot[bot_cnt - 1] = i;
		}

		if(avg[i] >= 60.0 && deviation[i] <= 15.0){
			++surprising_cnt;
			if(surprising_cnt > surprising_sz){
				int *copy;
				copy = malloc((int)(1.5 * surprising_sz) * sizeof (int));
				for(int i = 0;i < surprising_sz;i++)
					copy[i] = surprising[i];
				free(surprising);
				surprising = copy;
				surprising_sz = (int)(1.5 * surprising_sz);
			}
			surprising[surprising_cnt - 1] = i;
		}

		if(avg[i] < 60.0 && maximum[i] >= 80 && minimum[i] > 20.0){
			++normal_cnt;
			if(normal_cnt > normal_sz){
				int *copy;
				copy = malloc((int)(1.5 * normal_sz) * sizeof (int));
				for(int i = 0;i < normal_sz;i++)
					copy[i] = normal[i];
				free(normal);
				normal = copy;
				normal_sz = (int)(1.5 * normal_sz);
			}
			normal[normal_cnt - 1] = i;
		}

		if(avg[i] < 60.0 && maximum[i] >= 80 && minimum[i] <= 20.0){
			++local_cnt;
			if(local_cnt > local_sz){
				int *copy;
				copy = malloc((int)(1.5 * local_sz) * sizeof (int));
				for(int i = 0;i < local_sz;i++)
					copy[i] = local[i];
				free(local);
				local = copy;
				local_sz = (int)(1.5 * local_sz);
			}
			local[local_cnt - 1] = i;
		}

		if(avg[i] < 60.0 && maximum[i] < 80){
			++irrelevant_cnt;
			if(irrelevant_cnt > irrelevant_sz){
				int *copy;
				copy = malloc((int)(1.5 * irrelevant_sz) * sizeof (int));
				for(int i = 0;i < irrelevant_sz;i++)
					copy[i] = irrelevant[i];
				free(irrelevant);
				irrelevant = copy;
				irrelevant_sz = (int)(1.5 * irrelevant_sz);
			}
			irrelevant[irrelevant_cnt - 1] = i;
		}

	}

	printf("\nRESULTADO:\nBot (%d): ", bot_cnt);
	for(int i = 0;i < bot_cnt;i++)
		printf("%s ", name[bot[i]]);
	printf("\nSurpreendente (%d): ", surprising_cnt);
	for(int i = 0;i < surprising_cnt;i++)
		printf("%s ", name[surprising[i]]);
	printf("\nNormal (%d): ", normal_cnt);
	for(int i = 0;i < normal_cnt;i++)
		printf("%s ", name[normal[i]]);
	printf("\nLocal (%d): ", local_cnt);
	for(int i = 0;i < local_cnt;i++)
		printf("%s ", name[local[i]]);
	printf("\nIrrelevante (%d): ", irrelevant_cnt);
	for(int i = 0;i < irrelevant_cnt;i++)
		printf("%s ", name[irrelevant[i]]);
	
	for(int i = 0;i < n;i++){
		free(name[i]);
		free(value[i]);
	}

	free(name);
	free(value);
	free(maximum);
	free(minimum);
	free(avg);
	free(deviation);

	free(bot);
	free(surprising);
	free(normal);
	free(local);
	free(irrelevant);
}