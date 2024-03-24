#include "../include/ambiente.hpp"

Ambiente::Ambiente()
{
    mapa = new Grafo(4);

    mapa->insere_aresta(0, 1, 20)->insere_aresta(0, 2, 42)->insere_aresta(0, 3, 35);

    mapa->insere_aresta(1, 2, 30)->insere_aresta(1, 3, 34);

    mapa->insere_aresta(2, 3, 12);
}

Ambiente::~Ambiente()
{
    delete mapa;
}

void Ambiente::imprime_mapa()
{
    mapa->imprime();
}