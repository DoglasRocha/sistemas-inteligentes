#include "include/ambiente.hpp"
#include "include/agente.hpp"
#include <iostream>

using namespace std;

int main(void)
{
    Ambiente a;

    // a.imprime_mapa();
    Agente ag(&a);
    int erro;

    // for (int i = 0; i < 100; i++)
    //     cout << ag.tempera_simulada() << endl;
    // ag.analise_tempera();
    ag.setup_algoritmo_genetico(&erro);

    return 0;
}