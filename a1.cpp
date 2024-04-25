#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

class Graph {
	int vertices;
	vector<vector<int>> adj;
public:
	Graph(int V) : vertices(V), adj(V) {}

	void addEdge(int v, int w) {
		adj[v].push_back(w);
	}

	void parallelDFS(int startVertex) {
		vector<bool> visited(vertices, false);
		parallelDFSInternal(startVertex, visited);
	}

	void parallelDFSInternal(int v, vector<bool>& visited) {
		visited[v] = true;
		cout << v << " ";

		#pragma omp parallel for
		for (int i = 0; i < adj[v].size(); ++i) {
			int n = adj[v][i];
			if (!visited[n])
				parallelDFSInternal(n, visited);
		}
	}

	void parallelBFS(int startVertex) {
		vector<bool> visited(vertices, false);
		queue<int> q;

		visited[startVertex] = true;
		q.push(startVertex);

		while (!q.empty()) {
			int v = q.front();
			q.pop();
			cout << v << " ";

			#pragma omp parallel for
			for (int i = 0; i < adj[v].size(); ++i) {
				int n = adj[v][i];
				if (!visited[n]) {
					visited[n] = true;
					q.push(n);
				}
			}
		}
	}
};

int main() {
	Graph g(6);
	g.addEdge(0, 1);
	g.addEdge(0, 2);
	g.addEdge(1, 3);
	g.addEdge(1, 5);
	g.addEdge(2, 3);
	g.addEdge(3, 4);
	g.addEdge(4, 5);
	
	/*
	
		0 --------- 1 --------- 5
		|			|			|
		|			|			|
		|			|			|
		2 --------- 3 --------- 4

	
	*/

	cout << "\n\n  Depth-First Search (DFS): ";
	g.parallelDFS(0);
	cout << endl;

	cout << "Breadth-First Search (BFS): ";
	g.parallelBFS(0);
	cout << "\n\n";
}