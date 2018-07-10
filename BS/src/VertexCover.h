/**@file VertexCover.h
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
 * vertices in N(k) in the vertex cover. That is, N[k] are removed and k is
 * decreased by |N(v)|.
 *
 * @author Jose L. Walteros
 *
 * @version   1.0
 * @date      January 2018
 *
 * Copyright (C) 2018 Jose L. Walteros. All rights reserved.
 *
 *---+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----*/
#ifndef _VERTEXCOVER_H_
#define _VERTEXCOVER_H_

#include <vector>
#include "Graph.h"

class VertexCover {
public:
    /**
     * Default constructor.
     */
    inline VertexCover() {}

    /**
     * DegreePreprocessing: The procedure performs the following tasks until
     * there is no further update:
     *
     * 1. Any isolated vertex is removed.
     * 2. If there is a vertex v with degree 1, both v and its neighbor are
     * removed and k is decreased by 1.
     * 3. If there is a graph v with degree 2:
     *    a. If its neighbors u and w are adjacent, u, v, and w are removed and
     *       k is decreased by 2.
     *    b. If its neighbors u and w are not adjacent, u and w are removed and
     *       its neighbors are attatched to v (vertex folding). k is decreased by 1.
     *
     * @param[in] n : Number of vertices in the graph.
     * @param[in] k : Expected size of the vertex cover.
     * @param[in] vertices : vertices of the graph.
     * @param[in] adjLists : Adjacency lists of the graphs.
     * @param[out] newK : new value of k after the update.
     * @param[out] kernel : the graph after the procedure.
     */
    int degreePreprocessing(
        int n,
        int k,
        std::vector<vertex>& vertices,
        std::vector<std::vector<int> >& adjLists,
        int& newK,
        subgraph& kernel);

    /**
     * Given a set of vertices that are marked as removed, produces the
     * corresponding
     * updated subgraph.
     *
     * @param[in] n : Number of vertices in the original graph.
     * @param[in] numRemoved : Number of vertices that are marked as removed.
     * @param[in] vertices : vertices of the graph.
     * @param[in] adjLists : Adjacency lists of the graphs.
     * @param[in] removed : if the vertices are removed or not.
     * @param[out] kernel : the graph after the procedure.
     */
    void subgraphUpdate(
        int n,
        int& numRemoved,
        std::vector<vertex>& vertices,
        std::vector<std::vector<int> >& adjLists,
        std::vector<bool>& removed,
        subgraph& kernel);

    /**
     * kVertexCover:
     *
     * Finds if a graph has a vertex cover of size k.
     *
     * a) Calls the degree reduction procedure (@see
     * VertexCover::degreePreprocessing)
     *
     * b) Generates two branches using the vertex with the largest degree k. The
     * first branch
     * is created by adding k the vertex cover reducing k and the size of G by
     * one.
     * The othe branch is created by adding all the neighbors of k reducing k by
     *\Delta(G') and
     * G' by \Delta(G')+1
     *
     * @param[in] n : Number of vertices in the graph.
     * @param[in] k : Expected size of the vertex cover.
     * @param[in] vertices : vertices of the graph.
     * @param[in] adjLists : Adjacency lists of the graphs.
     */
    bool kVertexCover(
        int n,
        int k,
        std::vector<vertex>& vertices,
        std::vector<std::vector<int> >& adjLists);
};
#endif // _VERTEXCOVER_H_
