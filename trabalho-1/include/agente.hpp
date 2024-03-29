#pragma once
#include "ambiente.hpp"
#include "grafo.hpp"

typedef struct
{
    int id;
    int weight;
} State;

class Agente
{
    Ambiente *ambiente;
    bool *visitados;
    Node *caminho;
    int qtd_cidades, cidades_visitadas;

public:
    Agente(Ambiente *ambiente);
    ~Agente();
    int escolhe_aleatorio(int limite);
    int tempera_simulada();
    int computa_media_caminhos_vizinhos(Node *cidade);
    int computa_qtd_vizinhos(Node *cidade);
    void imprime_caminho();
};