#include "AdjacencyList.h"
#include <random>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

AdjacencyList::AdjacencyList(int vertices, bool isDirected) {
	this->adjList = new list < pair<int, int> >[vertices];
	this->edges = 0;
	this->srcVertice = 0;
	this->vertices = vertices;
	this->isDirected = isDirected;
}

void AdjacencyList::randomGraph(int density) {
	this->isDirected ? randomDirectedGraph(density) : randomUndirectedGraph(density);
}

void AdjacencyList::randomUndirectedGraph(int density) {
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

void AdjacencyList::randomDirectedGraph(int density) {
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

bool AdjacencyList::findEdge(int u, int v) {
	for (auto i = adjList[u].begin(); i != adjList[u].end(); i++) {
		if (i->first == v)
			return true;
	}
	return false;
}

void AdjacencyList::addEdge(int u, int v, int w) {
	this->adjList[u].push_back(std::make_pair(v, w));
	
	if (!isDirected)
		this->adjList[v].push_back(std::make_pair(u, w));
}

void AdjacencyList::display() {
	int v, w;
	for (int u = 0; u < this->vertices; u++) {
		cout << "Node " << u << " makes an edge with \n";
		for (auto i = this->adjList[u].begin(); i != this->adjList[u].end(); i++) {
			v = i->first;
			w = i->second;
			cout << "\tNode " << v << " with edge weight = " << w << endl;
		}
		cout << endl;
	}
}

void AdjacencyList::prim() {
	priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > >pq;
	int* keys = new int[this->vertices];
	int* parents = new int[this->vertices];
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

		for (auto i = this->adjList[u].begin(); i != this->adjList[u].end(); ++i) {
			int v = i->first;
			int weight = i->second;
			if (checked[v] == false && keys[v] > weight) {
				keys[v] = weight;
				parents[v] = u;
				pq.push(make_pair(keys[v], v));
			}
		}
	}

	int sum = 0;
	for (int i = 1; i < this->vertices; ++i) {
		cout << "Edge: " << parents[i] << " - " << i << ", weight: " << keys[i] << endl;
		sum += keys[i];
	}
	cout << "Sum: " << sum << endl;
}

void AdjacencyList::dijkstra(int start) {
	priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > >pq;
	int* distance = new int[this->vertices];
	int* parent = new int[this->vertices];
	bool* checked = new bool[this->vertices];

	for (int i = 0; i < this->vertices; i++) {
		distance[i] = INT_MAX;
		checked[i] = false;
		parent[i] = -1;
	}

	pq.push(make_pair(0, start));
	distance[start] = 0;
	while (!pq.empty()) {
		int u = pq.top().second;
		pq.pop();
		checked[u] = true;
		for (auto i = this->adjList[u].begin(); i != this->adjList[u].end(); ++i) {
			int v = i->first;
			int weight = i->second;
			if (!checked[v] && distance[v] && distance[v] > distance[u] + weight) {
				distance[v] = distance[u] + weight;
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
	cout << endl;
}

void AdjacencyList::displayPath(int parents[], int v) {
	if (parents[v] == -1) return;
	displayPath(parents, parents[v]);
	cout << " -> " << v;
}

void AdjacencyList::resetGraph(int edges, int vertices) {
	this->adjList = new list < pair<int, int> >[vertices];
	this->edges = edges;
	this->vertices = vertices;
}

void AdjacencyList::readFromFile(string fileName) {
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