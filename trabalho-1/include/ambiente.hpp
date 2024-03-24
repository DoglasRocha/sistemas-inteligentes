#pragma once
#include "grafo.hpp"

class Ambiente
{
    Grafo *mapa;

public:
    Ambiente();
    ~Ambiente();
    void imprime_mapa();
};
