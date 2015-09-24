struct lista{
	char dado;
	struct lista*prox;
};
typedef struct lista Lista;

struct fila{
	Lista * inicio;
	Lista * fim;
	int tamanho;
};
typedef struct fila Fila;

Fila* cria(){
	Fila* aux;
	aux = (Fila*)malloc(sizeof(Fila));
	aux->inicio = NULL;
	aux->fim = NULL;
	aux->tamanho = 0;
	return aux;
}

Lista* cria_lista(){
	Lista *aux;
	aux = (Lista*)malloc(sizeof(Lista));
	aux = NULL;
	return aux;
}

int fila_vaziaBit(Fila * fila){
	if(fila->tamanho == 0){
		return 0;
	}else{
		return 1;
	}
}

void insere_fila(Fila *fila, char valor){
	Lista *aux;
	aux = (Lista*)malloc(sizeof(Lista));
	aux->dado = valor;
	aux->prox = NULL;
	if(fila->fim != NULL){
		fila->fim->prox = aux;
		fila->fim = fila->fim->prox;
		fila->tamanho++;
	}else{
		fila->inicio = aux;
		fila->fim = aux;
		fila->tamanho++;
	}
}

void remove(Fila * fila){
	Lista* aux;
	if(fila_vaziaBit(fila) == 0){
		printf("Lista Vazia!\n");
	}else{
		aux = fila->inicio;
		fila->inicio = aux->prox;
		fila->tamanho--;
		if(fila->inicio == NULL){
			fila->fim = NULL;
		}
		free(aux);
	}
}
