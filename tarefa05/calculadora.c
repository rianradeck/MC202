#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No{
    int d;
    struct No *nxt;
} No;

typedef struct No * p_no;

p_no lcreate(){
    return NULL;
}

void ldestroy(p_no list){
    if(list != NULL)
        ldestroy(list->nxt);
    free(list);
}

p_no linsert(p_no list, int x){
    p_no new_node = malloc(sizeof(No));
    new_node->d = x;
    new_node->nxt = list;
    return new_node;
}

int lprint(p_no list, int firstcall){
    if(list != NULL){
        if(lprint(list->nxt, 0) || list->d != 0){
            printf("%d", list->d);
            return 1;
        }
        else
            return 0;
    }
    return 0;
}

p_no lconvert(char v[]){
    p_no new_list = NULL;
    for(int i = 0;v[i] != '\0';i++)
        new_list = linsert(new_list, v[i] - '0');
    return new_list;
}

p_no lsum(p_no a, p_no b, int carry){
    p_no ans = NULL;
    if(a == NULL && b == NULL){
        if(carry != 0)
            ans = linsert(ans, carry);
        return ans;
    }
    int x = (a == NULL ? 0 : a->d);
    int y = (b == NULL ? 0 : b->d);

    ans = linsert(ans, (x + y + carry) % 10);
    ans->nxt = lsum(a == NULL ? a : a->nxt, b == NULL ? b : b->nxt, (x + y + carry) / 10);
    return ans;
}

p_no lsub(p_no a, p_no b, int carry){
    p_no ans = NULL;
    if(a == NULL && b == NULL)
        return ans;

    int x = (a == NULL ? 0 : a->d);
    int y = (b == NULL ? 0 : b->d);
    y += carry;

    ans = linsert(ans, (x >= y ? x - y : x - y + 10));
    ans->nxt = lsub(a == NULL ? a : a->nxt, b == NULL ? b : b->nxt, x >= y ? 0 : 1);
    return ans;
}

p_no lmultsingle(p_no a, int x){
    p_no ans = NULL;
    for(int i = 0;i < x;i++){
        p_no tmp = ans;
        ans = lsum(tmp, a, 0);
        ldestroy(tmp);
    }
    return ans;
}

p_no lmult(p_no a, p_no b){
    p_no ans = NULL;
    if(b == NULL)
        return ans;

    ans = lmult(a, b->nxt);
    ans = linsert(ans, 0);
    p_no tmp = lmultsingle(a, b->d);
    p_no tmp2 = ans;
    ans = lsum(tmp2, tmp, 0);
    ldestroy(tmp2);
    ldestroy(tmp);
    return ans;
}

int main(){
    int t;
    scanf("%d", &t);
    while(t--){
        p_no a = NULL, b = NULL;
        char op;
        scanf(" %c", &op);
        char s1[120], s2[120];
        scanf("%s %s", s1, s2);

        a = lconvert(s1);
        b = lconvert(s2);

        p_no tmp = NULL;
        if(op == '+'){
            tmp = lsum(a, b, 0);
            if(!lprint(tmp, 1))
                printf("0");
            puts("");
        }
        if(op == '-'){
            if(strlen(s1) < strlen(s2) || (strlen(s1) == strlen(s2) && strcmp(s1, s2) < 0)){
                p_no aux = a;
                a = b;
                b = aux;
            }

            tmp = lsub(a, b, 0);
            if(!lprint(tmp, 1))
                printf("0");
            puts("");
        }
        if(op == '*'){
            tmp = lmult(a, b);
            if(!lprint(tmp, 1))
                printf("0");
            puts("");
        }
        if(op == '/')
            puts("0");

        ldestroy(tmp);
        ldestroy(a);
        ldestroy(b);
    }
}