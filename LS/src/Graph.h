/**@file  Graph.h
 *
 * @brief  Graph represesentation class consisting of the adjacency lists of its
 * vertices.
 *
 * @details This class reads either a standard edge type file or an adjacency
 * lists file (@see Graph::Graph for details). The Graph class also implements a
 * procedure that generates a degeneracy ordering of the given graph
 * (@see Graph::degeneracyOrdering)and a procedure that outputs the complement
 * of the subgraph induced by a given vertex v and it's neighbors to the right
 * in the degeneracy ordering.
 *
 * @author Jose L. Walteros
 *
 * @version   1.0
 * @date      June 2018
 *
 * Copyright (C) 2018 Jose L. Walteros. All rights reserved.
 *
 *---+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----*/
#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <string>
#include <sstream>
#include <vector>
#include <chrono>

/**
 * vertex structure. The name of the vertex is generaly different to its
 * possition in the vertex list.
 */
struct vertex {
    int v; /**< Vertex name */
    int degree; /**< Vertex's degree */
    int pos; /**< Position of the vertex in the vector of vertices */

    /**
     * Comparator
     */
    friend bool operator<(const vertex& x, const vertex& y) {
        return x.v < y.v;
    }
};

/**
 * subgraph structure.
 */
struct subgraph {
    int n; /**< Number of vertices in the subgraph */
    int m; /**< Number of edges in the subgraph */
    bool created = false; /**< Whether the subgraph is empty*/
    std::vector<vertex>vertices; /**< Set of vertices of the subgraph */
    std::vector<std::vector<int> >adjLists; /**< Adj. lists of the vertices */
    int largestDegreeVertex; /**< Index of the vertex with the largest degree */
};

class Graph {
public:
    const char* name; /**< Graph's name (filename) */
    int n; /**< Number of vertices in G */
    int m; /**< Number of edges in G*/
    int delta; /**< Min degree */
    int Delta; /**< Max degree */
    std::vector<int>EdgeTo; /**< Vector that contains the adjacency list of each
                             * Vertex. The lists are appended one after the other */
    std::vector<int>EdgesBegin;  /**< The position of the first neighbor of each vertex */
    std::vector<int>degree; /**< Degree of the vertices */
    std::vector<int>alias; /**< Name of the vertices */
    std::chrono::duration<double>readTime; /**< ReadingTime */

    /*
     * Members generated after the degeneray ordering is calculated
     */
    int d; /**<  Degeneracy value */
    int cliqueLB; /**< Clique size given by the degeneracy ordering. Obtained when the
                   * rightDegree of a vertex equals to the number of vertices to the right
                   * in the degeneracy ordering */
    int cliqueUB; /**< Upper bound on the clique. Is d+1 normally, but will be set to d if the
                   * d-core is d-regular but does not have a component of size d*/
    std::vector<int>rightDegree; /**<  Number of neighbors to the right in the ordering */
    std::vector<int>ordering; /**< Degeneracy ordering */
    std::vector<int>position; /**< Position of the vertices in the ordering */

    /**
     * Default constructor.
     */
    inline Graph() {}

    /**
     * Graph constructor: Receives a filename with the graph information to be
     * read.The expected format is either the edge list or the adjacency lists of the
     * graph.
     *
     * - Adjacency lists:
     *   The first line of the file must include the number vertices and edges.
     *   (i.e., 62 159). The vertices are expected to be labeled from 1 to n.
     *
     * - Edge list:
     *   The fisrt line of the file must include the number vertices and edges.
     *   (i.e., 62 159). The vertices are expected to be labeled from 0 to n-1.
     *
     * @param[in] filename : Name of the file with the graph's information.
     */
    Graph(
        const char* type,
        const char* filename,
        bool& read);

    /**
     * This procedure generates the degeneracy ordering of the graph (Matula and
     * Beck (1983)). It also populates the vertex sets of the subgraphs induced
     * by the closed right neighborhood of each vertex. This procedure is used
     * by the clique finding algorithms.
     *
     * This code also calculates a lower and and upper bound on the maximum
     * clique size (i.e., the LB results from sequentially removing low degree
     * vertices until the residual graph is a clique; UB is set as d+1)
     *
     * The code identifies whether the d-core is d-regular.
     *
     * @param[out] subgraphs : Vector with the n subgraphs. The vertex set will
     * be created here.
     */
    void degeneracyOrdering(
        std::vector<subgraph>& subgraphs);


    /**
     * This procedure generates the degeneracy ordering of the graph (Matula and
     * Beck (1983)).
     */
    void degeneracyOrdering();

    /**
     * Generates the complement graph induced by the closed right neighborhood
     * of v.
     * @param[in] v : The node for which \bar G[v] will be created
     * @param[out] sG : the corresponding subgraph \bar G[v].
     */
    void generateCompGraphRightNeighbors(
        int v,
        std::vector<subgraph>& sG);

    /**
     * Prints the full description of the graph:
     *
     * <name n m delta Delta>
     * <v(name): adjacency list(v)>
     *
     * For example:
     *
     * test.graph 10 13 1 5
     * 0(4): 1 2 3 9
     * 1(2): 0 2
     * 2(2): 0 1
     * 3(5): 0 4 5 7 8
     * 4(3): 3 5 6
     * 5(4): 3 4 6 8
     * 6(2): 4 5
     * 7(1): 3
     * 8(2): 3 5
     * 9(1): 0
     */
    void print();

    /**
     * Prints a short description of graph:
     *
     * <name n m delta Delta>
     *
     * For example:
     *
     * test.graph 10 13 1 5
     */
    void printShort();
};
#endif // _GRAPH_H_
