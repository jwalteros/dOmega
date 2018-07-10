/**@file NemhauserTrotter.cpp
 *
 * @brief Finds the linear relaxation of the vertex cover formulation and removes
 * the vertices for which the corresponding variable is either 1 or 0.
 *
 * @details The procedure generates a bipartite graph G' in which the two
 * partitions correspond to copies of the original vertices. Then, it uses Hopcroft-Karp to find a
 * maximum matching and then Tarjan's SCC algorithm to extract a vertex cover with the least
 * number of fractional variables in such a G'.
 *
 * @author Jose L. Walteros
 *
 * @version   1.0
 * @date      January 2018
 *
 * Copyright (C) 2018 Jose L. Walteros. All rights reserved.
 *
 *---+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----*/
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
#include <limits.h>
#include "Graph.h"
#include "NemhauserTrotter.h"


int NemhauserTrotter::getKernel
(
    subgraph& kernel,
    int& numRemoved,
    int& numInVC) {
    HopcroftKarp();
    Tarjan();

    /**
     * The following code generates the SCC graph. That is, a vertex is assigned
     * to
     * each component and an arc from component k1 to component k2 is created
     * if there is an arc whose tail and head are in k1 and k2, respectively.
     */
    int n = sG->n;
    adjListsComp = std::vector<std::vector<int> >(numComponents);
    compOutDegree = std::vector<int>(numComponents, 0);
    connected = std::vector<int>(numComponents, -1);

    for (int t = 0; t < numComponents; t++) {
        for (std::vector<int>::iterator v = components[t].begin(); v != components[t].end(); ++v) {
            if (*v < n) {
                for (std::vector<int>::iterator u = sG->adjLists[*v].begin(); u != sG->adjLists[*v].end(); u++) {
                    if (componentMap[*v] != componentMap[*u + n]) {
                        if (connected[componentMap[*u + n]] != componentMap[*v]) {
                            adjListsComp[componentMap[*u + n]].push_back(componentMap[*v]);
                            compOutDegree[componentMap[*v]]++;
                            connected[componentMap[*u + n]] = componentMap[*v];
                        }
                    }
                }
            } else {
                if ((matchR[*v - n] >= 0) && (componentMap[*v] != componentMap[matchR[*v - n]])) {
                    if (connected[componentMap[matchR[*v - n]]] != componentMap[*v]) {
                        adjListsComp[componentMap[matchR[*v - n]]].push_back(componentMap[*v]);
                        compOutDegree[componentMap[*v]]++;
                        connected[componentMap[matchR[*v - n]]] = componentMap[*v];
                    }
                }
            }
        }
    }

    /**
     * The following code removes secuentially the tail SCC for which
     * (V(S_L) \cap v(S_R) = \emptyset)
     */
    std::vector<bool>removed(sG->n, false);
    std::vector<int>degDecrease(sG->n, 0);
    compRemoved = std::vector<bool>(numComponents, false);
    bool update = true;

    while (update) {
        update = false;

        for (int p = 0; p < numComponents; p++) {
            if ((compRemoved[p] == false) && (compOutDegree[p] == 0) && (toBeRemoved[p] == true)) {
                compRemoved[p] = true;

                if ((components[p].size() == 1) && (removed[components[p][0] % n] == false)) {
                    removed[components[p][0] % n] = true;

                    for (std::vector<int>::iterator current = sG->adjLists[components[p][0] % n].begin(); current != sG->adjLists[components[p][0] % n].end(); current++)
                    {
                        if (removed[*current] == false) {
                            degDecrease[*current]++;
                        }
                    }
                    numRemoved++;
                    continue;
                }

                for (std::vector<int>::iterator v = components[p].begin(); v != components[p].end(); ++v) {
                    if (removed[*v % n] == false) {
                        for (std::vector<int>::iterator current = sG->adjLists[*v % n].begin(); current != sG->adjLists[*v % n].end(); current++) {
                            if (removed[*current] == false) {
                                degDecrease[*current]++;
                            }
                        }
                        removed[*v % n] = true;
                        numRemoved++;

                        if (*v >= n) {
                            numInVC++;
                        }
                    }
                }

                for (std::vector<int>::iterator v = adjListsComp[p].begin(); v != adjListsComp[p].end(); ++v) {
                    compOutDegree[*v]--;
                }
                update = true;
            }
        }
    }

    /**
     * If there are more vertices in the vertex cover than @param k, return -1.
     */
    if (numInVC > k) {
        return -1;
    }

    /**
     * If no vertex is removed, return 0. Also, the kernel is equal to the
     * original subgraph
     * the original subgraph is then hard-copied into the kernel.
     */
    if (numRemoved == 0) {
        kernel = *sG;
        return 0;
    }

    /**
     * If the number of vertices in the kernel is less than @param k -
     * highDegVertices,
     * returns 1 (there is a VC).
     */
    if (sG->n - numRemoved <= k - numInVC) {
        return 1;
    }

    /**
     * The kernel is generated based on the vector of removed vertices.
     */
    VertexCover VC;
    VC.subgraphUpdate(sG->n, numRemoved, sG->vertices, sG->adjLists, removed, kernel);

    /**
     * If the number of edges in the kernel is less than k*(k-highDegVertices),
     * returns -1 (there is no VC).
     */
    if (kernel.m > k * (k - numInVC)) {
        return -1;
    }

    /**
     * Return 0, as there could be a VC.
     */
    return 0;
}

void NemhauserTrotter::HopcroftKarp() {
    int matching = 0;
    int dMax;
    int n = sG->n;

    std::vector<int>dist(n, 0);
    std::queue<int>queue;

    while (BFS(dist, queue, dMax) == true) {
        for (std::vector<vertex>::iterator u = sG->vertices.begin(); u < sG->vertices.begin() + n; u++) {
            if (matchL[u->pos] == -1) {
                if (DFS(u->pos, dist, queue, dMax)) {
                    matching++;
                }
            }
        }
    }
}

bool NemhauserTrotter::BFS(
    std::vector<int>& dist,
    std::queue<int>& queue,
    int& dMax) {
    int n = sG->n;

    for (std::vector<vertex>::iterator u = sG->vertices.begin(); u < sG->vertices.begin() + n; u++) {
        int pos = u->pos;

        if (matchL[pos] == -1) {
            dist[pos] = 0;
            queue.push(pos);
        } else {
            dist[pos] = INT_MAX;
        }
    }
    dMax = INT_MAX;

    while (!queue.empty()) {
        int u = queue.front();
        queue.pop();

        if (dist[u] < dMax) {
            for (std::vector<int>::iterator element = sG->adjLists[u].begin(); element != sG->adjLists[u].end(); element++) {
                int v = *element;

                if (matchR[v] == -1) {
                    if (dMax == INT_MAX) {
                        dMax = dist[u] + 1;
                    }
                } else {
                    if (dist[matchR[v]] == INT_MAX) {
                        dist[matchR[v]] = dist[u] + 1;
                        queue.push(matchR[v]);
                    }
                }
            }
        }
    }
    return dMax != INT_MAX;
}

bool NemhauserTrotter::DFS(
    int u,
    std::vector<int>& dist,
    std::queue<int>& queue,
    int& dMax) {
    if (u != -1) {
        for (std::vector<int>::iterator element = sG->adjLists[u].begin(); element != sG->adjLists[u].end(); element++) {
            int v     = *element;
            int k     = matchR[v];
            int distK = dMax;

            if (k >= 0) {
                distK = dist[matchR[v]];
            }

            if (distK == dist[u] + 1) {
                if (DFS(k, dist, queue, dMax)) {
                    matchR[v] = u;
                    matchL[u] = v;
                    return true;
                }
            }
        }
        dist[u] = INT_MAX;
        return false;
    }
    return true;
}

void NemhauserTrotter::Tarjan() {
    int n = sG->n;

    indices = std::vector<int>(2 * n, -1);
    lowLink = std::vector<int>(2 * n, 0);
    onStack = std::vector<bool>(2 * n, false);
    componentMap = std::vector<int>(2 * n);
    vertexMap = std::vector<int>(n, -1);
    numComponents = 0;
    index = 0;

    for (int i = 0; i < n; i++) {
        if (indices[i] == -1) {
            strongConnect(i);
        }
    }
}

void NemhauserTrotter::strongConnect(
    int v) {
    int n = sG->n;

    indices[v] = index;
    lowLink[v] = index;
    index++;
    S.push(v);
    onStack[v] = true;

    if (v < n) {
        for (std::vector<int>::iterator current = sG->adjLists[v].begin(); current != sG->adjLists[v].end(); current++) {
            int u = *current;

            if (indices[u + n] == -1) {
                strongConnect(u + n);
                lowLink[v] = std::min(lowLink[v], lowLink[u + n]);
            } else {
                if (onStack[u + n] == true) {
                    lowLink[v] = std::min(lowLink[v], lowLink[u + n]);
                }
            }
        }
    } else {
        int u = matchR[v - n];

        if (u >= 0) {
            if (indices[u] == -1) {
                strongConnect(u);
                lowLink[v] = std::min(lowLink[v], lowLink[u]);
            } else {
                if (onStack[u] == true) {
                    lowLink[v] = std::min(lowLink[v], lowLink[u]);
                }
            }
        }
    }


    if (lowLink[v] == indices[v]) {
        components.push_back(std::vector<int>());
        toBeRemoved.push_back(true);

        while (S.top() != v) {
            int u = S.top();
            componentMap[u] = numComponents;
            components[numComponents].push_back(u);

            if (u < n) {
                if (vertexMap[u] == numComponents) {
                    toBeRemoved[numComponents] = false;
                }
                vertexMap[u] = numComponents;
            } else {
                if (vertexMap[u - n] == numComponents) {
                    toBeRemoved[numComponents] = false;
                }

                vertexMap[u - n] = numComponents;
            }
            onStack[u] = false;
            S.pop();
        }
        int u = S.top();
        componentMap[u] = numComponents;
        components[numComponents].push_back(u);

        if (u < n) {
            if (vertexMap[u] == numComponents) {
                toBeRemoved[numComponents] = false;
            }
            vertexMap[u] = numComponents;
        } else {
            if (vertexMap[u - n] == numComponents) {
                toBeRemoved[numComponents] = false;
            }
            vertexMap[u - n] = numComponents;
        }
        numComponents++;
        onStack[u] = false;
        S.pop();
    }
}
