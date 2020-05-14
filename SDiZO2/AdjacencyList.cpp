#include "AdjacencyList.h"
#include "DisjointSet.h"
#include <random>
#include <fstream>
#include <string>

using namespace std;

AdjacencyList::AdjacencyList(int vertices, bool isDirected) {
	this->adjList = new list < pair<int, int> >[vertices];
	this->edges = 0;
	this->srcVertice = 0;
	this->vertices = vertices;
	this->isDirected = isDirected;
}

void AdjacencyList::addEdge(int u, int v, int w) {
	this->adjList[u].push_back(make_pair(v, w));

	// If graph is not directed, also connect vertices vice versa
	if (!isDirected)
		this->adjList[v].push_back(make_pair(u, w));

	this->edges++;
}

// Check if graph has an (u,v) edge
bool AdjacencyList::findEdge(int u, int v) {
	// Cycle trough list of edges with U vertice
	for (auto i = adjList[u].begin(); i != adjList[u].end(); i++) {
		if (i->first == v)
			return true;
	}
	return false;
}

void AdjacencyList::resetGraph(int edges, int vertices) {
	this->adjList = new list < pair<int, int> >[vertices];
	this->edges = edges;
	this->vertices = vertices;
}

void AdjacencyList::display() {
	for (int u = 0; u < this->vertices; u++) {
		cout << "Vertice " << u << ":" << endl;
		for (auto i = this->adjList[u].begin(); i != this->adjList[u].end(); i++) {
			auto v = i->first;
			auto w = i->second;
			cout << "\t -> " << v << ", weight = " << w << endl;
		}
		cout << endl;
	}
}

void AdjacencyList::prim() {
	priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > >pq;
	int* mst = new int[this->vertices];
	int* parents = new int[this->vertices];
	bool* checked = new bool[this->vertices];

	for (int i = 0; i < this->vertices; i++) {
		mst[i] = INT_MAX;
		checked[i] = false;
		parents[i] = -1;
	}

	// Start from first vertice
	pq.push(make_pair(0, 0));
	mst[0] = 0;

	while (!pq.empty()) {
		int u = pq.top().second;
		pq.pop();
		checked[u] = true;

		// Cycle trough every edge
		for (auto i = this->adjList[u].begin(); i != this->adjList[u].end(); ++i) {
			int v = i->first;
			int weight = i->second;
			if (checked[v] == false && mst[v] > weight) {
				mst[v] = weight;
				parents[v] = u;
				pq.push(make_pair(mst[v], v));
			}
		}
	}

	int sum = 0;
	for (int i = 1; i < this->vertices; ++i) {
		cout << "Edge: " << parents[i] << " - " << i << ", weight: " << mst[i] << endl;
		sum += mst[i];
	}
	cout << "Sum: " << sum << endl;
}


// Custom sorting for (u (v, w)) style of list
bool sortByWeightM(const pair<int, pair<int, int>>& a, const pair<int, pair<int, int>>& b) {
	return (a.second.second < b.second.second);
}

void AdjacencyList::kruskal() {
	int sum = 0;
	int i = 0;
	vector<pair<int, pair<int, int>>> edges;
	DisjointSet* sets = new DisjointSet(vertices);

	// Copy vertices from list to vector for faster & easier sorting
	while (i < vertices) {
		for (auto u = this->adjList[i].begin(); u != this->adjList[i].end(); u++) {
			edges.push_back(make_pair(i, *u));
		}
		i++;
	}

	// Sort edges
	sort(edges.begin(), edges.end(), sortByWeightM);

	for (auto it = edges.begin(); it != edges.end(); it++) {
		int u = it->first;
		int v = it->second.first;

		int uSet = sets->findSet(u);
		int vSet = sets->findSet(v);

		if (uSet != vSet) {
			int weight = it->second.second;
			sum += weight;
			cout << "Edge: " << u << " - " << v << ", weighs: " << weight << endl;

			sets->unionSet(uSet, vSet);
		}
	}

	cout << "Sum: " << sum << endl;
}

void AdjacencyList::dijkstra(int start) {
	priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > >pq;
	int* distance = new int[this->vertices];
	int* parents = new int[this->vertices];
	bool* checked = new bool[this->vertices];

	for (int i = 0; i < this->vertices; i++) {
		distance[i] = INT_MAX;
		parents[i] = -1;
		checked[i] = false;
	}

	// Start from user-entered vertice
	pq.push(make_pair(0, start));
	distance[start] = 0;
	while (!pq.empty()) {
		int u = pq.top().second;
		pq.pop();
		checked[u] = true;

		// Cycle through every edge
		for (auto i = this->adjList[u].begin(); i != this->adjList[u].end(); ++i) {
			int v = i->first;
			int weight = i->second;
			// Relaxation of an edge
			if (!checked[v] && distance[v] && distance[v] > distance[u] + weight) {
				distance[v] = distance[u] + weight;
				parents[v] = u;
				pq.push(make_pair(distance[v], v));
			}
		}
	}

	// Show result
	for (int i = 0; i < vertices; i++) {
		cout << "Path " << start;
		if (distance[i] != INT_MAX) {
			displayPath(parents, i);
			cout << ", weighs: " << distance[i] << endl;
		}
		else
			cout << " !-> " << i << endl;
	}
}

void AdjacencyList::bellman(int start) {
	int* d = new int[this->vertices];
	int* p = new int[this->vertices];
	for (int i = 0; i < this->vertices; i++) {
		d[i] = INT_MAX;
		p[i] = -1;
	}
	d[start] = 0;

	// Relax every edge V-1 times
	for (int i = 1; i < vertices - 1; i++) {
		for (int u = 0; u < vertices; u++) {
			for (auto it = adjList[u].begin(); it != adjList[u].end(); it++) {
				auto v = it->first;
				auto w = it->second;
				if (d[v] > d[u] + w && d[u] != INT_MAX) {
					d[v] = d[u] + w;
					p[v] = u;
				}
			}
		}
	}

	// Search for negative edges
	for (int u = 0; u < vertices; u++) {
		for (auto it = adjList[u].begin(); it != adjList[u].end(); it++) {
			auto v = it->first;
			auto w = it->second;

			if (d[v] > d[u] + w)
				cout << "Negative edges found." << endl;
			return;
		}
	}

	// Show result
	for (int i = 0; i < vertices; i++) {
		cout << "Path " << start;
		if (d[i] != INT_MAX) {
			displayPath(p, i);
			cout << ", weighs: " << d[i] << endl;
		}
	}
}

void AdjacencyList::displayPath(int* parents, int v) {
	// Stop if no parent
	if (parents[v] == -1) 
		return;
	displayPath(parents, parents[v]);
	cout << " -> " << v;
}

void AdjacencyList::readFromFile(string fileName) {
	ifstream file;
	file.open(fileName);

	if (file) {
		int edges = 0;
		int vertices = 0;

		file >> edges >> vertices;
		// Reset current graph to a new one with properties from file
		resetGraph(edges, vertices);

		if (isDirected) {
			int vertice = 0;
			file >> vertice;
			this->srcVertice = vertice;
		}

		// Add edges till eof
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