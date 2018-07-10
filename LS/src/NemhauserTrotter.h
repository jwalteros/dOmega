/**@file NemhauserTrotter.h
 *
 * @brief Finds the linear relaxation of the vertex cover formulation and removes
 * the vertices for which the corresponding variable is either 1 or
 * 0.
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
#ifndef _NEMHAUSERTROTTER_H_
#define _NEMHAUSERTROTTER_H_

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include "Graph.h"
#include "VertexCover.h"

class NemhauserTrotter {
public:
    subgraph* sG; /**< Subgraph to be processed.*/
    int k; /**< Expected size of the VC.*/

    /**
     *  Data for the bipartite matching algorithm (Hopcroft-Karp)
     */
    std::vector<int>matchL; /**< Match of the vertices in the left set. */
    std::vector<int>matchR; /**< Match of the vertices in the right set. */

    /**
     * Data for the strongly connected components algorithm (Tarjan).
     */
    int index; /**< Discover time counter. */
    std::vector<bool>onStack; /**< Is v still in the stack? */
    std::vector<int>indices; /**< Discovery time of v. */
    std::vector<int>lowLink; /**< the smallest index of any vertex known to be
                              * reachable from v, including v itself. */
    std::stack<int>S; /**< Stack of vertices. */

    /**
     * Strongly connected component data
     */
    std::vector<int>componentMap; /**< SSC that contains v. */
    std::vector<std::vector<int> >components; /**< Vertices that are in S. */
    std::vector<int>vertexMap; /**< Map used to check if both the left and right
                                * copies of v are covered by the same SCC. */
    std::vector<bool>toBeRemoved; /**< If the S can be removed(V(S_L) \cap v(S_R)
                                   * = \emptyset). */
    int numComponents; /**< Number of SCC. */

    /*
     * Data required for producing the kernel
     */
    std::vector<std::vector<int> >adjListsComp;  /**< Anti-adjacency lists. Nodes
                                                  * connected to v. */
    std::vector<int>compOutDegree; /**< outdegree of component p. */
    std::vector<int>connected; /**< This map is used to check if and arc between
                                * two components already exists. */
    std::vector<bool>compRemoved; /**< If component p has been removed during the
                                   * kernel generation. */

    /**
     * NemhauserTrotter constructor: Receives the graph to be processed and the
     * expected VC size.
     * @param[in] sG : Subgraph to be processed.
     * @param[in] k : Expected size of the VC.
     */
    inline NemhauserTrotter(
        subgraph* sG,
        int       k) {
        this->sG = sG;
        this->k = k;
        matchL = std::vector<int>(sG->n, -1);
        matchR = std::vector<int>(sG->n, -1);
    }

    /**
     * This procedure generates the Nemhauser*Trotter kernel.
     * @param[out] kernel : NT kernel.
     * @param[out] numRemoved : Number of vertices removed from the graph
     * @param[out] numInVC : Number of vertices added to the VC.
     *
     * @returns -1 if there is no VC in the subgraph, 1 if there is a VC in the
     * subgraph,and 0 if there could be a VC in the subgraph.
     */
    int getKernel(
        subgraph& kernel,
        int& numRemoved,
        int& numInVC
        );

    /**
     * Hopcroft-Karp procedure: Finds a maximum matching of bipartite graphs. It
     * is an augmenting path algorithm that shares similarities with max-flow
     * algorithms, but has a better asymptotic complexity
     *
     * The procedure uses functions BFS and DFS (@see NemhauserTrotter::BFS and
     * NemhauserTrotter::DFS)
     */
    void HopcroftKarp();
    bool BFS(
        std::vector<int>&,
        std::queue<int>&,
        int&);

    bool DFS(
        int,
        std::vector<int>&,
        std::queue<int>&,
        int&);

    /**
     * Tarjan procedure: Finds the strongly connected components of the residual
     * graph given by the matching. This version of Tarjan's is tailored for the graph
     * resulting from the matching.
     *
     * The procedure uses the recursive function strongConnect (@see
     * NemhauserTrotter::strongConnect).
     */
    void Tarjan();
    void strongConnect(
        int v);
};
#endif // _NEMHAUSERTROTTER_H_
