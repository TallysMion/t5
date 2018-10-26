#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int tipo;
    char* cep;
    char* face;
    char* num;
    char* comp;
    void* pessoa;
} Endereco;

typedef struct{
    char* cpf;
    char* nome;
    char* sobrenome;
    char* sexo;
    char* nasc;
    Endereco* endereco;
} Pessoa;


//criar Estabelecimento
void* Pessoa_create(char* cpf, char* nome, char* sobrenome, char* sexo, char* nasc){
    Pessoa* pessoa;
    pessoa = (Pessoa*) calloc(1, sizeof(Pessoa));
    
    pessoa->cpf         = (char*) calloc(strlen(cpf)+2, sizeof(char));
    pessoa->nome        = (char*) calloc(strlen(nome)+2, sizeof(char));
    pessoa->sobrenome   = (char*) calloc(strlen(sobrenome)+2, sizeof(char));
    pessoa->sexo        = (char*) calloc(strlen(sexo)+2, sizeof(char));
    pessoa->endereco    = NULL;

    strcpy(pessoa->cpf, cpf);
    strcpy(pessoa->nome, nome);
    strcpy(pessoa->sobrenome, sobrenome);
    strcpy(pessoa->sexo, sexo);

    return (void*) pessoa;

}


//criar Morador
void* Pessoa_SetEndereco(void* pes, char* cep, char* face, char* num, char* comp){
    Pessoa* pessoa;
    pessoa = (Pessoa*) pes;
    if(pessoa->endereco == NULL){
        Endereco* end;
        end = (Endereco*) calloc(1, sizeof(Endereco));
        end->tipo = 1;
        end->cep = (char*) calloc(strlen(cep )+2, sizeof(char));
        end->face= (char*) calloc(strlen(face)+2, sizeof(char));
        end->num = (char*) calloc(strlen(num )+2, sizeof(char));
        end->comp= (char*) calloc(strlen(comp)+2, sizeof(char));
        end->pessoa = pessoa;
        strcpy(end->cep, cep);
        strcpy(end->face, face);
        strcpy(end->num, num);
        strcpy(end->comp, comp);
        pessoa->endereco = end;
        return (void*) end;
    }else{
        free(pessoa->endereco->cep);
        free(pessoa->endereco->face);
        free(pessoa->endereco->num);
        free(pessoa->endereco->comp);
        strcpy(pessoa->endereco->cep, cep);
        strcpy(pessoa->endereco->face, face);
        strcpy(pessoa->endereco->num, num);
        strcpy(pessoa->endereco->comp, comp);
        return (void*) pessoa->endereco;
    }
}

//Retorna o endereco
void* Pessoa_getEndereco(void* pes){
    Pessoa* pessoa;
    pessoa = (Pessoa*) pes;
    return (void*) pessoa->endereco;
}

//free - Limpa a memoria
void Pessoa_Free(void* pes){
    Pessoa* pessoa;
    pessoa = (Pessoa*) pes;
    free(pessoa->cpf);
    free(pessoa->nome);
    free(pessoa->sobrenome);
    free(pessoa->sexo);
    free(pessoa->nasc);
    free(pessoa->endereco->cep);
    free(pessoa->endereco->face);
    free(pessoa->endereco->num);
    free(pessoa->endereco->comp);
    free(pessoa->endereco);
    free(pessoa);
}

//compare - função de comparaçao de Pessoaelecimentos
int Pessoa_compare(void* pes1, void* pes2, int dimension){
    Pessoa *pesA, *pesB;
    pesA = (Pessoa*)pes1;
    pesB = (Pessoa*)pes2;
    return strcmp(pesA->cpf, pesB->cpf);
}

//hashCode - retorna o codigo do Pessoaelecimento
int Pessoa_HashCode(void* pes, int modulo){
    Pessoa *pessoa;
    pessoa = (Pessoa*) pes;
    int x = strlen(pessoa->cpf);
    int hash = 0;
    char *aux = pessoa->cpf;
    while(*aux != 0){
        hash += x*(*aux);
        aux++;
        x--;
    }
    return modulo < 0 ? hash : hash%modulo;
}

int Pessoa_HashCompare(void* pes, char* cpf){
    Pessoa *pessoa;
    pessoa = (Pessoa*) pes;
    return strcmp(pessoa->cpf, cpf);
}