#pragma once
#include "AdjacencyList.h"
#include "AdjacencyMatrix.h"
#include <random>

// Helper struct to generate one graph for both representations

struct GraphUtils {
	AdjacencyList* adjList;
	AdjacencyMatrix* adjMatrix;
	bool isDirected = false;;
	int vertices = 0;
	
	GraphUtils() {
		this->adjList = new AdjacencyList(0, false);
		this->adjMatrix = new AdjacencyMatrix(0, false);
	}

	void randomGraph(int vertices, int density, bool isDirected) {
		this->adjList = new AdjacencyList(vertices, isDirected);
		this->adjMatrix = new AdjacencyMatrix(vertices, isDirected);
		this->isDirected = isDirected;
		this->vertices = vertices;

		this->isDirected ? randomDirectedGraph(density) : randomUndirectedGraph(density);
	}

	// Add edge to both representations
	void addEdge(int u, int v, int w) {
		adjList->addEdge(u, v, w);
		adjMatrix->addEdge(u, v, w);
	}

	// Find edge from both representations
	bool findEdge(int u, int v) {
		return (adjList->findEdge(u, v) && adjMatrix->findEdge(u, v));
	}

	void randomDirectedGraph(int density) {
		random_device dev;
		mt19937 rng(dev());
		uniform_int_distribution<mt19937::result_type> distV(0, this->vertices - 1);
		uniform_int_distribution<mt19937::result_type> distW(1, 20);

		// Create spanning tree
		for (int i = 1; i < this->vertices; i++) {
			int w = distW(rng);
			addEdge(0, i, w);
		}

		// Density = E / V*(V-1)
		// E = D*V*(V-1)
		int neededEdges = round((1.0 * density / 100) * (vertices - 1) * vertices);
		while (neededEdges > vertices - 1) {
			int u = distV(rng);
			int v = distV(rng);
			int w = distW(rng);

			while (u == v)
				v = distV(rng);

			// Check if we already have edge (u,v)
			if (findEdge(u, v)) {
				// Check if we already have edge (v,u)
				if (!findEdge(v, u))
					// Add (v,u) if we only have (u,v)
					addEdge(v, u, w);
			}
			else
				// Add (u,v)
				addEdge(u, v, w);

			neededEdges--;
		}
	}

	void randomUndirectedGraph(int density) {
		random_device dev;
		mt19937 rng(dev());
		uniform_int_distribution<mt19937::result_type> distV(0, this->vertices - 1);
		uniform_int_distribution<mt19937::result_type> distW(1, 20);

		// Create spanning tree
		for (int i = 1; i < this->vertices; i++) {
			int w = distW(rng);
			addEdge(0, i, w);
		}

		// Density = 2E / V*(V-1)
		// E = D*V*(V-1) / 2
		int neededEdges = round(((1.0 * density / 100) * (vertices - 1) * vertices) / 2);
		while (neededEdges > vertices - 1) {
			int u = distV(rng);
			int v = distV(rng);
			int w = distW(rng);

			while (u == v)
				v = distV(rng);

			// Check if we have (u,v) in both directions
			if (!findEdge(u, v) && !findEdge(v, u))
				// Add if no
				addEdge(u, v, w);

			neededEdges--;
		}
	}

	AdjacencyList* getAdjacencyList() {
		return adjList;
	}

	AdjacencyMatrix* getAdjacencyMatrix() {
		return adjMatrix;
	}
};

