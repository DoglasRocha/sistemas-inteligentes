#pragma once
#include "ambiente.hpp"
#include "grafo.hpp"

class Agente
{
    Ambiente *ambiente;
    bool *visitados;
    Node *caminho;

public:
    Agente(Ambiente *ambiente);
    ~Agente();
    int escolhe_aleatorio(int limite);
    void andar();
};