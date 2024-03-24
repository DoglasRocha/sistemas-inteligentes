#include "../include/grafo.hpp"
#include <iostream>

using namespace std;

Node::Node(int id, int weight, Node *next)
{
    this->id = id;
    this->weight = weight;
    this->next = next;
}

Node::~Node()
{
    if (this->next != nullptr)
        delete this->next;
}

Grafo::Grafo(int tamanho)
{
    this->E = 0;
    this->V = tamanho;
    this->Adj = new Node *[tamanho];

    for (int v = 0; v < tamanho; v++)
        this->Adj[v] = nullptr;
}

Grafo::~Grafo()
{
    for (int i = 0; i < this->V; i++)
        delete this->Adj[i];

    delete this->Adj;
}

Grafo *Grafo::insere_aresta(int origem, int destino, int peso)
{
    Node *aux;
    if (origem >= this->V || origem >= this->V)
        return;

    this->E++;
    aux = this->Adj[origem];
    if (aux == nullptr)
        this->Adj[origem] = new Node(destino, peso, nullptr);
    else
    {
        while (aux->next != nullptr)
            aux = aux->next;

        aux->next = new Node(destino, peso, nullptr);
    }

    return this;
}

void Grafo::imprime()
{
    Node *aux;
    for (int i = 0; i < this->V; i++)
    {
        cout << i << " -> ";
        // printf("%2d -> ", i);

        aux = this->Adj[i];
        while (aux != NULL)
        {
            cout << aux->id << ": (w:" << aux->weight << "), ";
            // printf("%2d: (w:%2d), ", aux->id, aux->weight);
            aux = aux->next;
        }
        cout << endl;
        // printf("\n");
    }
}