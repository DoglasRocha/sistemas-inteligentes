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

void Agente::computa_qtd_vizinhos_e_media(Node *cidade, int *qtd_vizinhos, int *media)
{
    *qtd_vizinhos = *media = 0;

    for (Node *aux = cidade; aux != nullptr; aux = aux->next)
    {
        (*qtd_vizinhos)++;
        *media += aux->weight;
    }

    *media /= *qtd_vizinhos;
}

int Agente::escolhe_aleatorio(int limite)
{
    return rand() % limite;
}

void Agente::andar()
{
    Node *proximo, *atual, *aux;
    int origem = this->escolhe_aleatorio(this->qtd_cidades);
    atual = ambiente->get_cidade(origem);
    caminho = new Node(origem, 0, nullptr);

    int t = 75;
    for (int i = 0; i < t; i++)
    {
        int qtd_vizinhos = 0, media = 0, proximo_vizinho = 0;
        atual = ambiente->get_cidade(atual->id);
        this->computa_qtd_vizinhos_e_media(atual, &qtd_vizinhos, &media);

        do
        {
            proximo_vizinho = this->escolhe_aleatorio(qtd_vizinhos);
            int j;
            for (j = 0, aux = atual; j < proximo_vizinho; j++)
                aux = aux->next;

            proximo = aux;
#ifdef DEBUG
            cout << "id proximo" << proximo->id << endl;
#endif

#ifdef DEBUG
            cout << "[ ";
            for (int k = 0; k < qtd_cidades; k++)
                cout << visitados[k] << " ";
            cout << "]" << endl;
#endif
        } while (visitados[proximo->id] && cidades_visitadas < qtd_cidades);
        if (proximo->id == origem && cidades_visitadas < qtd_cidades - 1)
            continue;

            // proximo = atual;

#ifdef DEBUG
        cout << "media " << media << endl;
#endif
        int delta_e = -(proximo->weight - media);
        float prob = std::exp((float)delta_e / (t - i));

#ifdef DEBUG
        cout << "prob " << prob << endl;
#endif
        if (delta_e > 0 || (double)rand() / RAND_MAX <= prob)
        {
#ifdef DEBUG
            cout << "delta e " << delta_e << endl;
#endif
            atual = proximo;
            visitados[proximo->id] = true;

            Node *aux = caminho;
            while (aux->next != nullptr)
            {
                aux = aux->next;
            }

            aux->next = new Node(proximo->id, proximo->weight, nullptr);
            cidades_visitadas++;
        }

        if (cidades_visitadas == qtd_cidades)
            return;
    }
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