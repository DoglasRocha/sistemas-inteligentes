#pragma once
#include "ambiente.hpp"
#include "grafo.hpp"

typedef struct {
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
    void andar();
    void computa_qtd_vizinhos_e_media(Node *cidade, int *qtd_vizinhos, int *media);
    void imprime_caminho();
};