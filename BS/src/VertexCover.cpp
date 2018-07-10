/**@file VertexCover.cpp
 *
 * @brief Finds if a graph has a vertex cover of size k.
 *
 * @details The procedure performs the following tasks until there is no further
 * update:
 *
 * 1. Any isolated vertex is removed.
 * 2. If there is a vertex v with degree 1, both v and its neighbor are removed
 *    and k is decreased by 1.
 * 3. If there is a graph v with degree 2:
 *    a. If its neighbors u and w are adjacent, u, v, and w are removed and k is
 *    decreased by 2.
 *    b. If its neighbors u and w are not adjacent, u and w are removed and its
 *    neighbors are attatched to v (vertex folding). k is decreased by 1.
 *
 * Then, the vertex v with the largest degree is selected. The procedure
 * generates one branch in which v is assumed to be in the vertex cover. That is,
 * v is removed and k is decreased by 1. A second branch is created assuming the
 * vertices in N(k)in the vertex cover. That is, N[k] are removed and k is decreased
 * by |N(v)|.
 *
 * @author Jose L. Walteros
 *
 * @version   1.0
 * @date      January 2018
 *
 * Copyright (C) 2018 Jose L. Walteros. All rights reserved.
 *
 *---+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----*/
#include <vector>
#include <iostream>
#include <algorithm>
#include "VertexCover.h"
#include "Graph.h"

int VertexCover::degreePreprocessing(
    int                             n,
    int                             k,
    std::vector<vertex>& vertices,
    std::vector<std::vector<int> >& adjLists,
    int& newK,
    subgraph& kernel) {
    int numRemoved = 0;
    bool change = true;

    newK = k;

    /**
     * degreeDecrease is used to udate the degree of the vertices based on the
     * neighbors that have been removed or the vertex that have been attached
     * after a vertex folding.
     */
    std::vector<int>degDecrease(n, 0);
    std::vector<bool>removed(n, false);

    while (change && n - numRemoved > newK && newK >= 0) {
        change = false;

        /**
         * Mark the vertices that will be removed based on their degree.
         */
        for (std::vector<vertex>::iterator i = vertices.begin(); i < vertices.begin() + n && newK >= 0; i++) {
            /**
             * If i has degree > new K.
             */
            if ((removed[i->pos] == false) && (i->degree - degDecrease[i->pos] > newK)) {
                removed[i->pos] = true;
                numRemoved++;

                newK--;
                change = true;

                /**
                 * Decrease the degree fo the vertex's neigbors.
                 */
                for (std::vector<int>::iterator current = adjLists[i->pos].begin(); current != adjLists[i->pos].end(); current++) {
                    if (removed[*current] == false) {
                        degDecrease[*current]++;
                    }
                }
                continue;
            }


            /**
             * If i has degree 1 or 0.
             */
            if ((removed[i->pos] == false) && (i->degree - degDecrease[i->pos] <= 1)) {
                removed[i->pos] = true;
                numRemoved++;

                /**
                 * If i has degree 1, find its neighbor and mark it as removed.
                 */
                if (i->degree - degDecrease[i->pos] == 1) {
                    newK--;
                    change = true;
                    std::vector<int>::iterator neighbor = adjLists[i->pos].begin();

                    while (removed[*neighbor] == true) {
                        neighbor++;
                    }

                    removed[*neighbor] = true;
                    numRemoved++;

                    /**
                     * Decrease the degree fo the vertex's neigbors.
                     */
                    for (std::vector<int>::iterator current = adjLists[*neighbor].begin(); current != adjLists[*neighbor].end(); current++) {
                        if (removed[*current] == false) {
                            degDecrease[*current]++;
                        }
                    }
                }
                continue;
            }

            /**
             * If i has degree 2.
             */
            if ((removed[i->pos] == false) && (i->degree - degDecrease[i->pos] == 2)) {
                /**
                 * Finds the neigbors.
                 */
                std::vector<int>::iterator neighbor1 = adjLists[i->pos].begin();

                while (removed[*neighbor1] == true) {
                    neighbor1++;
                }

                std::vector<int>::iterator neighbor2 = neighbor1 + 1;

                while (removed[*neighbor2] == true) {
                    neighbor2++;
                }

                /**
                 * Check is the neighbors are adjacent.
                 */
                bool adjacent = false;

                if (vertices[*neighbor1].degree - degDecrease[*neighbor1] <= vertices[*neighbor2].degree - degDecrease[*neighbor2]) {
                    adjacent = std::binary_search(adjLists[*neighbor1].begin(), adjLists[*neighbor1].end(), *neighbor2);
                } else {
                    adjacent = std::binary_search(adjLists[*neighbor2].begin(), adjLists[*neighbor2].end(), *neighbor1);
                }

                removed[*neighbor1] = true;
                removed[*neighbor2] = true;
                change = true;

                /**
                 * If the neighbors are adjacent, remove the three vertices.
                 */
                if (adjacent) {
                    removed[i->pos] = true;
                    newK = newK - 2;
                    numRemoved = numRemoved + 3;

                    for (std::vector<int>::iterator current = adjLists[*neighbor1].begin(); current != adjLists[*neighbor1].end(); current++) {
                        if (removed[*current] == false) {
                            degDecrease[*current]++;
                        }
                    }

                    for (std::vector<int>::iterator current = adjLists[*neighbor2].begin(); current != adjLists[*neighbor2].end(); current++) {
                        if (removed[*current] == false) {
                            degDecrease[*current]++;
                        }
                    }
                } else {
                    /**
                     * If the neighbors are not adjacent, performs a vertex folding.
                     */
                    newK = newK - 1;
                    numRemoved = numRemoved + 2;
                    degDecrease[i->pos] = degDecrease[i->pos] + 2;
                    int a = *neighbor1;
                    int b = *neighbor2;
                    adjLists[i->pos] = std::vector<int>();
                    adjLists[i->pos].reserve(vertices[a].degree + vertices[b].degree - degDecrease[vertices[a].pos] - degDecrease[vertices[b].pos]);
                    std::vector<int>::iterator current1 = adjLists[a].begin();
                    std::vector<int>::iterator current2 = adjLists[b].begin();

                    while (current1 != adjLists[a].end() && current2 != adjLists[b].end()) {
                        if ((removed[*current1] == true) || (vertices[*current1].v == i->v)) {
                            current1++;
                            continue;
                        }

                        if ((removed[*current2] == true) || (vertices[*current2].v == i->v)) {
                            current2++;
                            continue;
                        }

                        if (*current1 < *current2) {
                            std::vector<int>::iterator pos = std::lower_bound(adjLists[*current1].begin(), adjLists[*current1].end(), i->pos);
                            adjLists[*current1].insert(pos, i->pos);
                            adjLists[i->pos].push_back(*current1);
                            degDecrease[i->pos]--;
                            current1++;
                            continue;
                        }

                        if (*current2 < *current1) {
                            std::vector<int>::iterator pos = std::lower_bound(adjLists[*current2].begin(), adjLists[*current2].end(), i->pos);
                            adjLists[*current2].insert(pos, i->pos);
                            adjLists[i->pos].push_back(*current2);
                            degDecrease[i->pos]--;
                            current2++;
                            continue;
                        }

                        // Same vertex
                        std::vector<int>::iterator pos = std::lower_bound(adjLists[*current1].begin(), adjLists[*current1].end(), i->pos);
                        adjLists[*current1].insert(pos, i->pos);
                        adjLists[i->pos].push_back(*current1);
                        degDecrease[i->pos]--;
                        degDecrease[*current1]++;
                        current1++;
                        current2++;
                    }

                    while (current1 != adjLists[a].end()) {
                        if ((removed[*current1] == false) && (vertices[*current1].v != i->v)) {
                            std::vector<int>::iterator pos = std::lower_bound(adjLists[*current1].begin(), adjLists[*current1].end(), i->pos);
                            adjLists[*current1].insert(pos, i->pos);
                            adjLists[i->pos].push_back(*current1);
                            degDecrease[i->pos]--;
                        }
                        current1++;
                    }

                    while (current2 != adjLists[b].end()) {
                        if ((removed[*current2] == false) && (vertices[*current2].v != i->v)) {
                            std::vector<int>::iterator pos = std::lower_bound(adjLists[*current2].begin(), adjLists[*current2].end(), i->pos);
                            adjLists[*current2].insert(pos, i->pos);
                            adjLists[i->pos].push_back(*current2);
                            degDecrease[i->pos]--;
                        }
                        current2++;
                    }
                }
            }
        }
    }

    if (n - numRemoved <= newK) {
        return 1;
    }

    if (newK <= 0) {
        return -1;
    }

    /**
     * Generates the kernel.
     */
    std::vector<int>mask(n);

    for (int i = 0; i < n; i++) {
        vertex v = vertices[i];

        if (removed[v.pos] == 0) {
            kernel.vertices[kernel.n].v = v.v;
            kernel.vertices[kernel.n].degree = 0;
            kernel.vertices[kernel.n].pos = kernel.n;
            kernel.adjLists[kernel.n] = std::vector<int>(v.degree - degDecrease[i]);
            mask[v.pos] = kernel.n;
            kernel.n++;
        }
    }
    int largestDegree = 0;
    kernel.m = 0;

    for (int i = 0; i < n; i++) {
        vertex v = vertices[i];

        if (removed[v.pos] == false) {
            int pos = 0;

            for (std::vector<int>::iterator current = adjLists[v.pos].begin(); current != adjLists[v.pos].end(); current++) {
                if (removed[*current] == false) {
                    kernel.adjLists[mask[v.pos]][pos++] = mask[*current];
                    kernel.vertices[mask[v.pos]].degree++;
                }
            }
            kernel.m += kernel.vertices[mask[v.pos]].degree;

            if (largestDegree < kernel.vertices[mask[v.pos]].degree) {
                largestDegree = kernel.vertices[mask[v.pos]].degree;
                kernel.largestDegreeVertex = mask[v.pos];
            }
        }
    }

    /**
     * If the number of vertices in the kernel is less than @param k -
     * highDegVertices,
     * returns 1 (there is a VC).
     */
    kernel.m = kernel.m / 2;

    /**
     * If the number of edges in the kernel is less than k*(k-highDegVertices),
     * returns -1 (there is no VC).
     */
    if (kernel.m > k * newK) {
        return -1;
    }

    /**
     * Return 0, as there could be a VC.
     */
    return 0;
}

void VertexCover::subgraphUpdate(
    int n,
    int& numRemoved,
    std::vector<vertex>& vertices,
    std::vector<std::vector<int> >& adjLists,
    std::vector<bool>& removed,
    subgraph& sG) {
    /**
     *  Position of the vertices in the kernel vector of vertices.
     */
    std::vector<int>mask(n);

    /**
     * The following code populates the vector of vertices in the kernel.
     */
    sG.n = n - numRemoved;
    sG.m = 0;
    sG.vertices = std::vector<vertex>(n - numRemoved);
    sG.adjLists = std::vector<std::vector<int> >(n - numRemoved);
    int count = 0;

    for (int i = 0; i < n; i++) {
        vertex v = vertices[i];

        if (removed[v.pos] == false) {
            sG.vertices[count].v = v.v;
            sG.vertices[count].degree = 0;
            sG.vertices[count].pos = count;
            sG.adjLists[count] = std::vector<int>();
            sG.adjLists[count].reserve(v.degree);
            mask[v.pos] = count;
            count++;
        }
    }

    /**
     * The following code populates the adjacency lists of the kernel.
     */
    int largestDegree = 0;

    for (int i = 0; i < n; i++) {
        vertex v = vertices[i];

        if (removed[v.pos] == false) {
            for (std::vector<int>::iterator current = adjLists[v.pos].begin(); current != adjLists[v.pos].end(); current++) {
                if (removed[*current] == false) {
                    sG.adjLists[mask[v.pos]].push_back(mask[*current]);
                    sG.vertices[mask[v.pos]].degree++;
                }
            }
            sG.m += sG.vertices[mask[v.pos]].degree;

            if (largestDegree < sG.vertices[mask[v.pos]].degree) {
                largestDegree = sG.vertices[mask[v.pos]].degree;
                sG.largestDegreeVertex = mask[v.pos];
            }
        }
    }
    sG.m = sG.m / 2;
    sG.created = true;
}

bool VertexCover::kVertexCover(
    int n,
    int k,
    std::vector<vertex>& vertices,
    std::vector<std::vector<int> >& adjLists) {
    /**
     * Creates tha kernel based on the procedure that preprocess that vertices
     * based on their degree (@see VertexCover::degreePreprocessing).
     */
    subgraph sG;

    sG.vertices = std::vector<vertex>(n);
    sG.adjLists = std::vector<std::vector<int> >(n);
    sG.n = 0;
    int newK = 0;
    int success = degreePreprocessing(n, k, vertices, adjLists, newK, sG);

    if (success == -1) {
        return false;
    }

    if (success == 1) {
        return true;
    }

    /**
     * Generates the upper branch: Assumes a is in the vertex cover.
     */
    int a = sG.largestDegreeVertex;
    std::vector<vertex>verticesUp(sG.n - 1);
    std::vector<std::vector<int> >adjListsUp(sG.n - 1);
    int count = 0;

    for (int i = 0; i < sG.n; i++) {
        if (i != a) {
            vertex v = vertices[i];
            verticesUp[count].v      = v.v;
            verticesUp[count].degree = 0;
            verticesUp[count].pos    = count;
            adjListsUp[count]        = std::vector<int>();
            adjListsUp[count].reserve(v.degree);

            for (std::vector<int>::iterator current = sG.adjLists[v.pos].begin(); current != sG.adjLists[v.pos].end(); current++) {
                if (*current < a) {
                    adjListsUp[count].push_back(*current);
                    verticesUp[count].degree++;
                }

                if (*current > a) {
                    adjListsUp[count].push_back(*current - 1);
                    verticesUp[count].degree++;
                }
            }
            count++;
        }
    }

    if (kVertexCover(sG.n - 1, newK - 1, verticesUp, adjListsUp) == true) {
        return true;
    }

    /**
     * Generates the upper branch: Assumes N(a) is in the vertex cover.
     */
    std::vector<bool>removed(sG.n, false);
    std::vector<vertex>verticesDown(sG.n - 1 - sG.vertices[a].degree);
    std::vector<std::vector<int> >adjListsDown(sG.n - 1 - sG.vertices[a].degree);
    removed[a] = true;

    for (std::vector<int>::iterator current = sG.adjLists[a].begin(); current != sG.adjLists[a].end(); current++) {
        removed[*current] = true;
    }

    count = 0;
    std::vector<int>mask(sG.n);

    for (int i = 0; i < sG.n; i++) {
        vertex v = sG.vertices[i];

        if (removed[v.pos] == false) {
            verticesDown[count].v = v.v;
            verticesDown[count].degree = 0;
            verticesDown[count].pos = count;
            adjListsDown[count] = std::vector<int>();
            adjListsDown[count].reserve(v.degree);
            mask[v.pos] = count;
            count++;
        }
    }

    for (int i = 0; i < sG.n; i++) {
        vertex v = sG.vertices[i];

        if (removed[v.pos] == false) {
            for (std::vector<int>::iterator current = sG.adjLists[v.pos].begin(); current != sG.adjLists[v.pos].end(); current++) {
                if (removed[*current] == false) {
                    adjListsDown[mask[v.pos]].push_back(mask[*current]);
                    verticesDown[mask[v.pos]].degree++;
                }
            }
        }
    }

    return kVertexCover(sG.n - 1 - sG.vertices[sG.largestDegreeVertex].degree,
                        newK - sG.vertices[sG.largestDegreeVertex].degree,
                        verticesDown,
                        adjListsDown);
}
