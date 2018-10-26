#include <stdlib.h>
#include "../Lista/lista.h"

typedef struct No{
    void* value;
    struct No *left;
    struct No *Right;
}Node;

typedef struct{
    int (*compare)(void*, void*, int);
    int dimension;
    int size;
    Node *no;
}Tree;

void* KDT_create(int (*compare)(void*, void*, int), int dimension){
    Tree *result;
    result = (Tree*) calloc(1, sizeof(Tree));
    result->compare = compare;
    result->dimension = dimension;
    result->no = NULL;
    result->size = 0;
    return (void*) result;
}

void insert_Node(Node *n, Tree *tr, void* value, int dim){
    if(n->value == NULL){
        n->value = value;
        return;
    }

    if(dim == tr->dimension){
        dim = 0;
    }

    if(tr->compare(n->value, value, dim) < 0){
        if(n->left == NULL){
            n->left = (Node*) calloc(1, sizeof(Node));
            n->left->value=NULL;
            n->left->Right=NULL;
            n->left->left=NULL;
        }
        insert_Node(n->left, tr, value, dim+1);
        return;
    }else{
        if(n->Right == NULL){
            n->Right = (Node*) calloc(1, sizeof(Node));
            n->Right->value=NULL;
            n->Right->Right=NULL;
            n->Right->left=NULL;
        }
        insert_Node(n->Right, tr, value, dim+1);
        return;
    }
}


void KDT_insert(void* tree, void*value){
    Tree *tr;
    tr = (Tree*) tree;
    Node *n;
    n = (Node*) tr->no;
    if(n == NULL){
        n = (Node*) calloc(1, sizeof(Node));
        tr->no = n;
    } 
    insert_Node(n, tr, value, 0);
    tr->size+=1;
}

void freeNode(void* node){
    Node *n;
    n = (Node*) node;
    if(n->left != NULL){
        freeNode(n->left);
    }
    if(n->Right != NULL){
        freeNode(n->Right);
    }
    n->value = NULL;
    free(node);
}

void freeKDTree(void* tree){
    Tree *tr;
    tr = (Tree*) tree;
    freeNode(tr->no);
    tr->compare = NULL;
    tr->size=0;
    tr->dimension=0;
    free(tree);
}

Lista getAllNode(void* node){
    Node* no;
    no = (Node*) node;
    Lista * ls;
    ls = Lista_createLista();
    if(no == NULL){
        return ls;
    }
    if(no->left != NULL){
        Lista_insertAll(ls, getAllNode(no->left));
    }
    if(no->value != NULL){
        Lista_insert(ls, no->value);
    }
    if(no->Right != NULL){
        Lista_insertAll(ls, getAllNode(no->Right));
    }
    return ls;   

}

Lista KDT_getAll(void* tree){
    Tree *tr;
    tr = (Tree*) tree;
    return getAllNode(tr->no);
}

void* getValueNode(Tree* tree, Node* no, int dim, void* reference){
    if(no == NULL){
        return NULL;
    }
    if(dim == tree->dimension){
        dim = 0;
    }

    if(tree->compare(no->value, reference, dim) == 0){
        return no->value;
    }
    if(tree->compare(no->value, reference, dim) < 0){
        return getValueNode(tree, no->left, dim, reference);
    }
    if(tree->compare(no->value, reference, dim) > 0){
        return getValueNode(tree, no->Right, dim, reference);
    }

}

void* KDT_getValue(void* tree, void*reference){
    Tree * tr;
    tr = (Tree*) tree;
    int dim = 0;
    return getValueNode(tree, tr->no, dim, reference);
}

Lista removeValueNode(Tree* tree, Node* no, int dim, void* reference){
    if(no == NULL){
        return NULL;
    }
    if(dim == tree->dimension){
        dim = 0;
    }

    if(tree->compare(no->value, reference, dim) == 0){
        Lista ls = Lista_createLista();
        Lista_insertAll(ls, getAllNode(no->left));
        Lista_insertAll(ls, getAllNode(no->Right));
        no->value = NULL;
        no->left = NULL;
        no->Right = NULL;
        return ls;
    }

    if(tree->compare(no->value, reference, dim) < 0){
        return getValueNode(tree, no->left, dim, reference);
    }
    if(tree->compare(no->value, reference, dim) > 0){
        return getValueNode(tree, no->Right, dim, reference);
    }

}

void KDT_remove(void* tree, void*reference){
    Tree * tr;
    tr = (Tree*) tree;
    int dim = 0;
    Lista ls;
    ls = removeValueNode(tree, tr->no, dim, reference);
    if(ls == NULL){
        return;
    }
    void *t, *temp;
    t = Lista_getFirst(ls);
    while(1){
        temp = Lista_get(ls,t);
        if(temp){
            KDT_insert(tree, temp);
            t = Lista_getNext(ls, t);
        }else{
        break;
        }
    }   

}
