#include "../include/agente.hpp"
#include <iostream>

using namespace std;

Agente::Agente(Ambiente *ambiente)
{
    this->ambiente = ambiente;

    this->visitados = new bool[this->ambiente->get_qtd_cidades()];
    for (int i = 0; i < this->ambiente->get_qtd_cidades(); i++)
        this->visitados[i] = false;
}

Agente::~Agente()
{
    delete[] visitados;
}