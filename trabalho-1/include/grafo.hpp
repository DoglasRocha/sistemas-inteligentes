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
    int E, V;
    Node **Adj;

public:
    Grafo(int tamanho);
    ~Grafo();

    Grafo *insere_aresta(int origem, int destino, int peso);
    void imprime();
};