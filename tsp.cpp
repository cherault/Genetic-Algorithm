/*
 * tsp.cpp
 *
 *  Created on: 25 févr. 2017
 *      Author: tux
 */

#include <iostream>
#include <algorithm>
#include "tsp.h"

using namespace std;

Graph::Graph(int V, int initial_vertex, bool random_graph)
{
	if(V < 1) //valide si le nb de vertex et inf. a 1
	{
		cout << "Erreurr: nb de vertex <= 0\n";
		exit(1);
	}

	this->V = V; //nb de vertices
	this->initial_vertex = initial_vertex;
	this->total_edges = 0;

	if(random_graph)
		generatesGraph();
}


void Graph::generatesGraph()
{
	vector<int> vec;

	//creation du vecteur
	for(int i = 0; i < V; i++)
		vec.push_back(i);

	//permutation aleatoire
	random_shuffle(vec.begin(), vec.end());

	initial_vertex = vec[0]; //updates vertex initial

	int i, weight;

	for(i = 0; i <= V; i++)
	{
		weight = rand() % V + 1; //poids aleatoire [1,V]

		if(i + 1 < V)
			addEdge(vec[i], vec[i + 1], weight);
		else
		{
			//ajoute le derneior cote
			addEdge(vec[i], vec[0], weight);
			break;
		}
	}

	int limit_edges = V * (V - 1); //calcul la limite des cotes
	int size_edges = rand() % (2 * limit_edges) + limit_edges;

	//ajoute aleatoirement d'autres cotes
	for(int i = 0; i < size_edges; i++)
	{
		int src = rand() % V; //source aleatoire
		int dest = rand() % V; //destination aleatoire
		weight = rand() % V + 1;

		if(src != dest)
		{
			addEdge(vec[src], vec[dest], weight);
			addEdge(vec[dest], vec[src], weight);
		}
	}
}

void Graph::showInfoGraph()
{
	cout << "Montre les infos du graphe:\n\n";
	cout << "Nb de vertices: " << V;
	cout << "\nNb de cotes: " << map_edges.size() << "\n";
}

void Graph::addEdge(int src, int dest, int weight)
{
	map_edges[make_pair(src, dest)] = weight; //ajoute un cote dans le mapping du graphe
}

void Graph::showGraph() //montre les connexions du graphe
{
	map<pair<int, int>, int>::iterator it;

	for(it = map_edges.begin(); it != map_edges.end(); ++it)
		cout << it->first.first << " Lien avec les vertex " << it->first.second << " avce les poids " << it->second << endl;
}

int Graph::existsEdge(int src, int dest)
{
	map<pair<int, int>,int>::iterator it = map_edges.find(make_pair(src, dest));

	if(it != map_edges.end())
		return it->second; //donne le cout

	return -1;
}

// constructeur de l'algo genetique
Genetic::Genetic(Graph* graph, int size_population, int generations, int mutation_rate, bool show_population)
{
	if(size_population < 1)
	{
		cout << "Erreur: taille de la population < 1\n";
		exit(1);
	}
	else if(mutation_rate < 0 || mutation_rate > 100) //verifie si le taux de mutation est inf. a 0
	{
		cout << "Erreur: le taux de mutatiion doit etre >= 0 et <= 100\n";
		exit(1);
	}

	this->graph = graph;
	this->size_population = size_population;
	this->real_size_population = 0;
	this->generations = generations;
	this->mutation_rate = mutation_rate;
	this->show_population = show_population;
}

int Genetic::isValidSolution(vector<int>& solution)
{
	int total_cost = 0;
	set<int> set_solution;

	//verifie si il n'existe pas de doublons
	for(int i = 0; i < graph->V; i++)
		set_solution.insert(solution[i]);

	if(set_solution.size() != (unsigned)graph->V)
		return -1;

	//verifie si les connexions sont valides
	for(int i = 0; i < graph->V; i++)
	{
		if(i + 1 <  graph->V)
		{
			int cost = graph->existsEdge(solution[i], solution[i+1]);

			//verifie si il existe une connexion
			if(cost == -1)
				return -1;
			else
				total_cost += cost;
		}
		else
		{
			int cost = graph->existsEdge(solution[i], solution[0]);

			if(cost == -1)
				return -1;
			else
				total_cost += cost;
			break;
		}
	}
	return total_cost;
}

bool Genetic::existsChromosome(const vector<int> & v)
{
	//verifie si il existe un chromosome dans la population
	for(vector<pair<vector<int>, int> >::iterator it=population.begin(); it!=population.end(); ++it)
	{
		const vector<int>& vec = (*it).first; //recupere le vecteur
		if(equal(v.begin(), v.end(), vec.begin())) //compare les vecteurs
			return true;
	}
	return false;
}

void Genetic::initialPopulation()
{
	vector<int> parent;

	parent.push_back(graph->initial_vertex);

	//creation des parents
	for(int i = 0; i < graph->V; i++)
	{
		if(i != graph->initial_vertex)
			parent.push_back(i);
	}

	int total_cost = isValidSolution(parent);

	if(total_cost != -1) //si les parents sont valides
	{
		population.push_back(make_pair(parent, total_cost)); //insertion de la population
		real_size_population++;
	}

	for(int i = 0; i < generations; i++)
	{
		//genere une permutation aleatoire
		random_shuffle(parent.begin() + 1, parent.begin() + (rand() % (graph->V - 1) + 1));

		int total_cost = isValidSolution(parent); //verifie si la solution est valide

		//si elle n'existe pas
		if(total_cost != -1 && !existsChromosome(parent))
		{
			population.push_back(make_pair(parent, total_cost)); //rajout de la population
			real_size_population++;
		}

		if(real_size_population == size_population) //verifie la taille de la population
			break;
	}

	if(real_size_population == 0)
		cout << "\nPopulation initiale vide...";
	else
		sort(population.begin(), population.end(), sort_pred()); //tri de la population
}

void Genetic::showPopulation()
{
	cout << "\nMontre les solutions...\n\n";

	for(vector<pair<vector<int>, int> >::iterator it=population.begin(); it!=population.end(); ++it)
	{
		const vector<int>& vec = (*it).first; //recupere le vecteur

		for(int i = 0; i < graph->V; i++)
			cout << vec[i] << " ";

		cout << graph->initial_vertex;
		cout << " | Cout: " << (*it).second << "\n\n";
	}
	cout << "\nTaille de la population: " << real_size_population << endl;
}

//insertion du vecteur en utilisant la recherche binaire
void Genetic::insertBinarySearch(vector<int>& child, int total_cost)
{
	int imin = 0;
	int imax = real_size_population - 1;

	while(imax >= imin)
	{
		int imid = imin + (imax - imin) / 2;

		if(total_cost == population[imid].second)
		{
			population.insert(population.begin() + imid, make_pair(child, total_cost));
			return;
		}
		else if(total_cost > population[imid].second)
			imin = imid + 1;
		else
			imax = imid - 1;
	}
	population.insert(population.begin() + imin, make_pair(child, total_cost));
}

void Genetic::crossOver(vector<int>& parent1, vector<int>& parent2)
{
	vector<int> child1, child2;

	//mapping des genes
	map<int, int> genes1, genes2;

	for(int i = 0; i < graph->V; i++)
	{
		//a l'initialisation les genes ne sont pas utilises
		genes1[parent1[i]] = 0;
		genes2[parent2[i]] = 0;
	}

	//generation de points aleatoires
	int point1 = rand() % (graph->V - 1) + 1;
	int point2 = rand() % (graph->V - point1) + point1;

	//ajustement des points si ils sont egaux
	if(point1 == point2)
	{
		if(point1 - 1 > 1)
			point1--;
		else if(point2 + 1 < graph->V)
			point2++;
		else
		{
			int point = rand() % 10 + 1; //nb entre 1 et 10
			if(point <= 5)
				point1--;
			else
				point2++;
		}
	}

	//generation des enfants

	//pour un point 1, l'enfant 1 recoit les genes des parents1
	//idem pour le deux
	for(int i = 0; i < point1; i++)
	{
		//ajout des genes
		child1.push_back(parent1[i]);
		child2.push_back(parent2[i]);
		//marque les genes
		genes1[parent1[i]] = 1;
		genes2[parent2[i]] = 1;
	}

	//marque de rappel des genes
	for(int i = point2 + 1; i < graph->V; i++)
	{
		genes1[parent1[i]] = 1;
		genes2[parent2[i]] = 1;
	}

	//inversion des genes
	//enfant 1 recoit genes des parents 2
	//idem enfant 2
	for(int i = point2; i >= point1; i--)
	{
		if(genes1[parent2[i]] == 0) //si gene non utilise
		{
			child1.push_back(parent2[i]);
			genes1[parent2[i]] = 1; //marque le gene
		}
		else
		{
			//si le gene est utilise, on choisi un gene non utilise
			for(map<int, int>::iterator it = genes1.begin(); it != genes1.end(); ++it)
			{
				if(it->second == 0) //verifie si non utilise
				{
					child1.push_back(it->first);
					genes1[it->first] = 1; //marque gene comme utilise
					break;
				}
			}
		}

		if(genes2[parent1[i]] == 0)
		{
			child2.push_back(parent1[i]);
			genes2[parent1[i]] = 1;
		}
		else
		{

			for(map<int, int>::iterator it = genes2.begin(); it != genes2.end(); ++it)
			{
				if(it->second == 0)
				{
					child2.push_back(it->first);
					genes2[it->first] = 1;
					break;
				}
			}
		}
	}

	//enfant 1 recoit gene parents 1 et idem
	for(int i = point2 + 1; i < graph->V; i++)
	{
		child1.push_back(parent1[i]);
		child2.push_back(parent2[i]);
	}

	//mutation
	int mutation = rand() % 100 + 1; //generation nb aleatoire [1,100]

	if(mutation <= mutation_rate) //verifie si le nb aleatoire <= taux de mutation
	{
		//mutation:  on change deux genes
		int index_gene1, index_gene2;
		index_gene1 = rand() % (graph->V - 1) + 1;
		index_gene2 = rand() % (graph->V - 1) + 1;

		//pour enfant 1
		int aux = child1[index_gene1];
		child1[index_gene1] = child1[index_gene2];
		child1[index_gene2] = aux;

		//pour enfant 2
		aux = child2[index_gene1];
		child2[index_gene1] = child2[index_gene2];
		child2[index_gene2] = aux;
	}

	int total_cost_child1 = isValidSolution(child1);
	int total_cost_child2 = isValidSolution(child2);

	//verifie si la solution est valide et n'existe pas dans la population
	if(total_cost_child1 != -1 && !existsChromosome(child1))
	{
		//ajout d'enfants dans la population
		insertBinarySearch(child1, total_cost_child1);
		real_size_population++;
	}

	//validation supplementaire
	if(total_cost_child2 != -1 && !existsChromosome(child2))
	{
		//ajout d'enfants a la population
		insertBinarySearch(child2, total_cost_child2);
		real_size_population++;
	}
}

//algo genetique
void Genetic::run()
{
	initialPopulation();

	if(real_size_population == 0)
		return;

	for(int i = 0; i < generations; i++)
	{
		int  old_size_population = real_size_population;

		//selection de parents si ils existent pour participer au processus de reproduction
		if(real_size_population >= 2)
		{
			if(real_size_population == 2)
			{
				crossOver(population[0].first, population[1].first);
			}
			else
			{
				//taille reelle de la population > 2
				int parent1, parent2;

				do
				{
					//selection aleatoire de 2 parents
					parent1 = rand() % real_size_population;
					parent2 = rand() % real_size_population;
				}
				while(parent1 == parent2);

				crossOver(population[parent1].first, population[parent2].first);
			}

			//recupere la difference pour voir si la population croit
			int diff_population = real_size_population - old_size_population;

			if(diff_population == 2)
			{
				if(real_size_population > size_population)
				{
					//enleve les 2 pires parents de la population
					population.pop_back();
					population.pop_back();

					//en decremente la population de 2
					real_size_population -= 2;
				}
			}
			else if(diff_population == 1)
			{
				if(real_size_population > size_population)
				{
					population.pop_back();
					real_size_population--;
				}
			}
		}
		else
		{
			crossOver(population[0].first, population[0].first);

			if(real_size_population > size_population)
			{
				population.pop_back();
				real_size_population--;
			}
		}
	}

	if(show_population == true)
		showPopulation();

	cout << "\nMeilleure solution: ";
	const vector<int>& vec = population[0].first;

	for(int i = 0; i < graph->V; i++)
		cout << vec[i] << " ";

	cout << graph->initial_vertex;
	cout << " | Cost: " << population[0].second;
}


int Genetic::getCostBestSolution()
{
	if(real_size_population > 0)
		return population[0].second;

	return -1;
}


