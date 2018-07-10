/**
 *
 * @file Clique.h
 * @brief Finds The maximum clique of a graph.
 *
 * @details This code implements an efficient algorithm for the maximum clique
 * problem that runs in time polynomial in the graph's size, but exponential
 * in the difference $d-\omega$ (where $d$ denotes the graph's degeneracy).
 * When this difference is a constant, the running time is ${O}(dm)={O}(m^{1.5})$.
 * Since 70% of common test instances have $d-\omega \le 3$, our implementation
 * performs rather well and is competitive with previous approaches.
 * Key subroutines in our approach include existing kernelization and
 * fixed-parameter tractable algorithms for vertex cover.
 *
 * Please cite the following paper if used:
 * Jose L. Walteros and Austin L. Buchanan. Why is maximum clique easy in practice?
 *
 * @author Jose L. Walteros
 *
 * @version   1.0
 * @date      January 2018
 *
 * Copyright (C) 2018 Jose L. Walteros. All rights reserved.
 *
 *---+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----*/
#ifndef _CLIQUE_H_
#define _CLIQUE_H_

#include <math.h>
#include <chrono>
#include <atomic>
#include <vector>
#include "Graph.h"
#include "VertexCover.h"


class Clique {
public:
    int numThreads; /**< Number of threads to use in the run */
    std::atomic<int>cliqueLB;  /**< Lower bound of max clique */
    std::atomic<int>cliqueUB; /**< Upper bound of max clique */
    std::atomic<bool>cliqueFlag; /**< Whether a thread has found a clique */
    std::atomic<int>subgraphClique; /**< subgraph that has a maximum clique */
    std::vector<int>sortedList; /**< List of vertices sorted by their right degree */
    Graph& graph; /**< Graph */
    std::vector<subgraph>subgraphs; /**< Subgraphs induced by closed right neighborhood of the vertices */
    std::chrono::duration<double>degeneracyTime; /**< Degeneracy running time */
    std::chrono::duration<double>runningTime; /**< Total running time */

    /**
     * Clique object constuctor.
     *
     * @param[in] graph: The graph.
     * @param[in] numThreads : Number of threads to be used in the run.
     */
    Clique(
        Graph& graph,
        const int numThreads);

    /**
     * findMaxClique: The procedure finds the size of the max clique of the graph
     */
    int findMaxClique();
};
#endif // _CLIQUE_H_
