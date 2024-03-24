#pragma once

class Node
{
public:
    int id, weight;
    Node *next;

    Node(int id, int weight, Node *next);
    ~Node();
};

class Grafo
{
public:
    int E, V;
    Node **Adj;
    void insere_aresta_direcionada(int origem, int destino, int peso);

    Grafo();
    Grafo(int tamanho);
    ~Grafo();

    Grafo *insere_aresta(int v1, int v2, int peso);
    void imprime();
};