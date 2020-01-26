#include "the3.h"

bool* candidateDisconVertices;
int** distanceMatrix;

void makeZeroEdgesInfinity(int n, int**& edgeList) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if ((i != j) && (edgeList[i][j] == 0)) {
				edgeList[i][j] = INT_MAX;
			}
		}
	}
}

void allPairsShortestAlgo(int n) {
	for (int k = 0; k < n; k++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {

				// unconnected components
				if ((distanceMatrix[i][k] == INT_MAX) || (distanceMatrix[k][j] == INT_MAX)) continue;

				// shorter path
				if (distanceMatrix[i][k] + distanceMatrix[k][j] < distanceMatrix[i][j]) distanceMatrix[i][j] = distanceMatrix[i][k] + distanceMatrix[k][j];

			}
		}
	}
}

bool connected(int i, int j) {
	if (distanceMatrix[i][j] == INT_MAX) return false;
	return true;
}

int getDisconnectedPairs(int n) {
	std::vector<int> realDisconVertices;
	for (int i = 0; i < n; i++) {

		if (!candidateDisconVertices[i]) continue;
		bool newDisconFlag = true;
		for (unsigned int j = 0; j < realDisconVertices.size(); j++) {
			if (connected(i, realDisconVertices[j])) {
				newDisconFlag = false;
				break;
			}
		}
		if (newDisconFlag) realDisconVertices.push_back(i);

	}
	
	return realDisconVertices.size() == 0 ? 1 : realDisconVertices.size();
}

int Important(int n, int**& edgeList, double*& scores)
{
	makeZeroEdgesInfinity(n, edgeList);

	candidateDisconVertices = new bool[n];
	distanceMatrix = new int* [n];

	for (int i = 0; i < n; i++) {
		distanceMatrix[i] = new int[n];
		candidateDisconVertices[i] = false;
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			distanceMatrix[i][j] = edgeList[i][j];
		}
	}

	allPairsShortestAlgo(n);



	for (int v = 0; v < n; v++) {
		for (int s = 0; s < n; s++) {
			for (int t = 0; t < n; t++) {

				if ((distanceMatrix[s][v] != INT_MAX) && (distanceMatrix[v][t] != INT_MAX)) {

					if ((v == s) || (v == t)) continue;
					else if (s == t) {
						scores[v] += distanceMatrix[s][v] + distanceMatrix[v][t];
					}
					else {
						scores[v] += (double)(distanceMatrix[s][v] + distanceMatrix[v][t]) / distanceMatrix[s][t];
					}
				}
				else {
					if (distanceMatrix[s][v] == INT_MAX) {
						candidateDisconVertices[s] = true;
						candidateDisconVertices[v] = true;
					}
					if (distanceMatrix[v][t] == INT_MAX) {
						candidateDisconVertices[v] = true;
						candidateDisconVertices[t] = true;
					}
				}
			}
		}
	}
	int disconnectedComponents = getDisconnectedPairs(n);
	for (int i = 0; i < n; i++) {
		delete[] distanceMatrix[i];
	}

	delete[] distanceMatrix;
	delete[] candidateDisconVertices;



	return disconnectedComponents;
}