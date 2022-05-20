#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Patient{
    int id, prioritary, on_queue;
    int clinics_to_go[200], clinics_to_go_size;
    char name[60];
} Patient;

void printPatient(Patient p){
	/*printf("Name: %s\nPrioritary: %d\nID: %d\nOn Queue: %d\nClinics to go: ", p.name, p.prioritary, p.id, p.on_queue);
	for(int i = 1;i <= p.clinics_to_go_size;i++)
		printf("%d ", p.clinics_to_go[i]);
	puts("");*/
	printf("ID: %d\n", p.id);
}

typedef struct No{
    Patient data;
    struct No *nxt, *prev;
} No;

typedef struct No * p_no;

typedef struct Deque{
    p_no bg, nd;
    int size;
} Deque;

typedef struct Deque * p_deque;

void printNo(p_no list){
	if(list != NULL) printPatient(list->data), printNo(list->nxt);
}

void printDeque(p_deque d){
	printNo(d->bg);
}

void push_back(p_deque d, Patient x){
    p_no new_node = malloc(sizeof(No));
    new_node->nxt = NULL;
    new_node->prev = d->nd;
    new_node->data = x;

    if(d->size == 0){
    	d->bg = new_node;
    	d->size++;
    	d->nd = new_node;
    	return;
    }
    d->nd->nxt = new_node;
    d->nd = new_node;
    d->size++;
}

void push_front(p_deque d, Patient x){
    p_no new_node = malloc(sizeof(No));
    new_node->nxt = d->bg;
    new_node->prev = NULL;
    new_node->data = x;

    if(d->size == 0){
    	d->nd = new_node;
    	d->size++;
    	d->bg = new_node;
        return;
    }
    d->bg->prev = new_node;
    d->bg = new_node;
    d->size++;
}

void pop_front(p_deque d){
    p_no new_node = d->bg->nxt;
    free(d->bg);

    d->bg = new_node;
    d->size--;
    if(d->size == 0)
    	d->nd = new_node;
    else
        new_node->prev = NULL;
}

void pop_back(p_deque d){
    p_no new_node = d->nd->prev;
    free(d->nd);
    
    d->nd = new_node;
    d->size--;
    if(d->size == 0)
    	d->bg = new_node;
}

Patient patient[100100];
int patient_size;
int t = 8, m = 0;

void print(Patient p){
	printf("%02d:%02d %s\n", t, m, p.name);
}

int pCnt = 0, solved = 0;
p_deque clinic[10];
int clinic_cnt[10] = {0, 10, 2, 5, 3, 4, 7, 2, 1, 4};

void process_patients(){
	for(int i = 1;i <= patient_size;i++){
		Patient *at = &patient[i];

		if(at->clinics_to_go[1] == 0 || at->on_queue || at->clinics_to_go[1] == -1){
			if(at->clinics_to_go[1] == 0 && at->on_queue == 0){
				at->clinics_to_go[1] = -1;
				solved++;
				print(*at);
			}
			continue;
		}

		at->on_queue = 1;
        if(at->prioritary)
            push_front(clinic[at->clinics_to_go[1]], *at);
        else
            push_back(clinic[at->clinics_to_go[1]], *at);
		for(int j = 1;j < 10;j++)
			at->clinics_to_go[j] = at->clinics_to_go[j + 1];
	}
}

void process_queues(){
	m += 10;
	if(m == 60)
		m = 0, t++;
	for(int i = 1;i <= 9;i++)
		for(int j = 1;j <= clinic_cnt[i];j++)
			if(clinic[i]->size) patient[clinic[i]->bg->data.id].on_queue = 0, pop_front(clinic[i]);
}

int main(){
	for(int i = 0;i < 10;i++){
		p_deque aux = malloc(sizeof(Deque));
		aux->bg = NULL;
		aux->nd = NULL;
		aux->size = 0;
		clinic[i] = aux;
	}
    char Type[12];
    Patient p;
    while(scanf("\"%[^\"]\" %s", p.name, Type) != EOF){
        p.prioritary = (Type[0] == 'p');
        p.id = ++pCnt;
        p.on_queue = 0;
        p.clinics_to_go_size = 0;
        for(int i = 0;i < 200;i++)
        	p.clinics_to_go[i] = 0;

        char aux;
        int clinic;
        while(scanf("%d%c", &clinic, &aux)){
            p.clinics_to_go[++p.clinics_to_go_size] = clinic;
            if(aux == '\n')
                break;
        }
        patient[++patient_size] = p;
    }

    while(solved != pCnt){
		process_patients();
		process_queues();
    }

    for(int i = 0;i < 10;i++)
        free(clinic[i]);
}