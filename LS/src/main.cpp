/**
 * @file main.h
 *
 * @brief Finds the maximum clique of a graph.
 *
 * @author Jose L. Walteros
 *
 * @version   1.0
 * @date      June 2018
 *
 * Copyright (C) 2018 Jose L. Walteros. All rights reserved.
 *
 *---+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----*/
#include <string>
#include <sstream>
#include <cstring>
#include <chrono>
#include <thread>
#include <iostream>
#include "Clique.h"
#include "Graph.h"


int main(int argc, const char* argv[]) {
    std::stringstream output;
    int numThreads = std::thread::hardware_concurrency();

    if (argc < 4) {
        std::cout << "Incorrect inputs. See the README file\n";
    } else {
        const char* type = argv[1];
        const char* filename = argv[2];
        const char* algorithm = argv[3];
        bool read = true;
        Graph graph(type, filename, read);

        if (read) {
            graph.printShort();

            if (strcmp(algorithm, "-d") == 0) {
                graph.degeneracyOrdering();

                output << filename << " " << graph.n << " " << graph.m << " " <<
                    graph.delta << " " << graph.Delta << " " <<
                    graph.readTime.count() << " " << graph.d << " " <<
                    graph.cliqueLB << "\n";

                std::cout << output.str();
            }

            if (strcmp(algorithm, "-m") == 0) {
                if (argc == 5) {
                    char* pconv;
                    int conv;
                    conv = strtol(argv[4], &pconv, 10);

                    if ((*pconv == '\0') && (conv <= numThreads)) {
                        numThreads = conv;
                    }
                }

                Clique clique(graph, numThreads);
                clique.findMaxClique();

                // clique.printMaxClique();

                output << filename << " " << graph.n << " " << graph.m << " " <<
                    graph.delta << " " << graph.Delta << " " <<
                    graph.readTime.count() << " " << graph.d << " " <<
                    graph.cliqueLB << " " << clique.degeneracyTime.count() << " " <<
                    clique.cliqueUB << " " << clique.runningTime.count() << " " <<
                    numThreads << "\n";

                std::cout << output.str();
            }
        }
    }
}
