#pragma once
#include "ambiente.hpp"
#include "grafo.hpp"

typedef struct
{
    int id;
    int weight;
} State;

typedef struct
{
    int tam_caminho, erros;
    Node *caminho;
    int *visitados;
} Resultado;

class Agente
{
    Ambiente *ambiente;
    bool *visitados;
    int qtd_cidades, cidades_visitadas;
    Node *caminho;

public:
    Agente(Ambiente *ambiente);
    ~Agente();
    void reset();
    int escolhe_aleatorio(int limite);
    int tempera_simulada(int *erro);
    int computa_media_caminhos_vizinhos(Node *cidade);
    int computa_qtd_vizinhos(Node *cidade);
    void analise_tempera();
    void imprime_caminho();
    void preenche_vetor_aleatorio(int *vetor, int tamanho);
    int computa_caminho_vetor(int *vetor, int tamanho);
    int setup_algoritmo_genetico(int tamanho_populacao, int geracoes);
    int funcao_adaptacao(int *custo_caminhos, int tamanho_populacao);
    int retorna_indice_mais_adaptado(int *custo_caminhos, int tamanho_populacao);
    int *reproduz(int *pai, int *mae, int tamanho_fita);
    void mutacao(int *fita, int tamanho_fita);
    int algoritmo_genetico(int **populacao, int tamanho_populacao, int *custo_caminhos, int geracoes);
    void analise_genetico();
};