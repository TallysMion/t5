#ifndef PESSOA_H
#define PESSOA_H

typedef void* Pessoa;
typedef void* Endereco;

//criar Estabelecimento
Pessoa Pessoa_create(char* cpf, char* nome, char* sobrenome, char* sexo, char* nasc);

//criar Morador
Endereco Pessoa_createSetEndereco(Pessoa pes, char* cep, char* face, char* num, char* comp);

//Retorna o endereco
Endereco Pessoa_getEndereco(Pessoa pes);

//free - Limpa a memoria
void Pessoa_Free(Pessoa pes);

//compare - função de comparaçao de Pessoaelecimentos
int Pessoa_compare(Pessoa pes1, Pessoa pes2, int dimension);

//hashCode - retorna o codigo do Pessoaelecimento
int Pessoa_HashCode(Pessoa pes, int modulo);

//Comparador para hashtable
int Pessoa_HashCompare(void* pes, char* cpf);

#endif
