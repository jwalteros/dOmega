dOmega Library
=====================================

This code implements an efficient algorithm for the maximum clique
problem that runs in time polynomial in the graph's size, but exponential
in the clique-core gap g:=(d+1)-omega (where d denotes the graph's degeneracy and omega
the size of the largest clique).

When this gap $g$ can be treated as a constant, as is often the case for real-life graphs, 
the proposed algorithm runs in time $O(dm)=O(m^{1.5})$.
Key subroutines in our approach include existing kernelization and
fixed-parameter tractable algorithms for vertex cover.

Please cite the following paper if used:
Jose L. Walteros and Austin L. Buchanan. Why is maximum clique often easy in practice?

The code implements the following modules:

* **Degeneracy:** Given a simple undirected graph G, the code outputs the graph's degeneracy an a valid degeneracy ordering.
* **Maximum clique:** Given a simple undirected graph G, the code finds the size of the largest clique.

The code includes two implementations of the algorithm (LS and BS(. The main difference between both implementations is the way in which the algorithm performs the search (see Jose L. Walteros and Austin L. Buchanan. Why is maximum clique often easy in practice? for further details). 

Usage
---------

### Compiling the code
The library provides a simple makefile to compile the library. The library has been tested on Linux (Ubuntu and CentOS), macOS X (Mavericks-Sierra), and Windows (Cygwin and VS)  

* **For the LS version:** 
		$ cd path/dOmega/LS
		$ make
        
* **For the BS version:** 
              $ cd path/dOmega/BS
              $ make


### Input file format
The code can receives as input adjacency lists and edge list files:

* **Adjacency lists**  
The first line of the file must include the number vertices and edges. The vertices are expected to be labeled from 1 to n.

		%% Adjacency lists of a 6-vertex, 6-edge graph
		6 6
		2 3 4
		1 3 5
		1 2 6
		1
		2
		3


* **Edge list**  
The first line of the file must include the number vertices and edges. The vertices are expected to be labeled from 0 to n-1.

		%% Edge list of a 6-vertex, 6-edge graph
		6 6
		0 1
		0 2
		0 3
		1 2
		1 4
		2 5


### Running the code

* **Degeneracy***  
To find the degeneracy ordering of a graph use:  
./dOmega [file type] [filename] -d

		# Finds the degeneracy ordering of the graph described by edge list file testEdge.txt
		./dOmega -e ../dat/testEdge.txt -d

		# Finds the degeneracy ordering of the graph described by adjacency list file testAdj.txt
		./dOmega -a ../dat/testAdj.txt -d


* **Maximum Clique***  
To find the size of the maximum clique of a graph use:  
./dOmega [file type] [filename] -m [optional: num. processors to use]

		# Finds the size of the maximum clique of the graph described by
		# edge list file testEdge.txt 3 processors
		./dOmega -e ../dat/testEdge.txt -k 3

Terms and conditions
--------------------

Please feel free to use these codes. We only ask that you cite:  

	Jose L. Walteros and Austin L. Buchanan. Why is maximum clique often easy in practice?

_This library was implemented for academic purposes. You may use the library with no limitation for academic, commercial, and/or leisure applications without the need an explicit permission, other than the one granted by the MIT type license included in the library. We kindly ask you to aknowledge the library by citing the above paper in any publication that uses this resources._

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Copyright (C) 2017 Jose L. Walteros. All rights reserved.
