#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 26  

// 🔹 Estrutura da sala da mansão
typedef struct Sala {
    char nome[50];
    char pista[100];          
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// 🔹 Estrutura da BST de pistas
typedef struct PistaNode {
    char texto[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// 🔹 Estrutura da tabela hash (cada pista aponta para um suspeito)
typedef struct SuspeitoNode {
    char pista[100];
    char suspeito[50];
    struct SuspeitoNode* proximo;
} SuspeitoNode;

// 🔹 Função criar sala
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    if (!novaSala) { printf("Erro ao alocar memória.\n"); exit(1); }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista != NULL ? pista : "");
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// 🔹 Função criar nó da BST
PistaNode* criarPistaNode(const char* texto) {
    PistaNode* node = (PistaNode*) malloc(sizeof(PistaNode));
    if (!node) { printf("Erro ao alocar memória.\n"); exit(1); }
    strcpy(node->texto, texto);
    node->esquerda = NULL;
    node->direita = NULL;
    return node;
}

// 🔹 Inserir pista na BST
PistaNode* inserirPista(PistaNode* raiz, const char* texto) {
    if (!raiz) return criarPistaNode(texto);
    if (strcmp(texto, raiz->texto) < 0) raiz->esquerda = inserirPista(raiz->esquerda, texto);
    else if (strcmp(texto, raiz->texto) > 0) raiz->direita = inserirPista(raiz->direita, texto);
    return raiz; // duplicatas ignoradas
}

// 🔹 Exibir pistas em ordem alfabética
void exibirPistas(PistaNode* raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->texto);
    exibirPistas(raiz->direita);
}

// 🔹 Inicializar hash (array de ponteiros para listas encadeadas)
void inicializarHash(SuspeitoNode* hash[]) {
    for (int i = 0; i < HASH_SIZE; i++) hash[i] = NULL;
}

// 🔹 Função hash simples: primeira letra da pista (A-Z)
int hashFunc(const char* pista) {
    char c = pista[0];
    if (c >= 'a' && c <= 'z') c -= 32; // maiúscula
    if (c < 'A' || c > 'Z') return 0;
    return c - 'A';
}

// 🔹 Inserir associação pista -> suspeito na hash
void inserirNaHash(SuspeitoNode* hash[], const char* pista, const char* suspeito) {
    int idx = hashFunc(pista);
    SuspeitoNode* novo = (SuspeitoNode*) malloc(sizeof(SuspeitoNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = hash[idx];
    hash[idx] = novo;
}

// 🔹 Encontrar suspeito de uma pista
char* encontrarSuspeito(SuspeitoNode* hash[], const char* pista) {
    int idx = hashFunc(pista);
    SuspeitoNode* atual = hash[idx];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0) return atual->suspeito;
        atual = atual->proximo;
    }
    return NULL;
}

// 🔹 Contar quantas vezes um suspeito aparece nas pistas coletadas
int contarPistasSuspeito(PistaNode* raiz, SuspeitoNode* hash[], const char* suspeito) {
    if (!raiz) return 0;
    int count = contarPistasSuspeito(raiz->esquerda, hash, suspeito) +
                contarPistasSuspeito(raiz->direita, hash, suspeito);
    char* s = encontrarSuspeito(hash, raiz->texto);
    if (s && strcmp(s, suspeito) == 0) count++;
    return count;
}

// 🔹 Explorar salas e coletar pistas
void explorarSalas(Sala* salaAtual, PistaNode** raizPistas, SuspeitoNode* hash[]) {
    if (!salaAtual) return;

    char escolha;
    printf("\nVocê está na sala: %s\n", salaAtual->nome);

    if (strlen(salaAtual->pista) > 0) {
        printf("Você encontrou uma pista: %s\n", salaAtual->pista);
        *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
    }

    do {
        printf("Escolha: esquerda (e), direita (d), sair (s): ");
        scanf(" %c", &escolha);
        if (escolha == 'e') { explorarSalas(salaAtual->esquerda, raizPistas, hash); break; }
        else if (escolha == 'd') { explorarSalas(salaAtual->direita, raizPistas, hash); break; }
        else if (escolha == 's') { printf("Encerrando exploração.\n"); break; }
        else printf("Opção inválida!\n");
    } while(1);
}

// 🔹 Liberar memória da BST
void liberarPistas(PistaNode* raiz) {
    if (!raiz) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// 🔹 Liberar memória da hash
void liberarHash(SuspeitoNode* hash[]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        SuspeitoNode* atual = hash[i];
        while (atual) {
            SuspeitoNode* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
}

int main() {
    // 🔹 Montagem do mapa da mansão
    Sala* hall = criarSala("Hall de Entrada", "Chave antiga");
    Sala* biblioteca = criarSala("Biblioteca", "Livro aberto");
    Sala* cozinha = criarSala("Cozinha", "Faca com resíduo");
    Sala* sotao = criarSala("Sótão", "Caixa trancada");
    Sala* jardim = criarSala("Jardim", "Pegadas misteriosas");
    Sala* salaSecreta = criarSala("Sala Secreta", "Carta cifrada");

    hall->esquerda = biblioteca; hall->direita = cozinha;
    biblioteca->esquerda = sotao; biblioteca->direita = jardim;
    cozinha->direita = salaSecreta;

    // 🔹 Inicializando BST de pistas e hash de suspeitos
    PistaNode* raizPistas = NULL;
    SuspeitoNode* hash[HASH_SIZE];
    inicializarHash(hash);

    // 🔹 Definindo associações pista -> suspeito
    inserirNaHash(hash, "Chave antiga", "Sr. Preto");
    inserirNaHash(hash, "Livro aberto", "Sra. Branca");
    inserirNaHash(hash, "Faca com resíduo", "Sr. Verde");
    inserirNaHash(hash, "Caixa trancada", "Sra. Branca");
    inserirNaHash(hash, "Pegadas misteriosas", "Sr. Preto");
    inserirNaHash(hash, "Carta cifrada", "Sr. Verde");

    printf("Bem-vindo ao Detective Quest - Nível Mestre!\nExplore a mansão e colete pistas.\n");
    explorarSalas(hall, &raizPistas, hash);

    printf("\n🎯 Pistas coletadas:\n");
    exibirPistas(raizPistas);

    // 🔹 Fase de acusação
    char acusado[50];
    printf("\nQuem você acusa como culpado? ");
    scanf(" %[^\n]", acusado);

    int total = contarPistasSuspeito(raizPistas, hash, acusado);
    if (total >= 2) printf("Você acertou! %s é o culpado com %d pistas.\n", acusado, total);
    else printf("Acusação insuficiente. %s não tem evidências suficientes (%d pistas).\n", acusado, total);

    // 🔹 Liberando memória
    free(hall); free(biblioteca); free(cozinha); free(sotao); free(jardim); free(salaSecreta);
    liberarPistas(raizPistas);
    liberarHash(hash);

    return 0;
}
