// Project 5b: Solving graph coloring using local search
// Michael Goodnow, Zackary Mackay

#include <algorithm>
#include <climits>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, undirectedS, VertexProperties,
                       EdgeProperties>
    Graph;

// convenience types
typedef Graph::vertex_iterator v_itr;
typedef Graph::edge_iterator e_itr;
typedef Graph::adjacency_iterator a_itr;
typedef Graph::edge_descriptor e_dsc;
typedef Graph::vertex_descriptor v_dsc;

// only using color
struct VertexProperties {
	int color;
	bool visited;
};

// no edge properties
struct EdgeProperties {};

// Initialize g using data from fin.
void initializeGraph(Graph &g, ifstream &fin) {
	int n, e;
	int j, k;

	fin >> n >> e;
	v_dsc v;

	// Add nodes.
	for (int i = 0; i < n; i++) {
		v = add_vertex(g);
	}

	for (int i = 0; i < e; i++) {
		fin >> j >> k;
		add_edge(j, k, g); // Assumes vertex list is type vecS
	}
}

// Find the best color for the given vertex
int findBestColor(Graph &g, v_dsc vertex, int numColors) {
	pair<a_itr, a_itr> r = adjacent_vertices(vertex, g);

	vector<int> colorFreq(numColors, 0);

	// Loop over adjacent nodes in the graph, tallying their colors
	for (a_itr i = r.first; i != r.second; ++i) {
		v_dsc v_adj = *i;
		if (g[v_adj].visited) {
			colorFreq[g[v_adj].color]++;
		}
	}

	g[vertex].visited = true;

	// return index of minimum element in colorFreq
	return min_element(colorFreq.begin(), colorFreq.end()) - colorFreq.begin();
}

// Greedily color the graph, minimizing color conflicts.
void greedyColoring(Graph &g, int numColors) {
	pair<v_itr, v_itr> range = vertices(g);

	// Loop over all nodes in the graph
	for (v_itr i = range.first; i != range.second; i++) {
		v_dsc vertex = *i;
		g[vertex].color = findBestColor(g, vertex, numColors);
	}
}

// Determine the number of coloring conflicts in the graph
int numConflicts(Graph &g) {
	int conflicts = 0;
	pair<e_itr, e_itr> r = edges(g);
	// Loop over all edges in the graph
	for (e_itr i = r.first; i != r.second; ++i) {

		// conflicts are when neighboring nodes have the same color
		e_dsc ed = *i;
		v_dsc tgt = target(ed, g);
		v_dsc src = source(ed, g);
		if (g[tgt].color == g[src].color) conflicts++;
	}
	return conflicts;
}

// print the number of conflicts and the coloring configuration
void printSolution(Graph &g) {
	cout << "------------------------------------------------" << endl;
	cout << "Num conflicts: " << numConflicts(g) << endl;
	pair<v_itr, v_itr> r = vertices(g);

	for (v_itr i = r.first; i < r.second; ++i) {
		cout << *i << " " << g[*i].color << endl;
	}
}

int main(int argc, char *argv[]) {
	ifstream fin;
	if (argc != 2) {
		cerr << "Usage:\n\t./p5b input_file" << endl;
		exit(1);
	}
	fin.open(argv[1]);
	if (!fin) {
		cerr << "Cannot open " << argv[1] << endl;
		exit(1);
	}

	Graph g;
	int numColors;
	fin >> numColors;
	initializeGraph(g, fin);

	cout << "Num colors: " << numColors << endl;
	cout << "Num nodes: " << num_vertices(g) << endl;
	cout << "Num edges: " << num_edges(g) << endl;
	cout << endl;

	greedyColoring(g, numColors);

	printSolution(g);

	return 0;
}
