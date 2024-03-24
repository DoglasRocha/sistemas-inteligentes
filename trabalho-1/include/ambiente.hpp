#pragma once
#include "grafo.hpp"

class Ambiente
{
    Grafo *mapa;

public:
    Ambiente();
    ~Ambiente();
    void imprime_mapa();
    int get_qtd_cidades();
    int get_tam_mapa();
    Node *get_cidade(int id);
};
