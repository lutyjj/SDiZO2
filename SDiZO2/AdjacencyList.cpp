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

	this->edges++;
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
		parents[i] = -1;
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
		cout << "edge: " << parents[i] << " - " << i << ", weight: " << keys[i] << endl;
		sum += keys[i];
	}
	cout << "sum: " << sum << endl;
}

void AdjacencyList::dijkstra(int start) {
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

bool sortByWeightM(const pair<int, pair<int,int>>& a, const pair<int, pair<int, int>>& b) {
	return (a.second.second < b.second.second);
}

void AdjacencyList::kruskal() {
	int sum = 0;
	int i = 0;
	vector<pair<int, pair<int, int>>> edges;
	DisjointSet* ds = new DisjointSet(vertices);

	while (i < vertices) {
		for (auto u = this->adjList[i].begin(); u != this->adjList[i].end(); u++) {
			edges.push_back(make_pair(i, *u));
		}
		i++;
	}

	sort(edges.begin(), edges.end(), sortByWeightM);

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

void AdjacencyList::bellman(int start) {
	int* d = new int[this->vertices];
	int* p = new int[this->vertices];
	for (int i = 0; i < vertices; i++) {
		d[i] = INT_MAX;
		p[i] = -1;
	}
	d[start] = 0;

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

	for (int u = 0; u < vertices; u++) {
		for (auto it = adjList[u].begin(); it != adjList[u].end(); it++) {
			auto v = it->first;
			auto w = it->second;

			if (d[v] > d[u] + w)
				cout << "Negative edges found." << endl;
				return;
		}
	}

	for (int i = 0; i < vertices; i++) {
		cout << "Path " << start;
		if (d[i] != INT_MAX) {
			displayPath(p, i);
			cout << ", weighs: " << d[i] << endl;
		}
		else
			cout << " !-> " << i << endl;
	}
}