#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
    char* cod;
    char* info;
} type;

typedef struct{
    int tipo;
    char* cep;
    char* face;
    char* num;
    char* comp;
    void* estab;
} Endereco;

typedef struct{
    char* cnpj;
    type* tipo;
    Endereco* ende;
    char* nome;
} Estab;

//criar Estabelecimento
Estab* Estab_create(char* cnpj, void* tip, char* cep, char* face, char* num, char* nome){
    Estab* result;
    result = (Estab*) calloc(1, sizeof(Estab));

    Endereco* end;
    end = (Endereco*) calloc(1, sizeof(Endereco));

    end->tipo    = 0;
    end->cep     = (char*) calloc(strlen(cep)+2  , sizeof(char*));
    end->face    = (char*) calloc(strlen(face)+2 , sizeof(char*));
    end->num     = (char*) calloc(strlen(num)+2  , sizeof(char*));
    end->comp    = NULL;
    
    type* tipo;
    tipo = (type*) tip;

    result->cnpj    = (char*) calloc(strlen(cnpj)+2 , sizeof(char*));
    result->nome    = (char*) calloc(strlen(nome)+2 , sizeof(char*));

    strcpy(result->cnpj, cnpj);
    strcpy(end->cep, cep);
    strcpy(end->face, face);
    strcpy(end->num, num);
    strcpy(result->nome, nome);
    result->tipo = tipo;
    result->ende = end;
    result->ende->estab = result;

    return (void*) result;

}

//Retornar o Endereco
Endereco* Estab_getEndereco(void* estab){
    Estab* est;
    est = (Estab*) estab;
    return (void*) est->ende;
}

//criar Tipo de Estabelecimento
type* Estab_createType(char* cod, char* info){
    type* tipo;
    tipo = (type*) calloc(1, sizeof(type));
    tipo->cod = cod;
    tipo->info = info;
    return (void*) tipo;
}

//Mudança - Retorna Linha em SVG da mudança
void* Estab_changeEndereco(void* estab,  char* cep, char* face, char* num){
    Estab* est;
    est = (Estab*) estab;
    free(est->ende->cep);
    free(est->ende->face);
    free(est->ende->num);

    est->ende->cep    = (char*) calloc(strlen(cep)+2 , sizeof(char*));
    est->ende->face    = (char*) calloc(strlen(face)+2 , sizeof(char*));
    est->ende->num     = (char*) calloc(strlen(num)+2  , sizeof(char*));

    strcpy(est->ende->cep, cep);
    strcpy(est->ende->face, face);
    strcpy(est->ende->num, num);

    return (void*) est->ende;
}

//free - Limpa a memoria
void Estab_Free(void* estab){
    Estab* est;
    est = (Estab*) estab;
    free(est->nome);
    free(est->cnpj);
    free(est->ende->cep);
    free(est->ende->face);
    free(est->ende->num);
    free(est->ende);
    free(est);
}

//compare - função de comparaçao de estabelecimentos
int Estab_compare(void* estab1, void* estab2, int dimension){
    Estab* estA;
    Estab* estB;    
    estA = (Estab*) estab1;
    estB = (Estab*) estab2;
    return strcmp(estA->cnpj, estB->cnpj);
}

//hashCode - retorna o codigo do estabelecimento
int Estab_HashCode(void* estab, int modulo){
    Estab *est;
    est = (Estab*) estab;
    int x = strlen(est->cnpj);
    int hash = 0;
    char *aux = est->cnpj;
    while(*aux != 0){
        hash += x*(*aux);
        aux++;
        x--;
    }
    return modulo < 0 ? hash : hash%modulo;
}

int Estab_HashCompare(void* estab, void* cnpj){
    Estab *est;
    est = (Estab*) estab;
    return strcmp(est->cnpj, cnpj);
}

int Endereco_compare(void* end1, void* end2, int dimension){
    Endereco* endr1;
    Endereco* endr2;    
    endr1 = (Endereco*) end1;
    endr2 = (Endereco*) end2;
    return strcmp(endr1->cep, endr2->cep);
}


//hashCode - retorna o codigo do estabelecimento
int Estab_Type_HashCode(void*  tip,int modulo){
    type *tp;
    tp = (type*) tip;
    int x = strlen(tp->cod);
    int hash = 0;
    char *aux = tp->cod;
    while(*aux != 0){
        hash += x*(*aux);
        aux++;
        x--;
    }
    return modulo < 0 ? hash : hash%modulo;
}

int Estab_Type_HashCompare(void* tip, void* cod){
    type *tp, *id;
    tp = (type*) tip;
    id = (type*) cod;
    return strcmp(tp->cod, id->cod);
}

