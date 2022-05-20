#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define nullptr NULL

// A arvore que eu implementei foi uma treap, assim como na tarefa 7
// O esquema para essa arvore ficar balanceada eh colocar uma prioridade nos nós
// Assim, ela estatisticamente fica balanceada, como a ideia do quick sort
// Portanto, se um no tiver maior prioridade que outro ele sera seu ancestral
// Ou seja, o nó de maior prioridade sera a raiz da arvore
// Salvo tambem para cada nó a sua chave, e a sua frequencia
// Para fazer a insercao, e se fosse necessario uma remocao, uso um esquema de merge e split
// Proprio da treap, e eu garanto a ordem para uma nova insercao pois eu coloco o no no "ponto de virada"
// Que seria entre os nós que ele eh mais proximo (após o maximo menor e antes do minimo maior)
// E depois a arvore vai se balanceando de acordo com as prioridades
// Eu tambem faço uma busca pela arvore antes de inseriri um novo nó
// Pois, se eu tiver um nó de mesmo valor na arvore, eu somente incremento a frequencia dele

typedef struct No{
    long int val, prio, freq;
    struct No *l, *r, *p;
} No;

typedef struct No * p_no;

p_no novo_no(long int val){
    p_no ret = malloc(sizeof(No));
    ret->l = ret->r = ret->p = nullptr;
    ret->val = val;
    ret->prio = rand();
    ret->freq = 1;
    return ret;
}

// Vou juntar as arvores com raiz em a e b e retorno uma nova raiz
// Se a prioridade de a for maior que a de b eu coloco ele como a raiz de juntar o seu filho direito com a arvore b e  retorno ele como raiz
// Senao, faço analogamente so que para o filho esquerdo de b
p_no merge(p_no a, p_no b){
    if(!a || !b)
        return (a ? a : b);
    else if(a->prio > b->prio){
        a->r = merge(a->r, b);
        if(a->r) a->r->p = a;
        return a;
    } else {
        b->l = merge(a, b->l);
        if(b->l) b->l->p = b;
        return b;
    }
}

// Separo a raiz em nós menores que x e maiores q x, os menores vao para a e os maoires pra b
void split(p_no root, p_no *a, p_no *b, long int x){
    if(!root)
        (*a) = (*b) = nullptr;
    else if(x < root->val){
        (*b) = root;
        split(root->l, a, &root->l, x);
        if(root->l) root->l->p = (*b);
    } else {
        (*a) = root;
        split(root->r, &root->r, b, x);
        if(root->r) root->r->p = (*a);
    }
}

// insere um valor com uma serie de splits e merges, bem intuitivo
p_no insert(p_no root, long int val){
    p_no a = nullptr, b = nullptr;
    split(root, &a, &b, val);
    p_no tmp = merge(a, novo_no(val));
    p_no aux = merge(tmp, b);
    return aux;
}

// retorna um nó que esta na arvore ou nullptr cc, bem intuitivo tb
p_no find(p_no root, long int val){
    if(!root)
        return root;
    if(root->val == val)
        return root;
    if(root->val < val)
        return find(root->r, val);
    return find(root->l, val);
}

// libera a arvore recursivamente
void free_tree(p_no root){
    if(root){
        free_tree(root->l);
        free_tree(root->r);
        free(root);
    }
}

int main(){
    srand(time(NULL));
    p_no treap = nullptr;

    long int n, k;
    long int ans = 0, discount = 0;
    scanf("%ld %ld", &n, &k);
    for(long int i = 1, x;i <= n;i++){
        scanf("%ld", &x);
        p_no aux = find(treap, x);
        if(!aux)
            treap = insert(treap, x);
        else 
            aux->freq++;

        ans++; // aumento a resposta q preciso tirar
        aux = find(treap, x);
        if(aux->val == aux->freq) // se eu chego em um cara com uma frequecia igual seu valor coloco isso no desconto dos caras que nao preciso tirar
            discount += aux->val;
    }
    while(k--){
        long int op, x;
        scanf("%ld", &op);
        if(op == 1){
            scanf("%ld", &x);
            p_no aux = find(treap, x);
            if(!aux) // verifico se um no ainda nao foi colocado
                treap = insert(treap, x);
            else 
                aux->freq++;

            // mesmo da entrada
            ans++;
            aux = find(treap, x);
            if(aux->val == aux->freq)
                discount += aux->val;
        }
        if(op == 2){
            scanf("%ld", &x);
            p_no aux = find(treap, x); 
            if(!aux) // verifico se ja tem na arvore ou eh 0 de frequencia
                puts("0");
            else
                printf("%ld\n", aux->freq);
        }
        if(op == 3){
            printf("%ld\n", ans - discount);
        }
    }
    free_tree(treap);
}