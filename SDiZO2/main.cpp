#include <iostream>
#include <conio.h>
#include <string>
#include <fstream>
#include "main.h"
#include "AdjacencyList.h"
#include "AdjacencyMatrix.h"
#include "GraphUtils.h"
#include "Timer.h"
#include "GraphTest.h"
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
		cout << "3. Start batch test" << endl;
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
		case '3':
			GraphTest test;
			test.batchTest();
			break;
		}

	} while (option != '0');

	return 0;
}

void menu_mst() {
	int opt, v, density;
	string fileName;

	Timer timer;
	GraphUtils utils;
	AdjacencyList* adjList = new AdjacencyList(0, false);
	AdjacencyMatrix* adjMatrix = new AdjacencyMatrix(0, false);

	do {
		cout << "=== MST ===" << endl;
		cout << "=== UNDIRECTED GRAPH ===" << endl << endl;
		menu_mst_entries();
		cin >> opt;
		cout << std::endl;
		switch (opt) {
		case 1:
			cout << "Enter file name: ";
			cin >> fileName;
			adjList->readFromFile(fileName);
			adjMatrix->readFromFile(fileName); 
			cout << "Adjacency list representation: " << endl;
			adjList->display();
			cout << "Adjacency matrix representation: " << endl;
			adjMatrix->display();
			break;

		case 2:
			cout << "Enter vertices amount: ";
			cin >> v;
			cout << "Enter density (%): ";
			cin >> density;
			utils.randomGraph(v, density, false);
			adjList = utils.getAdjacencyList();
			adjMatrix = utils.getAdjacencyMatrix();
			cout << "Adjacency list representation: " << endl;
			adjList->display();
			cout << "Adjacency matrix representation: " << endl;
			adjMatrix->display();
			break;

		case 3:
			cout << "Adjacency list representation: " << endl;
			adjList->display();
			cout << "Adjacency matrix representation: " << endl;
			adjMatrix->display();
			break;

		case 4:
			timer.start();
			adjList->prim();
			timer.stop();
			timer.showTimeElapsed();

			timer.start();
			adjMatrix->prim();
			timer.stop();
			timer.showTimeElapsed();
			break;

		case 5:
			timer.start();
			adjList->kruskal();
			timer.stop();
			timer.showTimeElapsed();

			timer.start();
			adjMatrix->kruskal();
			timer.stop();
			timer.showTimeElapsed();
			break;

		case 6:
			GraphTest test;
			// 0 - Prim test
			test.startTest(20, 5, 0);
			test.startTest(60, 5, 0);
			test.startTest(99, 5, 0);
			// 1 - Kruskal test
			test.startTest(20, 5, 1);
			test.startTest(60, 5, 1);
			test.startTest(99, 5, 1);
			cout << "Test finished." << endl;
			break;
		}
		cout << endl;
	} while (opt != 0);
}

void menu_mst_entries() {
	cout << "1. Read from file" << endl;
	cout << "2. Generate random graph" << endl;
	cout << "3. Show graph" << endl;
	cout << "4. Prim's algorithm" << endl;
	cout << "5. Kruskal's algorithm" << endl;
	cout << "6. Run test" << endl;
	cout << "0. Back to menu" << endl;
	cout << "Enter: ";
}

void menu_spt() {
	int opt, v, density;
	string fileName;

	Timer timer;
	GraphUtils utils;
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
			cout << "Adjacency list representation: " << endl;
			adjList->display();
			cout << "Adjacency matrix representation: " << endl;
			adjMatrix->display();
			break;

		case 2:
			cout << "Enter vertices amount: ";
			cin >> v;
			cout << "Enter density (%): ";
			cin >> density;
			utils.randomGraph(v, density, true);
			adjList = utils.getAdjacencyList();
			adjMatrix = utils.getAdjacencyMatrix();
			cout << "Adjacency list representation: " << endl;
			adjList->display();
			cout << "Adjacency matrix representation: " << endl;
			adjMatrix->display();
			break;

		case 3:
			cout << "Adjacency list representation: " << endl;
			adjList->display();
			cout << "Adjacency matrix representation: " << endl;
			adjMatrix->display();
			break;

		case 4:
			cout << "Enter source vertice: ";
			cin >> v;
			timer.start();
			adjList->dijkstra(v);
			timer.stop();
			timer.showTimeElapsed();

			timer.start();
			adjMatrix->dijkstra(v);
			timer.stop();
			timer.showTimeElapsed();
			break;

		case 5:
			cout << "Enter source vertice: ";
			cin >> v;
			timer.start();
			adjList->bellman(v);
			timer.stop();
			timer.showTimeElapsed();

			timer.start();
			adjMatrix->bellman(v);
			timer.stop();
			timer.showTimeElapsed();
			break;

		case 6:
			GraphTest test;
			// 2 - Dijstra test
			test.startTest(20, 5, 2);
			test.startTest(60, 5, 2);
			test.startTest(99, 5, 2);
			// 3 - Bellman-Ford test
			test.startTest(20, 5, 3);
			test.startTest(60, 5, 3);
			test.startTest(99, 5, 3);
			cout << "Test finished." << endl;
			break;
		}
		cout << endl;
	} while (opt != 0);
}

void menu_spt_entries() {
	cout << "1. Read from file" << endl;
	cout << "2. Generate random graph" << endl;
	cout << "3. Show graph" << endl;
	cout << "4. Dijkstra's algorithm" << endl;
	cout << "5. Bellman-Ford's algorithm" << endl;
	cout << "6. Run test" << endl;
	cout << "0. Back to menu" << endl;
	cout << "Enter: ";
}
