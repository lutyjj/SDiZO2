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
	int opt, value;
	string fileName;

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
			cout << "Enter vertices amount: ";
			cin >> value;
			adjList = new AdjacencyList(value, true);
			adjMatrix = new AdjacencyMatrix(value, true);
			cout << "Enter density (%): ";
			cin >> value;
			adjList->randomGraph(value);
			adjMatrix->randomGraph(value);
			break;

		case 3:
			adjList->display();
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
		}
		cout << endl;
	} while (opt != 0);
}

void menu_mst_entries() {
	cout << "1. Read from file" << endl;
	cout << "2. Generate random graph" << endl;
	cout << "3. Show graph" << endl;
	cout << "4. Prim's algorithm" << endl;
	cout << "0. Back to menu" << endl;
	cout << "Enter: ";
}

void menu_spt() {
	int opt, value;
	string fileName;

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
			cout << "Enter vertices amount: ";
			cin >> value;
			adjList = new AdjacencyList(value, true);
			adjMatrix = new AdjacencyMatrix(value, true);
			cout << "Enter density (%): ";
			cin >> value;
			adjList->randomGraph(value);
			adjMatrix->randomGraph(value);
			break;

		case 3:
			adjList->display();
			adjMatrix->display();
			break;

		case 4:
			cout << "Enter source vertice: ";
			cin >> value;
			timer.start();
			adjList->dijkstra(value);
			timer.stop();
			timer.showTimeElapsed();

			timer.start();
			adjMatrix->dijkstra(value);
			timer.stop();
			timer.showTimeElapsed();
		}
		cout << endl;
	} while (opt != 0);
}

void menu_spt_entries() {
	cout << "1. Read from file" << endl;
	cout << "2. Generate random graph" << endl;
	cout << "3. Show graph" << endl;
	cout << "4. Dijkstra's algorithm" << endl;
	cout << "0. Back to menu" << endl;
	cout << "Enter: ";
}