#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Parâmetros Gerais ---
#define CAPACIDADE_FILA 5  // A fila manterá exatamente 5 elementos
#define NOME_PECA_MAX 3    // Ex: 'I', 'O', 'T', 'L' + '\0'

// Estrutura que representa uma peça do jogo
typedef struct {
    int id;
    char nome[NOME_PECA_MAX];
} Peca;

// Estrutura da Fila Circular
typedef struct {
    Peca pecas[CAPACIDADE_FILA];
    int frente;      // Posição do primeiro item
    int tras;        // Ponto onde a próxima peça será inserida
    int tamanho;     // Quantidade atual de peças armazenadas
    int proximo_id;  // Contador de IDs únicos
} FilaCircular;

// --- Protótipos ---
void inicializarFila(FilaCircular *fila);
Peca gerarPeca(FilaCircular *fila);
void exibirFila(FilaCircular *fila);
int filaCheia(FilaCircular *fila);
int filaVazia(FilaCircular *fila);
void inserirPeca(FilaCircular *fila, Peca novaPeca);
Peca removerPeca(FilaCircular *fila);
void menuPrincipal(FilaCircular *fila);


// --- Funções ---

// Inicializa a fila circular e insere 5 peças aleatórias.
void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = 0;
    fila->tamanho = 0;
    fila->proximo_id = 1;
    srand(time(NULL)); // Semente do gerador aleatório

    // Insere peças iniciais
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        inserirPeca(fila, gerarPeca(fila));
    }
}

// Gera uma nova peça com um ID único e nome aleatório.
Peca gerarPeca(FilaCircular *fila) {
    Peca novaPeca;

    novaPeca.id = fila->proximo_id++;

    // Tipos de peças válidas
    const char *tipos[] = {"I", "O", "T", "L", "J", "Z", "S"};
    int total_tipos = sizeof(tipos) / sizeof(tipos[0]);

    int indice_random = rand() % total_tipos;
    strcpy(novaPeca.nome, tipos[indice_random]);

    return novaPeca;
}

// Retorna 1 se a fila estiver cheia.
int filaCheia(FilaCircular *fila) {
    return fila->tamanho == CAPACIDADE_FILA;
}

// Retorna 1 se a fila estiver vazia.
int filaVazia(FilaCircular *fila) {
    return fila->tamanho == 0;
}

// Insere peça ao final da fila.
void inserirPeca(FilaCircular *fila, Peca novaPeca) {
    if (filaCheia(fila)) {
        printf("Erro: A fila está completa. Não é possível inserir.\n");
        return;
    }

    fila->pecas[fila->tras] = novaPeca;
    fila->tras = (fila->tras + 1) % CAPACIDADE_FILA;
    fila->tamanho++;
}

// Remove a peça do início da fila.
Peca removerPeca(FilaCircular *fila) {
    if (filaVazia(fila)) {
        printf("Erro: Fila vazia. Sem peças disponíveis.\n");
        Peca vazia = {0, ""};
        return vazia;
    }

    Peca retirada = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % CAPACIDADE_FILA;
    fila->tamanho--;

    return retirada;
}

// Exibe a fila atual e mostra frente e traseira.
void exibirFila(FilaCircular *fila) {
    printf("\n--- Fila de Peças Futuras (Limite: %d) ---\n", CAPACIDADE_FILA);

    if (filaVazia(fila)) {
        printf("Nenhum item na fila.\n");
        return;
    }

    int i = fila->frente;
    int contador = 0;

    printf("Fila: [ ");
    while (contador < fila->tamanho) {
        printf("(%s #%d) ", fila->pecas[i].nome, fila->pecas[i].id);
        i = (i + 1) % CAPACIDADE_FILA;
        contador++;
    }
    printf("]\n");

    printf("-> Próxima peça (frente): (%s #%d)\n",
           fila->pecas[fila->frente].nome, fila->pecas[fila->frente].id);

    printf("-> Posição de inserção (trás): %d\n", fila->tras);
    printf("-------------------------------------------\n");
}

// Controla o menu interativo.
void menuPrincipal(FilaCircular *fila) {
    int escolha;

    do {
        exibirFila(fila);

        printf("\n--- Menu de Ações ---\n");
        printf("1. Jogar a peça da frente\n");
        printf("2. Atualizar visualização\n");
        printf("0. Encerrar\n");
        printf("Selecione: ");

        if (scanf("%d", &escolha) != 1) {
            while (getchar() != '\n');
            escolha = -1;
        }

        printf("\n");

        switch (escolha) {
            case 1: {
                Peca jogada = removerPeca(fila);
                if (jogada.id != 0) {
                    printf("Peça removida: (%s #%d)\n", jogada.nome, jogada.id);

                    Peca nova = gerarPeca(fila);
                    inserirPeca(fila, nova);
                    printf("Nova peça inserida automaticamente: (%s #%d)\n", nova.nome, nova.id);
                }
                break;
            }
            case 2:
                printf("Exibição atualizada.\n");
                break;
            case 0:
                printf("Encerrando o sistema...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

        printf("\n");

    } while (escolha != 0);
}

// Função principal.
int main() {
    FilaCircular filaDePecas;

    inicializarFila(&filaDePecas);
    menuPrincipal(&filaDePecas);

    return 0;
}
