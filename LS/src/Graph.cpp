/**
 *
 * @file  Graph.cpp
 *
 * @brief  Graph represesentation class consisting of the adjacency lists of its
 * vertices.
 *
 * @details This class reads either a standard edge type file or an adjacency
 * lists file (@see Graph::Graph for details). The Graph class also implements a
 * procedure that generates a degeneracy ordering of the given graph
 * (@see Graph::degeneracyOrdering) and a procedure that outputs the complement
 * of the subgraph induced by a givenv ertex v and its neighbors to the right in
 * the degeneracy ordering.
 *
 * @author Jose L. Walteros
 *
 * @version   1.0
 * @date      January 2018
 *
 * Copyright (C) 2018 Jose L. Walteros. All rights reserved.
 *
 *---+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----*/
#include "Graph.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <fstream>
#include <algorithm>


Graph::Graph(
    const char *type,
    const char *filename,
    bool &read)
{
    name = filename;
    std::ifstream file(filename);

    if (!file)
    {
        std::cerr << "ERROR: could not open file '" << filename <<
        " for reading" << std::endl;
        read = false;
    }
    file >> n >> m;

    if (n * m == 0)
    {
        std::cerr << "ERROR: when reading the graph from file '" << filename;
        read = false;
    }
    else
    {
        /*
         * The input files have several inconsistencies including duplicated
         * edges.
         *
         * The adjacency lists are first filtered by a set that removes duplicates.
         * Additionally, this filtering process sorts the adjacency lists, which
         * is quite useful. The filtering process increases the time required to
         * generate the graph, but removes duplicates. Cannot be removed unless the
         * input files are fixed first.
         */
        int i;
        int j;
        int counter;
        int real_m = 0;
        EdgesBegin = std::vector<int>(n, 0);
        degree = std::vector<int>(n, 0);
        alias = std::vector<int>(n, 0);
        std::vector<std::set<int> > adjLists(n, std::set<int>());
        real_m = 0;
        std::chrono::high_resolution_clock::time_point begin_time = std::chrono::high_resolution_clock::now();

        /*
         * Procedure for reading edge list input files. The adjacency lists are
         * first created as sets to ensure there are no duplicated edges and loops.
         */
        if (strcmp(type, "-e") == 0)
        {
            std::map<int, int> nameMap;
            counter = 0;
            int u;
            int v;
            std::map<int, int>::iterator it;

            for (int e = 0; e < m; e++)
            {
                file >> i >> j;

                it = nameMap.find(i);

                if (it != nameMap.end())
                {
                    u = nameMap[i];
                }
                else
                {
                    nameMap[i] = counter;
                    alias[counter] = i;
                    u = counter;
                    counter++;
                }
                it = nameMap.find(j);

                if (it != nameMap.end())
                {
                    v = nameMap[j];
                }
                else
                {
                    nameMap[j] = counter;
                    alias[counter] = j;
                    v = counter;
                    counter++;
                }

                if ((u != v) && adjLists[u].insert(v).second)
                {
                    degree[u]++;
                    real_m++;
                }

                if ((u != v) && adjLists[v].insert(u).second)
                {
                    degree[v]++;
                }
            }
        }
        else
        {
            /*
             * Procedure for reading edge list input files. The adjacency lists
             * are first created as sets to ensure there are no duplicated edges
             * and loops.
             */
            if (strcmp(type, "-a") == 0)
            {
                /**
                 * These files are assumed to list the vertices starting with 1
                 */

                std::string str;
                i = 0;

                std::getline(file, str);

                while (std::getline(file, str))
                {
                    std::istringstream ss(str);
                    alias[i] = i + 1;

                    while (ss >> j)
                    {
                        if ((adjLists[i].insert(j - 1)).second)
                        {
                            degree[i]++;
                        }
                    }
                    real_m += degree[i];
                    i++;
                }
                real_m = real_m / 2;
            }
        }
        m = real_m;

        EdgeTo = std::vector<int>(2 * m);
        delta = n;
        Delta = 0;
        counter = 0;

        /*
         * The following procedure extracts the adjacency lists from the filter
         * created by the sets. We don't keep the sets because std::vectors
         * tend to perform better. The extra I/O overhead of the filter is a
         * result of the inconsistencies in most of the graph files used for
         * testing.
         */
        for (int i = 0; i < n; i++)
        {
            if (degree[i] < delta)
            {
                delta = degree[i];
            }

            if (degree[i] > Delta)
            {
                Delta = degree[i];
            }
            EdgesBegin[i] = counter;

            for (std::set<int>::iterator it = adjLists[i].begin(); it != adjLists[i].end(); ++it)
            {
                EdgeTo[counter++] = *it;
            }
        }

        std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
        readTime = std::chrono::duration_cast<std::chrono::duration<double> >(end_time - begin_time);
        rightDegree = std::vector<int>(n, 0);
        position = std::vector<int>(n, 0);
        ordering = std::vector<int>(n, 0);
    }
}

void Graph::degeneracyOrdering(
    std::vector<subgraph> &subgraphs)
{
    std::vector<int> buckets(Delta + 1, 0);
    cliqueLB = 0;
    d = 0;

    /**
     * This code generates the initial state of the buckets based on the degree
     * of the vertices.
     */
    for (int i = 0; i < n; i++)
    {
        rightDegree[i] = degree[i];
        buckets[rightDegree[i]]++;
    }

    int temp;
    int count = 0;

    for (int k = 0; k < Delta + 1; k++)
    {
        temp = buckets[k];
        buckets[k] = count;
        count += temp;
    }

    for (int i = 0; i < n; i++)
    {
        position[i] = buckets[rightDegree[i]];
        ordering[position[i]] = i;
        buckets[rightDegree[i]]++;
    }

    for (int k = Delta; k > 0; k--)
    {
        buckets[k] = buckets[k - 1];
    }
    buckets[0] = 0;

    /**
     * If the subgraph induced by the d-core is d-regular, dRegular will store
     * the position of the first vertex of the d-core in the degerenracy ordering.
     */
    int dRegular = -1;
    int minV;

    for (int i = 0; i < n; i++)
    {
        /**
         * The position of the vertex with the current min degree is obtained
         * and the corresponding vertex v is added to the degeneracy ordering.
         * The degeneracy is updated in case the degree of the vertex is bigger
         * than the previous d.
         *
         * Also, the vertex set of the subgraph of v is generated here for all
         * the vertices. This, helps to generate the adjacency lists efficiently
         * later.
         */
        minV = ordering[i];
        subgraphs[minV].n = rightDegree[minV] + 1;
        subgraphs[minV].m = 0;
        subgraphs[minV].vertices = std::vector<vertex>(rightDegree[minV] + 1);
        int nV = 0;
        subgraphs[minV].vertices[nV].v = minV;
        subgraphs[minV].vertices[nV].degree = 0;
        subgraphs[minV].vertices[nV].pos = nV;
        nV++;
        buckets[rightDegree[minV]]++;

        if (rightDegree[minV] > d)
        {
            d = rightDegree[minV];
            if (rightDegree[ordering[n - 1]] == d)
            {
                dRegular = i;
            }
        }

        /**
         * If the current degree of the vertex that is being removed is equal to
         * the number of vertices left in the graph, those vertices induce a clique
         * whose size can be used as a lower bound.
         */
        if ((cliqueLB == 0) && (rightDegree[ordering[i]] == n - i - 1))
        {
            cliqueLB = rightDegree[ordering[i]] + 1;
        }

        /**
         * This part of the code iterates over the neighbors to the right and
         * of the vertex being removed and reduces their degree by one
         */
        for (int j = EdgesBegin[minV]; j < degree[minV] + EdgesBegin[minV]; j++)
        {
            int neighbor = EdgeTo[j];

            /**
             * If the neighbor has not been removed, updates its degree and
             * bucket
             */
            if (position[neighbor] > position[minV])
            {
                subgraphs[minV].vertices[nV].v = EdgeTo[j];
                subgraphs[minV].vertices[nV].degree = 0;
                subgraphs[minV].vertices[nV].pos = nV;
                nV++;

                if (rightDegree[neighbor] == rightDegree[minV])
                {
                    if (neighbor != ordering[buckets[rightDegree[minV]]])
                    {
                        int pu = buckets[rightDegree[minV]];
                        int u = ordering[pu];
                        ordering[pu] = neighbor;
                        ordering[position[neighbor]] = u;
                        position[u] = position[neighbor];
                        position[neighbor] = pu;
                    }
                    buckets[rightDegree[minV] - 1] = position[minV] + 1;
                    buckets[rightDegree[neighbor]]++;
                    rightDegree[neighbor]--;
                }
                else
                {
                    int pu = buckets[rightDegree[neighbor]];
                    int u = ordering[pu];

                    if (neighbor != u)
                    {
                        ordering[pu] = neighbor;
                        ordering[position[neighbor]] = u;
                        position[u] = position[neighbor];
                        position[neighbor] = pu;
                    }
                    buckets[rightDegree[neighbor]]++;
                    rightDegree[neighbor]--;
                }
            }
        }
    }

    cliqueUB = d + 1;

    if ((dRegular > 0) && (cliqueLB < cliqueUB))
    {
        bool clique = false;
        std::vector<int> S(n);
        int head = 0;
        int tail = 0;
        std::vector<bool> discovered(n, false);

        for (std::vector<int>::iterator i = ordering.begin() + dRegular; i < ordering.end(); i++)
        {
            int count = 0;
            S[tail++] = *i;
            discovered[*i] = true;
            count++;

            while (head < tail)
            {
                int v = S[head++];

                for (int j = EdgesBegin[v]; j < degree[v] + EdgesBegin[v]; j++)
                {
                    if ((position[EdgeTo[j]] > dRegular) && !discovered[EdgeTo[j]])
                    {
                        discovered[EdgeTo[j]] = true;
                        S[tail++] = EdgeTo[j];
                        count++;
                    }
                }
            }

            if (count == d + 1)
            {
                clique = true;
                break;
            }
        }

        if (!clique)
        {
            cliqueUB = d;
        }
    }
}

void Graph::degeneracyOrdering()
{
    std::chrono::high_resolution_clock::time_point begin_time = std::chrono::high_resolution_clock::now();
    std::vector<int> buckets(Delta + 1, 0);

    d = 0;

    /**
     * This code generates the initial state of the buckets based on the degree
     * of the vertices.
     */
    for (int i = 0; i < n; i++)
    {
        rightDegree[i] = degree[i];
        buckets[rightDegree[i]]++;
    }

    int temp;
    int count = 0;

    for (int k = 0; k < Delta + 1; k++)
    {
        temp = buckets[k];
        buckets[k] = count;
        count += temp;
    }

    for (int i = 0; i < n; i++)
    {
        position[i] = buckets[rightDegree[i]];
        ordering[position[i]] = i;
        buckets[rightDegree[i]]++;
    }

    for (int k = Delta; k > 0; k--)
    {
        buckets[k] = buckets[k - 1];
    }
    buckets[0] = 0;

    int minV;

    for (int i = 0; i < n; i++)
    {
        /**
         * The position of the vertex with the current min degree is obtained
         * and the corresponding vertex v is added to the degeneracy ordering.
         * The degeneracy is updated in case the degree of the vertex is larger
         * than the previous d.
         *
         * Also, the vertex set of the subgraph of v is generated here for all
         * the vertices. This, helps to generate the adjacency lists efficiently
         * later.
         */
        minV = ordering[i];

        buckets[rightDegree[minV]]++;

        if (rightDegree[minV] > d)
        {
            d = rightDegree[minV];
        }

        /**
         * This part of the code iterate over the neighbors to the right and
         * decrease of the vertex being removed and reduces their degree by one
         */
        for (int j = EdgesBegin[minV]; j < degree[minV] + EdgesBegin[minV]; j++)
        {
            int neighbor = EdgeTo[j];

            /**
             * If the neighbor has not been removed, update its degree and
             * bucket
             */
            if (position[neighbor] > position[minV])
            {
                if (rightDegree[neighbor] == rightDegree[minV])
                {
                    if (neighbor != ordering[buckets[rightDegree[minV]]])
                    {
                        int pu = buckets[rightDegree[minV]];
                        int u = ordering[pu];
                        ordering[pu] = neighbor;
                        ordering[position[neighbor]] = u;
                        position[u] = position[neighbor];
                        position[neighbor] = pu;
                    }
                    buckets[rightDegree[minV] - 1] = position[minV] + 1;
                    buckets[rightDegree[neighbor]]++;
                    rightDegree[neighbor]--;
                }
                else
                {
                    int pu = buckets[rightDegree[neighbor]];
                    int u = ordering[pu];

                    if (neighbor != u)
                    {
                        ordering[pu] = neighbor;
                        ordering[position[neighbor]] = u;
                        position[u] = position[neighbor];
                        position[neighbor] = pu;
                    }
                    buckets[rightDegree[neighbor]]++;
                    rightDegree[neighbor]--;
                }
            }
        }
    }

    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> degeneracyTime = std::chrono::duration_cast<std::chrono::duration<double> >(end_time - begin_time);

    std::clog << "Degeneracy: " << d << "\n";
    std::clog << "Order: [";

    for (int j = 0; j < n; j++)
    {
        std::clog << alias[ordering[j]] << " ";
    }
    std::clog << "]\n";
    std::clog << "Right Degree: [\n";

    for (int j = 0; j < n; j++)
    {
        std::clog << alias[j] << ": " << rightDegree[j] << "\n";
    }
    std::clog << "]\n";
    std::clog << "Position: [\n";

    for (int j = 0; j < n; j++)
    {
        std::clog << alias[j] << ": " << position[j] << "\n";
    }
    std::clog << "]\n";
    std::clog << "Degeneracy running time: " << degeneracyTime.count() << "\n";
    std::clog << "-------------------------------------------------------------\n";
}

void Graph::generateCompGraphRightNeighbors(
    int v,
    std::vector<subgraph> &subgraphs)
{
    /**
     * The following code populates the std::vector of right neighboors of v in
     * the degeneracy ordering. The std::vector includes v as well.
     */
    subgraphs[v].created = true;
    subgraphs[v].adjLists = std::vector<std::vector<int> >(subgraphs[v].n);

    /**
     * The following code finds, for each pair of vertices in the subgraph, if
     * they are adjacent in G. If not, it adds the corresponding edge to the
     * subgraph.
     *
     * See G. Manoussakis. New algorithms for cliques and related structures in k-degenerate graphs. arXiv preprint arXiv:1501.01819v4, 2016
     */
    int largestDegree = 0;

    std::vector<std::vector<bool>> incMat(subgraphs[v].n, std::vector<bool>(subgraphs[v].n, false));

    for (std::vector<vertex>::iterator i = subgraphs[v].vertices.begin() + 1; i < subgraphs[v].vertices.end(); i++)
    {
        std::vector<vertex>::iterator current1 = subgraphs[v].vertices.begin() + 1;
        std::vector<vertex>::iterator current2 = subgraphs[i->v].vertices.begin() + 1;
        while (current1 != subgraphs[v].vertices.end() && current2 != subgraphs[i->v].vertices.end())
        {
            if (current2->v < current1->v)
            {
                current2++;
                continue;
            }
            if (current1->v == current2->v)
            {
                current1++;
                current2++;
                continue;
            }
            if (current1->v == i->v)
            {
                current1++;
                continue;
            }
            if (current1->v < current2->v)
            {
                if (position[i->v] < position[current1->v])
                {
                    incMat[i->pos][current1->pos] = true;
                    incMat[current1->pos][i->pos] = true;
                    i->degree++;
                    current1->degree++;
                    subgraphs[v].m++;
                }
                current1++;
                continue;
            }
        }
        while (current1 != subgraphs[v].vertices.end())
        {
            if (position[i->v] < position[current1->v])
            {
                incMat[i->pos][current1->pos] = true;
                incMat[current1->pos][i->pos] = true;
                i->degree++;
                current1->degree++;
                subgraphs[v].m++;
            }
            current1++;
            continue;
        }
        if (i->degree > largestDegree)
        {
            subgraphs[v].largestDegreeVertex = i->pos;
        }
    }
    for (std::vector<vertex>::iterator i = subgraphs[v].vertices.begin() + 1; i < subgraphs[v].vertices.end(); i++)
    {
        for (int j = 1; j < subgraphs[v].n; j++)
        {
            if (incMat[i->pos][j])
            {
                subgraphs[v].adjLists[i->pos].push_back(j);
            }
        }
    }
}

void Graph::clearElements()
{
    std::vector<int>().swap(EdgeTo);
    std::vector<int>().swap(EdgesBegin);
    std::vector<int>().swap(degree);
    std::vector<int>().swap(alias);
    std::vector<int>().swap(rightDegree);
    std::vector<int>().swap(ordering);
}

void Graph::print()
{
    printShort();
    std::clog << "\n";

    for (int i = 0; i < n; i++)
    {
        std::clog << alias[i] << "(" << degree[i] << "): ";

        for (int j = EdgesBegin[i]; j < degree[i] + EdgesBegin[i]; j++)
        {
            std::clog << alias[EdgeTo[j]] << " ";
        }
        std::clog << std::endl;
    }
    std::clog << std::endl;
}

void Graph::printShort()
{
    std::clog << "-------------------------------------------------------------\n";
    std::clog << "Filename: " << name << "\nn: " << n << "\nm: " << m <<
    "\ndelta: " << delta << "\nDelta: " << Delta << "\nReading time: " <<
    readTime.count() << "\n";
    std::clog << "-------------------------------------------------------------\n";
}
