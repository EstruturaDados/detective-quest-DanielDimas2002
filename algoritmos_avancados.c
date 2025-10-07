#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 🔹 Estrutura da sala da mansão
typedef struct Sala {
    char nome[50];           // Nome da sala
    char pista[100];         // Pista da sala (opcional)
    struct Sala* esquerda;   // Ponteiro para sala à esquerda
    struct Sala* direita;    // Ponteiro para sala à direita
} Sala;

// 🔹 Estrutura do nó da BST de pistas
typedef struct PistaNode {
    char texto[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// 🔹 Função criarSala
// Cria uma sala com nome e pista opcional
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    if (pista != NULL) {
        strcpy(novaSala->pista, pista);
    } else {
        novaSala->pista[0] = '\0';
    }
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// 🔹 Função criar nó da BST de pistas
PistaNode* criarPistaNode(const char* texto) {
    PistaNode* novoNode = (PistaNode*) malloc(sizeof(PistaNode));
    if (novoNode == NULL) {
        printf("Erro ao alocar memória para a pista.\n");
        exit(1);
    }
    strcpy(novoNode->texto, texto);
    novoNode->esquerda = NULL;
    novoNode->direita = NULL;
    return novoNode;
}

// 🔹 Função inserirPista
// Insere uma pista na BST em ordem alfabética
PistaNode* inserirPista(PistaNode* raiz, const char* texto) {
    if (raiz == NULL) {
        return criarPistaNode(texto);
    }
    if (strcmp(texto, raiz->texto) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    } else if (strcmp(texto, raiz->texto) > 0) {
        raiz->direita = inserirPista(raiz->direita, texto);
    }
    // Se a pista já existe, não insere duplicata
    return raiz;
}

// 🔹 Função exibirPistas
// Imprime a BST de pistas em ordem alfabética
void exibirPistas(PistaNode* raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->texto);
    exibirPistas(raiz->direita);
}

// 🔹 Função explorarSalasComPistas
// Navega pelo mapa e coleta automaticamente as pistas encontradas
void explorarSalasComPistas(Sala* salaAtual, PistaNode** raizPistas) {
    if (salaAtual == NULL) return;

    char escolha;
    printf("\nVocê está na sala: %s\n", salaAtual->nome);

    // Se houver pista, adiciona à BST
    if (strlen(salaAtual->pista) > 0) {
        printf("Você encontrou uma pista: %s\n", salaAtual->pista);
        *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
    }

    // Menu de navegação
    do {
        printf("Escolha seu caminho: esquerda (e), direita (d) ou sair (s): ");
        scanf(" %c", &escolha);

        if (escolha == 'e') {
            explorarSalasComPistas(salaAtual->esquerda, raizPistas);
            break;
        } else if (escolha == 'd') {
            explorarSalasComPistas(salaAtual->direita, raizPistas);
            break;
        } else if (escolha == 's') {
            printf("Você decidiu encerrar a exploração.\n");
            break;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    } while (1);
}

// 🔹 Função liberar memória da BST
void liberarPistas(PistaNode* raiz) {
    if (raiz == NULL) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

int main() {
    // 🔹 Mapa da mansão
    Sala* hallEntrada = criarSala("Hall de Entrada", "Chave antiga no tapete");
    Sala* biblioteca = criarSala("Biblioteca", "Livro estranho aberto");
    Sala* cozinha = criarSala("Cozinha", "Faca com resíduo suspeito");
    Sala* sotao = criarSala("Sótão", "Caixa trancada com documentos");
    Sala* jardim = criarSala("Jardim", "Pegadas misteriosas");
    Sala* salaSecreta = criarSala("Sala Secreta", "Carta cifrada");

    // Conectando salas
    hallEntrada->esquerda = biblioteca;
    hallEntrada->direita = cozinha;

    biblioteca->esquerda = sotao;
    biblioteca->direita = jardim;

    cozinha->esquerda = NULL;
    cozinha->direita = salaSecreta;

    // 🔹 BST de pistas
    PistaNode* raizPistas = NULL;

    printf("Bem-vindo ao Detective Quest!\nExplore a mansão e colete todas as pistas.\n");
    explorarSalasComPistas(hallEntrada, &raizPistas);

    printf("\n🎯 Pistas coletadas em ordem alfabética:\n");
    exibirPistas(raizPistas);

    // 🔹 Liberação de memória
    free(hallEntrada);
    free(biblioteca);
    free(cozinha);
    free(sotao);
    free(jardim);
    free(salaSecreta);
    liberarPistas(raizPistas);

    return 0;
}
