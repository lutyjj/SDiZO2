#pragma once
struct DisjointSet {
	int* parent;
	int* rank;
	int vertices;

	DisjointSet(int vertices);
	int findSet(int v);
	void unionSet(int x, int y);
};
