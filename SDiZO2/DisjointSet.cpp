#include "DisjointSet.h"

DisjointSet::DisjointSet(int vertices) {
	this->vertices = vertices;
	this->parent = new int[vertices];
	this->rank = new int[vertices];
	for (int i = 0; i < vertices; i++) {
		rank[i] = 0;
		parent[i] = i;
	}
}

int DisjointSet::findSet(int v) {
	if (v != parent[v])
		parent[v] = findSet(parent[v]);
	return parent[v];
}

void DisjointSet::unionSet(int x, int y) {
	x = findSet(x);
	y = findSet(y);

	if (rank[x] > rank[y])
		parent[y] = x;
	else
		parent[x] = y;

	if (rank[x] == rank[y])
		rank[y]++;
}