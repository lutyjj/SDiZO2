#include <iostream>
#include <conio.h>
#include <string>
#include <fstream>
#include "main.h"
#include "AdjacencyList.h"
#include "AdjacencyMatrix.h"
#include "Timer.h"
#include <windows.h>

using namespace std;

void menu_mst();
void menu_spt();
void menu_mst_entries();
void menu_spt_entries();

int main() {
	char option;
	do {
		cout << "==== MENU ====" << endl;
		cout << "1. MST" << endl;
		cout << "2. SPT" << endl;
		cout << "Enter: ";
		option = _getche();
		system("CLS");
		switch (option) {
		case '1':
			menu_mst();
			break;
		case '2':
			menu_spt();
			break;
		}

	} while (option != '0');

	return 0;
}

void menu_mst() {
	int opt;
	string fileName;
	int vertices, density;

	Timer timer;
	AdjacencyList* adjList = new AdjacencyList(0, false);
	AdjacencyMatrix* adjMatrix = new AdjacencyMatrix(0, false);

	do {
		cout << "=== MST ===" << endl;
		cout << "=== UNDIRECTED GRAPH ===" << endl << endl;
		cout << "Adjacency list representation: " << endl;
		adjList->display();
		cout << "Adjacency matrix representation: " << endl;
		adjMatrix->display();
		menu_mst_entries();
		cin >> opt;
		cout << std::endl;
		switch (opt) {
		case 1:
			cout << "Enter file name: ";
			cin >> fileName;
			adjList->readFromFile(fileName);
			adjMatrix->readFromFile(fileName);
			break;

		case 2:
			cout << "Enter vertices amount and density (%): ";
			cin >> vertices >> density;
			adjList = new AdjacencyList(vertices, false);
			adjMatrix = new AdjacencyMatrix(vertices, false);
			adjList->randomGraph(density);
			adjMatrix->randomGraph(density);
			break;

		case 3:
			timer.start();
			adjList->prim();
			timer.stop();
			timer.showTimeElapsed();

			timer.start();
			adjMatrix->prim();
			timer.stop();
			timer.showTimeElapsed();
			break;

		case 4:
			cout << "Enter source vertice: ";
			cin >> vertices;
			timer.start();
			adjList->dijkstra(vertices);
			timer.stop();
			timer.showTimeElapsed();
		}
		cout << endl;
	} while (opt != 0);
}

void menu_mst_entries() {
	cout << "1. Read from file" << endl;
	cout << "2. Generate random graph" << endl;
	cout << "3. Prim's algorithm" << endl;
	cout << "0. Back to menu" << endl;
	cout << "Enter: ";
}

void menu_spt() {
	int opt;
	string fileName;
	int vertices, density;

	Timer timer;
	AdjacencyList* adjList = new AdjacencyList(0, true);
	AdjacencyMatrix* adjMatrix = new AdjacencyMatrix(0, true);

	do {
		cout << "=== SPT ===" << endl;
		cout << "=== DIRECTED GRAPH ===" << endl << endl;
		cout << "Adjacency list representation: " << endl;
		adjList->display();
		cout << "Adjacency matrix representation: " << endl;
		adjMatrix->display();
		menu_spt_entries();
		cin >> opt;
		cout << endl;
		switch (opt) {
		case 1:
			cout << "Enter file name: ";
			cin >> fileName;
			adjList->readFromFile(fileName);
			adjMatrix->readFromFile(fileName);
			break;

		case 2:
			cout << "Enter vertices amount and density (%): ";
			cin >> vertices >> density;
			adjList = new AdjacencyList(vertices, true);
			adjMatrix = new AdjacencyMatrix(vertices, true);
			adjList->randomGraph(density);
			adjMatrix->randomGraph(density);
			break;

		case 3:
			cout << "Enter source vertice: ";
			cin >> vertices;
			timer.start();
			adjList->dijkstra(vertices);
			timer.stop();
			timer.showTimeElapsed();

			timer.start();
			adjMatrix->dijkstra(vertices);
			timer.stop();
			timer.showTimeElapsed();
		}
		cout << endl;
	} while (opt != 0);
}

void menu_spt_entries() {
	cout << "1. Read from file" << endl;
	cout << "2. Generate random graph" << endl;
	cout << "3. Dijkstra's algorithm" << endl;
	cout << "0. Back to menu" << endl;
	cout << "Enter: ";
}