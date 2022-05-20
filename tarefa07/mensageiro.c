#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define nullptr NULL

// val é o valor do meu nó, name é o cartao, l é o filho esquerdo e r o direito, p é o pai
typedef struct No{
    int val;
    struct No *l, *r, *p;
    char *name;
} No;

typedef struct No * p_no;

// crio um novo no, com o valor val e cartao name, e coloco meus filhos e meu pai como nullptr (null pointer)
p_no novo_no(int val, char name[]){
    p_no ret = malloc(sizeof(No));
    ret->l = ret->r = ret->p = nullptr;
    ret->val = val;
    ret->name = malloc(strlen(name) + 1);
    strcpy(ret->name, name);
    return ret;
}

// retorno a raiz da juncao de duas arvores cujo as raizes sao a e b
p_no merge(p_no a, p_no b){
    if(!a || !b)
        return (a ? a : b);
    else if(a->val < b->val){
        a->r = merge(a->r, b);
        if(a->r) a->r->p = a;
        return a;
    } else {
        b->r = merge(a, b->r);
        if(b->r) b->r->p = b;
        return b;
    }
}

// separo uma arvore onde todo mundo maior ou igual a x fica na arvore com raiz b
// e todo mundo menor fica em uma arvore com raiz em a
void split(p_no root, p_no *a, p_no *b, int x){
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

// insere um no com valor e palavra na arvore
p_no insert(p_no root, int val, char name[]){
    p_no a = nullptr, b = nullptr;
    split(root, &a, &b, val);
    p_no tmp = merge(a, novo_no(val, name));
    p_no aux = merge(tmp, b);
    return aux;
}

// deleta um valor da arvore
p_no erase(p_no root, int val){
    p_no a = nullptr, b = nullptr, aux = nullptr;
    split(root, &a, &b, val - 1);
    split(b, &aux, &b, val);
    free(aux->name);
    free(aux);
    p_no tmp = merge(a, b);
    if(tmp) tmp->p = nullptr;
    return tmp;
}

// imprime a arvore
void print(p_no node){
    if(node){
        print(node->l);
        printf("%s", node->name);
        print(node->r);
    }
}

// pega o menor valor da arvore
p_no get_min(p_no root){
    if(!root)
        return root;
    while(root->l)
        root = root->l;
    return root;
}

// pega o proximo cara da arvore
p_no get_nxt(p_no root){
    if(!root)
        return root;
    if(root->r)
        return get_min(root->r);
    else{
        while(root->p && root->p->l != root)
            root = root->p;
        return root->p;
    }
}

// verifica de maneira "rapida" se um valor esta na arvore
// se os explos forem randomicos a complexidade esperada eh O(lg n)
// porem no pior naso pode ser O(n), onde n eh a quantidade de elementos na arvore
p_no find(p_no root, int val){
    if(!root)
        return nullptr;
    if(root->val == val)
        return root;

    if(root->val < val)
        return find(root->r, val);
    else
        return find(root->l, val);
}

// libero a arvore
void free_tree(p_no root){
    if(root){
        free_tree(root->l);
        free_tree(root->r);
        free(root->name);
        free(root);
    }
}

int main(){
    p_no root = nullptr;

    int n, q;
    while(scanf("%d %d", &n, &q) != EOF){
        for(int i = 0;i < n;i++){
            int val;
            char name[6];
            scanf("%d \"%[^\"]\"", &val, name);
            root = insert(root, val, name);
        }
        
        while(q--){
            int x, ok = 1;
            scanf("%d", &x);

            char c[5010] = "";
            // percorro a arvore com 2 valores indo do menor para o maior e vendo se tem algum q completa a resposta
            for(p_no i = get_min(root);i != nullptr && ok;i = ok ? get_nxt(i) : nullptr)
                for(p_no j = get_nxt(i);j != nullptr && ok; j = ok ? get_nxt(j) : nullptr){
                    p_no k = find(root, x - i->val - j->val);
                    if(k && k->val != i->val && k->val != j->val){
                        c[0] = 0;
                        strcat(c, i->name);
                        strcat(c, j->name);
                        strcat(c, k->name);
                        root = erase(root, i->val);
                        root = erase(root, j->val);
                        root = erase(root, k->val);
                        root = insert(root, x, c);
                        ok = 0;
                    }
                }
        }
        print(root);
        puts("");
        free_tree(root);
        root = nullptr;
    }
}