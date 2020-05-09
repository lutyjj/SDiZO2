#pragma once
#include <iostream>
#include <list>
#include <queue>

using namespace std;

struct AdjacencyMatrix {
	AdjacencyMatrix(int vertices, bool isDirected);
	void readFromFile(string fileName);
	void display();
	void prim();
	void dijkstra(int start);
	void randomGraph(int density);

private:
	int edges;
	int vertices;
	int srcVertice = 0;
	bool isDirected = false;
	int** adjMatrix;

	void addEdge(int u, int v, int w);
	bool findEdge(int u, int v);
	void resetGraph(int edges, int vertices);
	void randomUndirectedGraph(int density);
	void randomDirectedGraph(int density);
};