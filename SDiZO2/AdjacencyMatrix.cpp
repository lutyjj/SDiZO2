#include "AdjacencyMatrix.h"
#include "DisjointSet.h"
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

void AdjacencyMatrix::addEdge(int u, int v, int w) {
	this->adjMatrix[u][v] = w;

	if (!isDirected)
		this->adjMatrix[v][u] = w;

	this->edges++;
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
	int* parent = new int[this->vertices];
	bool* checked = new bool[this->vertices];

	for (int i = 0; i < this->vertices; i++) {
		distance[i] = INT_MAX;
		parent[i] = -1;
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
				parent[v] = u;
				pq.push(make_pair(distance[v], v));
			}
		}
	}

	for (int i = 0; i < vertices; i++) {
		cout << "Path " << start;
		if (distance[i] != INT_MAX) {
			displayPath(parent, i);
			cout << ", weighs: " << distance[i] << endl;
		}
		else
			cout << " !-> " << i << endl;
	}
}

void AdjacencyMatrix::displayPath(int parent[], int v) {
	if (parent[v] == -1) return;
	displayPath(parent, parent[v]);
	cout << " -> " << v;
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

bool sortByWeight(const pair<int, pair<int, int>>& a, const pair<int, pair<int, int>>& b) {
	return (a.second.second < b.second.second);
}

void AdjacencyMatrix::kruskal() {
	int sum = 0;
	int i = 0;
	DisjointSet* ds = new DisjointSet(vertices);

	vector<pair<int, pair<int, int>>> edges;
	for (int i = 0; i < vertices; i++) {
		for (int j = 0; j < vertices; j++) {
			if (adjMatrix[i][j] != 0) {
				auto u = make_pair(j, adjMatrix[i][j]);
				edges.push_back(make_pair(i, u));
			}
		}
	}

	sort(edges.begin(), edges.end(), sortByWeight);

	for (auto it = edges.begin(); it != edges.end(); it++) {
		int u = it->first;
		int v = it->second.first;

		int uSet = ds->findSet(u);
		int vSet = ds->findSet(v);

		if (uSet != vSet) {
			int weight = it->second.second;
			sum += weight;
			cout << "Edge: " << u << " - " << v << ", weighs: " << weight << endl;

			ds->unionSet(uSet, vSet);
		}
	}

	cout << "Sum: " << sum << endl;
}

void AdjacencyMatrix::bellman(int start) {
	int* d = new int[this->vertices];
	int* p = new int[this->vertices];
	for (int i = 0; i < this->vertices; i++) {
		d[i] = INT_MAX;
		p[i] = -1;
	}
	d[start] = 0;
	
	for (int i = 0; i < vertices - 1; i++) {
		for (int v = 0; v < vertices; v++) {
			for (int u = 0; u < vertices; u++) {
				if (adjMatrix[u][v] != 0 && d[u] != INT_MAX && d[v] > d[u] + adjMatrix[u][v]) {
					d[v] = d[u] + adjMatrix[u][v];
					p[v] = u;
				}
			}
		}
	}

	for (int v = 0; v < vertices; v++) {
		for (int u = 0; u < vertices; u++) {
			if (adjMatrix[u][v] != 0 && d[v] > d[u] + adjMatrix[u][v]) {
				cout << "Negative edges found." << endl;
				return;
			}
		}
	}

	for (int i = 0; i < vertices; i++) {
		cout << "Path " << start;
		if (d[i] != INT_MAX) {
			displayPath(p, i);
			cout << ", weighs: " << d[i] << endl;
		}
	}
}