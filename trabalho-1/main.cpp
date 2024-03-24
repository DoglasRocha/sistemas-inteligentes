#include "include/ambiente.hpp"
#include "include/agente.hpp"

int main(void)
{
    Ambiente a;

    a.imprime_mapa();
    Agente ag(&a);

    ag.andar();
    ag.imprime_caminho();

    return 0;
}