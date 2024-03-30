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
    int algoritmo_genetico(int *erro);
    void analise_genetico();
};