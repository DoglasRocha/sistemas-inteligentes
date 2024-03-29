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
    void reset();
    int escolhe_aleatorio(int limite);
    int tempera_simulada(int *percorreu_tudo);
    int computa_media_caminhos_vizinhos(Node *cidade);
    int computa_qtd_vizinhos(Node *cidade);
    void analise_tempera();
    void imprime_caminho();
};