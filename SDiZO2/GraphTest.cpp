#include "GraphTest.h"
#include "Timer.h"
#include "AdjacencyList.h"
#include "AdjacencyMatrix.h"
#include "GraphUtils.h"
#include <fstream>
#include <random>

using namespace std;

void GraphTest::batchTest() {
	// Test every algoritm [Prim, Kruskal, Dijkstra, Bellman-Ford]
	for (int alg = 0; alg < 4; alg++) {
		startTest(20, 5, alg);
		startTest(60, 5, alg);
		startTest(99, 5, alg);
	}
}

void GraphTest::startTest(int density, int V, int choice) {
	ofstream log, avg;
	string avgFileName, logFileName;
	if (choice == 0) {
		avgFileName = "Prim_Avg.txt";
		logFileName = "Prim_Full.txt";
	}
	else if (choice == 1) {
		avgFileName = "Kruskal_Avg.txt";
		logFileName = "Kruskal_Full.txt";
	}
	else if (choice == 2) {
		avgFileName = "Dijkstra_Avg.txt";
		logFileName = "Dijkstra_Full.txt";
	}
	else if (choice == 3) {
		avgFileName = "BellmanFord_Avg.txt";
		logFileName = "BellmanFord_Full.txt";
	}
	avg.open(avgFileName, ios_base::app);
	log.open(logFileName, ios_base::app);

	random_device dev;
	mt19937 rng(dev());

	Timer timerL, timerM;
	GraphUtils utils;
	AdjacencyList* adjList;
	AdjacencyMatrix* adjMatrix;

	int timesToRepeat = 100;
	long double counterL = 0, counterM = 0;

	for (int j = 0; j < 7; j++) {
		uniform_int_distribution<std::mt19937::result_type> dist(0, V - 1);
		log << "V count: " << V << endl;
		log << "Density: " << density << "%" << endl;;
		for (int i = 0; i < timesToRepeat; i++) {
			utils.randomGraph(V, density, false);
			adjList = utils.getAdjacencyList();
			adjMatrix = utils.getAdjacencyMatrix();

			if (choice == 0) {
				timerL.start();
				adjList->prim();
				timerL.stop();

				timerM.start();
				adjMatrix->prim();
				timerM.stop();
			}
			else if (choice == 1) {
				timerL.start();
				adjList->kruskal();
				timerL.stop();

				timerM.start();
				adjMatrix->kruskal();
				timerM.stop();
			}
			else if (choice == 2) {
				int v = dist(rng);

				timerL.start();
				adjList->dijkstra(v);
				timerL.stop();

				timerM.start();
				adjMatrix->dijkstra(v);
				timerM.stop();
			}
			else if (choice == 3) {
				int v = dist(rng);

				timerL.start();
				adjList->bellman(v);
				timerL.stop();

				timerM.start();
				adjMatrix->bellman(v);
				timerM.stop();
			}
			
			log << "List: " << timerL.timeElapsed << " [ms]" << endl;
			counterL += timerL.timeElapsed;
			log << "Matrix: " << timerM.timeElapsed << " [ms]" << endl;
			counterM += timerM.timeElapsed;
		}
		avg << "V count: " << V << endl;
		avg << "Density: " << density << "%" << endl;
		avg << "Avg. time: " << endl;
		avg << "list = " << counterL / timesToRepeat << " [ms]" << endl;
		avg << "matrix = " << counterM / timesToRepeat << " [ms]" << endl << endl;
		V += 5;
	}
}

