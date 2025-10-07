#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 🔹 Estrutura da sala
typedef struct Sala {
    char nome[50];          // Nome da sala
    struct Sala* esquerda;  // Sala à esquerda
    struct Sala* direita;   // Sala à direita
} Sala;

// 🔹 Função criarSala
// Cria dinamicamente uma sala com o nome informado
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// 🔹 Função explorarSalas
// Permite ao jogador navegar pelo mapa interativamente
void explorarSalas(Sala* salaAtual) {
    if (salaAtual == NULL) return;

    char escolha;
    printf("\nVocê está na sala: %s\n", salaAtual->nome);

    // Se a sala é folha, termina exploração
    if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
        printf("Esta sala não possui caminhos. Fim da exploração!\n");
        return;
    }

    // Menu de navegação
    do {
        printf("Escolha seu caminho: esquerda (e), direita (d) ou sair (s): ");
        scanf(" %c", &escolha);

        if (escolha == 'e') {
            explorarSalas(salaAtual->esquerda);
            break;
        } else if (escolha == 'd') {
            explorarSalas(salaAtual->direita);
            break;
        } else if (escolha == 's') {
            printf("Você decidiu sair da exploração.\n");
            break;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    } while (1);
}

int main() {
    // 🔹 Montagem do mapa da mansão
    Sala* hallEntrada = criarSala("Hall de Entrada");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* cozinha = criarSala("Cozinha");
    Sala* sotao = criarSala("Sótão");
    Sala* jardim = criarSala("Jardim");
    Sala* salaSecreta = criarSala("Sala Secreta");

    // Conexão entre salas (esquerda e direita)
    hallEntrada->esquerda = biblioteca;
    hallEntrada->direita = cozinha;

    biblioteca->esquerda = sotao;
    biblioteca->direita = jardim;

    cozinha->esquerda = NULL;
    cozinha->direita = salaSecreta;

    // 🔹 Início da exploração
    printf("Bem-vindo ao Detective Quest!\nExplore a mansão para encontrar pistas.\n");
    explorarSalas(hallEntrada);

    // 🔹 Liberação de memória
    free(hallEntrada);
    free(biblioteca);
    free(cozinha);
    free(sotao);
    free(jardim);
    free(salaSecreta);

    return 0;
}
