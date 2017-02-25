/*
 * tsp.h
 *
 *  Created on: 25 févr. 2017
 *      Author: tux
 */

#ifndef TSP_H
#define TSP_H

#include <vector>
#include <map>
#include <set>
#include <utility>
#include <time.h>
#include <stdlib.h>

class Graph
{
private:
	int V; // numero de vertices
	int total_edges; // total des cotes
	int initial_vertex; // vertex initial

	std::map<std::pair<int, int>, int> map_edges; // mapping des cotes

public:
	Graph(int V, int initial_vertex, bool random_graph = false); // constructeur

	void addEdge(int v1, int v2, int weight); //ajout des cotes
	void showGraph();
	void generatesGraph(); //genere graphe aleatoire
	void showInfoGraph();

	int existsEdge(int src, int dest);

	friend class Genetic; //acces aux memebres prives de la classe
};

typedef std::pair<std::vector<int>, int> my_pair;

//tri des vecteurs par paire
struct sort_pred
{
	bool operator()(const my_pair& firstElem, const my_pair& secondElem)
	{
		return firstElem.second < secondElem.second;
	}
};

//classe de l'algo genetique
class Genetic
{
private:
	Graph* graph;

	std::vector< my_pair > population; //chaque element est une paire: vecteur et cout total

	int size_population;
	int real_size_population;
	int generations; //nb de generation
	int mutation_rate; //taux de mutation
	bool show_population;

private:
	void initialPopulation();

public:
	Genetic(Graph* graph, int amount_population, int generations, int mutation_rate, bool show_population = true); //constructeur

	int isValidSolution(std::vector<int>& solution);

	void showPopulation();
	void crossOver(std::vector<int>& parent1, std::vector<int>& parent2);
	void insertBinarySearch(std::vector<int>& child, int total_cost);
	void run();

	int getCostBestSolution(); //donne le cout de la meilleure solution

	bool existsChromosome(const std::vector<int> & v);
};

#endif
