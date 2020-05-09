#include "AdjacencyMatrix.h"
#include <random>
#include <iomanip>
#include <fstream>

using namespace std;

AdjacencyMatrix::AdjacencyMatrix(int vertices, bool isDirected) {
	this->adjMatrix = new int* [vertices];
	for (int i = 0; i < vertices; i++)
		adjMatrix[i] = new int[vertices];

	for (int x = 0; x < vertices; x++)
		for (int y = 0; y < vertices; y++)
			adjMatrix[x][y] = 0;

	this->edges = 0;
	this->srcVertice = 0;
	this->vertices = vertices;
	this->isDirected = isDirected;
}

void AdjacencyMatrix::randomGraph(int density) {
	this->isDirected ? randomDirectedGraph(density) : randomUndirectedGraph(density);
}

void AdjacencyMatrix::randomUndirectedGraph(int density) {
	random_device dev;
	mt19937 rng(dev());
	uniform_int_distribution<mt19937::result_type> distV(0, this->vertices - 1);
	uniform_int_distribution<mt19937::result_type> distW(1, 100);

	// creating spanning tree
	for (int i = 1; i < this->vertices; i++) {
		int w = distW(rng);
		addEdge(0, i, w);
	}

	int neededEdges = round(((1.0 * density / 100) * (vertices - 1) * vertices) / 2);
	while (neededEdges > vertices - 1) {
		int u = distV(rng);
		int v = distV(rng);
		int w = distW(rng);

		while (u == v)
			v = distV(rng);

		if (!findEdge(u, v) && !findEdge(v, u))
			addEdge(u, v, w);

		neededEdges--;
	}
}

void AdjacencyMatrix::randomDirectedGraph(int density) {
	random_device dev;
	mt19937 rng(dev());
	uniform_int_distribution<mt19937::result_type> distV(0, this->vertices - 1);
	uniform_int_distribution<mt19937::result_type> distW(1, 100);

	// creating spanning tree
	for (int i = 1; i < this->vertices; i++) {
		int w = distW(rng);
		addEdge(0, i, w);
	}

	int neededEdges = round((1.0 * density / 100) * (vertices - 1) * vertices);
	while (neededEdges > vertices - 1) {
		int u = distV(rng);
		int v = distV(rng);
		int w = distW(rng);

		while (u == v)
			v = distV(rng);

		if (findEdge(u, v)) {
			if (!findEdge(v, u))
				addEdge(v, u, w);
		}
		else
			addEdge(u, v, w);

		neededEdges--;
	}
}

void AdjacencyMatrix::addEdge(int u, int v, int w) {
	this->adjMatrix[u][v] = w;

	if (!isDirected)
		this->adjMatrix[v][u] = w;
}

bool AdjacencyMatrix::findEdge(int u, int v) {
	return (adjMatrix[u][v] != 0);
}

void AdjacencyMatrix::display() {
	for (int x = 0; x < vertices; x++) {
		for (int y = 0; y < vertices; y++)
			cout << setw(5) << this->adjMatrix[x][y];
		cout << endl;
	}
}

void AdjacencyMatrix::prim() {
	priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > >pq;
	int* keys = new int[this->vertices];
	int* parent = new int[this->vertices];
	bool* checked = new bool[this->vertices];

	for (int i = 0; i < this->vertices; i++) {
		keys[i] = INT_MAX;
		checked[i] = false;
	}

	pq.push(make_pair(0, 0));
	keys[0] = 0;
	while (!pq.empty()) {
		int u = pq.top().second;
		pq.pop();
		checked[u] = true;

		for (int v = 0; v < vertices; v++) {
			if (adjMatrix[u][v] && !checked[v] && adjMatrix[u][v] < keys[v]) {
				keys[v] = adjMatrix[u][v];
				pq.push(make_pair(keys[v], v));
				parent[v] = u;
			}
		}
	}

	int sum = 0;
	for (int i = 1; i < this->vertices; ++i) {
		cout << "Edge: " << parent[i] << " - " << i << ", weight: " << keys[i] << endl;
		sum += keys[i];
	}
	cout << "Sum: " << sum << endl;
}

void AdjacencyMatrix::dijkstra(int start) {
	priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > >pq;
	int* distance = new int[this->vertices];
	bool* checked = new bool[this->vertices];

	for (int i = 0; i < this->vertices; i++) {
		distance[i] = INT_MAX;
		checked[i] = false;
	}

	pq.push(make_pair(0, start));
	distance[start] = 0;
	while (!pq.empty()) {
		int u = pq.top().second;
		pq.pop();
		checked[u] = true;

		for (int v = 0; v < vertices; v++) {
			if (!checked[v] && adjMatrix[u][v]  && distance[v] > distance[u] + adjMatrix[u][v]) {
				distance[v] = distance[u] + adjMatrix[u][v];
				pq.push(make_pair(distance[v], v));
			}
		}
	}

	for (int i = 0; i < vertices; i++)
		cout << start << " -> " << i << " path: " << distance[i] << endl;
	cout << endl;
}

void AdjacencyMatrix::resetGraph(int edges, int vertices) {
	delete adjMatrix;
	this->adjMatrix = new int* [vertices];

	for (int i = 0; i < vertices; i++)
		adjMatrix[i] = new int[vertices];

	for (int x = 0; x < vertices; x++)
		for (int y = 0; y < vertices; y++)
			adjMatrix[x][y] = 0;

	this->edges = edges;
	this->vertices = vertices;
}

void AdjacencyMatrix::readFromFile(string fileName) {
	ifstream file;
	file.open(fileName);
	if (file) {
		int edges = 0;
		int vertices = 0;

		file >> edges >> vertices;
		resetGraph(edges, vertices);

		if (isDirected) {
			int vertice = 0;
			file >> vertice;
			this->srcVertice = vertice;
		}

		int i = 0;
		while (i < edges && file) {
			int u = 0, v = 0, w = 0;
			file >> u >> v >> w;
			addEdge(u, v, w);
			i++;
		}
	}
	else cerr << "Error while reading file";
	file.close();
}