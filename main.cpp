/*
 * main.cpp
 *
 *  Created on: 25 févr. 2017
 *      Author: tux
 */

#include <iostream>
#include "tsp.h"

using namespace std;

int main()
{
	srand(time(NULL)); //nombre aleatoire

	//creation du 1er graphe avec: vertex initial et nb de vertex
	Graph * graph1 = new Graph(5, 0);

	graph1->addEdge(0, 1, 1);
	graph1->addEdge(1, 0, 1);
	graph1->addEdge(0, 2, 3);
	graph1->addEdge(2, 0, 3);
	graph1->addEdge(0, 3, 4);
	graph1->addEdge(3, 0, 4);
	graph1->addEdge(0, 4, 5);
	graph1->addEdge(4, 0, 5);
	graph1->addEdge(1, 2, 1);
	graph1->addEdge(2, 1, 1);
	graph1->addEdge(1, 3, 4);
	graph1->addEdge(3, 1, 4);
	graph1->addEdge(1, 4, 8);
	graph1->addEdge(4, 1, 8);
	graph1->addEdge(2, 3, 5);
	graph1->addEdge(3, 2, 5);
	graph1->addEdge(2, 4, 1);
	graph1->addEdge(4, 2, 1);
	graph1->addEdge(3, 4, 2);
	graph1->addEdge(4, 3, 2);

	//creation du second graphe
	Graph * graph2 = new Graph(4, 0);

	graph2->addEdge(0, 1, 2);
	graph2->addEdge(1, 0, 2);
	graph2->addEdge(0, 2, 6);
	graph2->addEdge(2, 0, 6);
	graph2->addEdge(0, 3, 3);
	graph2->addEdge(3, 0, 3);
	graph2->addEdge(1, 3, 7);
	graph2->addEdge(3, 1, 7);
	graph2->addEdge(1, 2, 4);
	graph2->addEdge(2, 1, 4);
	graph2->addEdge(2, 3, 2);
	graph2->addEdge(3, 2, 2);

	//creation du troisieme graphe
	Graph * graph3 = new Graph(4, 0);

	graph3->addEdge(0, 1, 10);
	graph3->addEdge(1, 0, 10);
	graph3->addEdge(1, 3, 20);
	graph3->addEdge(3, 1, 20);
	graph3->addEdge(0, 2, 15);
	graph3->addEdge(2, 0, 15);
	graph3->addEdge(1, 3, 25);
	graph3->addEdge(3, 1, 25);
	graph3->addEdge(2, 3, 30);
	graph3->addEdge(3, 2, 30);
	graph3->addEdge(1, 2, 35);
	graph3->addEdge(2, 1, 35);

	//creation d'un graphe aleatoire (le booleen sert a generer le graphe)
	Graph * graph4 = new Graph(50, 0, true);
	graph4->showInfoGraph();

	//parametres du graphe: le graphe, la taille de la population, et les taux de generations et de mutations
	Genetic genetic(graph4, 10, 1000, 5, true);

	const clock_t begin_time = clock();
	genetic.run();

	cout << "\n\nTemps pour l'algo genetique: " << float(clock () - begin_time) /  CLOCKS_PER_SEC << " seconds";

	return 0;
}


