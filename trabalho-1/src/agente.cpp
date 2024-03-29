#include "../include/agente.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

// #define DEBUG

using namespace std;

Agente::Agente(Ambiente *ambiente)
{
    srand(time(NULL));
    this->ambiente = ambiente;
    this->qtd_cidades = this->ambiente->get_qtd_cidades();
    this->cidades_visitadas = 0;

    this->visitados = new bool[this->qtd_cidades];
    for (int i = 0; i < this->qtd_cidades; i++)
        this->visitados[i] = false;
}

Agente::~Agente()
{
    delete[] visitados;
    delete caminho;
}

int Agente::computa_media_caminhos_vizinhos(Node *cidade)
{
    int sum = 0, qtd_vizinhos = 0;

    for (Node *aux = cidade; aux != nullptr; aux = aux->next)
    {
        qtd_vizinhos++;
        sum += aux->weight;
    }

    return sum / qtd_vizinhos;
}

int Agente::computa_qtd_vizinhos(Node *cidade)
{
    int qtd_vizinhos = 0;

    for (Node *aux = cidade; aux != nullptr; aux = aux->next)
    {
        qtd_vizinhos++;
    }

    return qtd_vizinhos;
}

int Agente::escolhe_aleatorio(int limite)
{
    return rand() % limite;
}

int Agente::tempera_simulada()
{
    State atualS;
    Node *proximo, *atual, *aux;
    int origem, qtd_vizinhos, t, tam_caminho = 0;

    // init
    origem = this->escolhe_aleatorio(this->qtd_cidades);
    atual = ambiente->get_cidade(origem);
    caminho = new Node(origem, 0, nullptr);
    qtd_vizinhos = this->computa_qtd_vizinhos(atual);
    atualS.id = origem;
    atualS.weight = this->computa_media_caminhos_vizinhos(atual);

    t = ambiente->get_tam_mapa() * 1.5;
#ifdef DEBUG
    cout << "t " << t << endl;
#endif

    for (int i = 0; i < t; i++)
    {
        int proximo_vizinho = 0;
        atual = ambiente->get_cidade(atualS.id);
        qtd_vizinhos = this->computa_qtd_vizinhos(atual);

        do
        {
            proximo_vizinho = this->escolhe_aleatorio(qtd_vizinhos);
            int j;
            for (j = 0, aux = atual; j < proximo_vizinho; j++)
                aux = aux->next;

            proximo = aux;

#ifdef DEBUG
            cout << "id proximo " << proximo->id << endl;
            cout << "[ ";
            for (int k = 0; k < qtd_cidades; k++)
                cout << visitados[k] << " ";
            cout << "]" << endl;
#endif

        } while (visitados[proximo->id] && cidades_visitadas < qtd_cidades);
        if (proximo->id == origem && cidades_visitadas < qtd_cidades - 1)
            continue;

#ifdef DEBUG
        cout << "atual weight " << atualS.weight << endl;
#endif

        int delta_e = atualS.weight - proximo->weight;
        float prob = std::exp((float)delta_e / (t - i));

#ifdef DEBUG
        cout << "prob " << prob << endl;
#endif

        if (delta_e > 0 || (double)rand() / RAND_MAX <= prob)
        {
#ifdef DEBUG
            cout << "delta e " << delta_e << endl;
#endif

            atualS.id = proximo->id;
            atualS.weight = proximo->weight;
            visitados[proximo->id] = true;
            tam_caminho += atualS.weight;

            aux = caminho;
            while (aux->next != nullptr)
            {
                aux = aux->next;
            }

            aux->next = new Node(proximo->id, proximo->weight, nullptr);
            cidades_visitadas++;
        }

#ifdef DEBUG
        cout << endl;
#endif

        if (cidades_visitadas == qtd_cidades)
            return tam_caminho;
    }

    return tam_caminho;
}

void Agente::imprime_caminho()
{
    Node *aux;
    int tam = 0;

#ifdef DEBUG
    cout << "[ ";
    for (int k = 0; k < qtd_cidades; k++)
        cout << visitados[k] << " ";
    cout << "]" << endl;
#endif
    for (aux = caminho; aux != nullptr; aux = aux->next)
    {
        cout << aux->id << " (w: " << aux->weight << ") -> ";
        tam += aux->weight;
    }
    cout << endl;

    cout << "tamanho do caminho: " << tam << endl;
}