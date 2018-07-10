/**@file Buss.h
 *
 * @brief  Implements the Buss kernel. It removes the vertices with degree larger
 * than @param k and the resulting isolated vertices.
 *
 * @details Implements the Buss kernel. It removes the vertices with degree larger
 * than k and the resulting isolated vertices. The resulting kernel is constructed
 * in the @param kernel.
 *
 * If the procedure removes more high degree vertices than @param k, it will
 * return -1 (no VC) without constructing the kernel. If no vertex is removed,
 * @param kernel is set to @param sG (the original input).
 *
 * If the resulting number of vertices in the kernel is les than @param k minus
 * the number of large degree vertices, the procedure returns 1, as there is a VC.
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
#ifndef _BUSS_H_
#define _BUSS_H_

#include <iostream>
#include "Graph.h"

class Buss {
public:
    subgraph* sG; /**< Subgraph to be processed.*/
    int k; /**< Expected size of the VC.*/

    /**
     * Buss constructor: Receives the graph to be processed and the expected VC
     * size.
     * @param[in] sG : Subgraph to be processed.
     * @param[in] k : Expected size of the VC.
     */
    inline Buss(
        subgraph* sG,
        int k) {
        this->sG = sG;
        this->k = k;
    }

    /**
     * This procedure generates the Buss kernel.
     * @param[out] kernel : Buss kernel.
     * @param[out] highDegVertices : Number of high degree vertices in the graph
     *
     * @returns -1 if there is no VC in the subgraph, 1 if there is a VC in the
     * subgraph and 0 if there could be a VC in the subgraph.
     */
    int getKernel(
        subgraph& kernel,
        int& highDegVertices);
};
#endif // ifndef _BUSS_H_
