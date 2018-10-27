#ifndef ESTABELECIMENTO
#define ESTABELECIMENTO

typedef void* type;
typedef void* Estab;

//criar Estabelecimento
Estab Estab_create(char* cnpj, type tipo, char* cep, char* face, char* num, char* nome);

//criar Tipo de Estabelecimento
type Estab_createType(char* cod, char* Info);

//Mudança - Retorna Linha em SVG da mudança
void Estab_changeEndereco(Estab estab,  char* cep, char* face, char* num);

//free - Limpa a memoria
void Estab_Free(Estab estab);

//compare - função de comparaçao de estabelecimentos
int Estab_compare(Estab estab1, Estab estab2, int dimension);

//hashCode - retorna o codigo do estabelecimento
int Estab_HashCode(Estab estab, int modulo);

//Comparador para hash
int Estab_HashCompare(void* estab, void* cnpj);

//Comparador para enderecos
int Endereco_compare(void* end1, void* end2, int dimension);

int Estab_Type_HashCode(void*  tip,int modulo);

int Estab_Type_HashCompare(void* tip, void* cod);

#endif
