/**@file Buss.h
 *
 * @brief Implements the Buss kernel. It removes the vertices with degree larger
 * than @param k and the resulting isolated vertices.
 *
 * @details Implements the Buss kernel. It removes the vertices with degree
 * larger than @param k and the resulting isolated vertices. The resulting
 * kernel is constructed in the @param kernel.
 *
 * If the procedure removes more high degree vertices than @param k, it will
 * return -1 (no VC) without constructing the kernel. If no vertex is removed,
 * @param kernel is set to @param sG (the original input).
 *
 * If the resulting number of vertices in the kernel is les than @param k minus
 * the number of large degree vertices, the procedure returns 1, as there is a
 * VC.
 *
 * If the resulting number of edges is greater than k*(k-highDegVertices), the
 * procedure returns -1 as there is no VC.
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
#include <vector>
#include "Buss.h"
#include "Graph.h"
#include "VertexCover.h"


int Buss::getKernel(
    subgraph& kernel,
    int& highDegVertices) {
    int numRemoved = 0;

    std::vector<bool>removed(sG->n, false);

    /**
     * Marks the vertices that will be removed based on their degree. If deg(i)>
     * @param k, mark the vertex as removed.
     */
    bool change = true;

    while (change &&  highDegVertices <= k) {
        change = false;

        for (std::vector<vertex>::iterator i = sG->vertices.begin(); i < sG->vertices.end() && highDegVertices <= k; i++) {
            if (i->degree > k - highDegVertices) {
                removed[i->pos] = true;
                highDegVertices++;
                numRemoved++;
                change = true;
            }
        }
    }

    /**
     * If there are more vertices with a degree larger than @param k, return -1.
     */
    if (highDegVertices > k) {
        return -1;
    }

    /**
     * If no vertex is removed, return 0. Also, the kernel is equal to the
     * original subgraph the original subgraph is then hard-copied into the kernel.
     */
    if (highDegVertices == 0) {
        kernel = *sG;
        return 0;
    }

    /**
     * Any isolated vertex that results after marking the high degree vertices
     * is also marked for deletion.
     */
    for (std::vector<vertex>::iterator i = sG->vertices.begin(); i < sG->vertices.end(); i++) {
        if (removed[i->pos] == false) {
            bool isolated = true;

            for (std::vector<int>::iterator current = sG->adjLists[i->pos].begin(); current != sG->adjLists[i->pos].end(); current++) {
                /**
                 * If the vertex is not marked to be removed and has at least
                 * one neighbor not marked to be removed, is not isolated.
                 */
                if (removed[*current] == false) {
                    isolated = false;
                    break;
                }
            }

            if (isolated) {
                removed[i->pos] = true;
                numRemoved++;
            }
        }
    }

    /**
     * The kernel is generated based on the vector of removed vertices.
     */
    VertexCover VC;
    VC.subgraphUpdate(sG->n, numRemoved, sG->vertices, sG->adjLists, removed, kernel);

    /**
     * If the number of vertices in the kernel is less than @param k - highDegVertices,
     * returns 1 (there is a VC).
     */
    if (kernel.n <= k - highDegVertices) {
        return 1;
    }

    /**
     * If the number of edges in the kernel is less than k*(k-highDegVertices),
     * returns -1 (there is no VC).
     */
    if (kernel.m > k * (k - highDegVertices)) {
        return -1;
    }

    /**
     * Return 0, as there could be a VC.
     */
    return 0;
}
