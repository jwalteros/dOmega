Bellerophon Library
=====================================

This code implements an efficient algorithm for the maximum clique
problem that runs in time polynomial in the graph's size, but exponential
in the difference d-omega (where d denotes the graph's degeneracy and omega
the size of the largest clique).

When this difference is a constant, the running time is ${O}(dm)={O}(m^{1.5})$.
Since 70% of common test instances have d-omega <= 3, our implementation
performs rather well and is typically faster than other libaries approaches.
Key subroutines in our approach include existing kernelization and
fixed-parameter tractable algorithms for vertex cover.

Please cite the following paper if used:
Jose L. Walteros and Austin L. Buchanan. Why is maximum clique easy in practice?

The code implements the following modules:

* **Degeneracy:** Given a simple undirected graph G, the code outputs the graph's degeneracy an a valid degeneracy ordering.
* **Clique of size k:** Given a simple undirected graph G and an integer k, the code finds whether G has a clique of size k.
* **Maximum clique:** Given a simple undirected graph G, the code finds the size of the largest clique.


Usage
---------

### Compiling the code
The library provides a simple makefile to compile the library. The library has ben tested on Linux (Ubuntu and CentOS), macOS X (Mavericks-Sierra), and Windows (Cygwin and VS)  

		$ cd path/VC clique/
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
./VC_clique [file type] [filename] -d

		# Finds the degeneracy ordering of the graph described by edge list file testEdge.txt
		./VC_clique -e ../dat/testEdge.txt -d

		# Finds the degeneracy ordering of the graph described by adjacency list file testAdj.txt
		./VC_clique -a ../dat/testAdj.txt -d

* **Clique of size k***  
To find whether a graph has a clique of size k use:  
./VC_clique [file type] [filename] -k [desired k] [optional: num. processors to use]

		# Finds whether the graph described by edge list file testEdge.txt
		# has a clique of size 4 using 3 processors
		./VC_clique -e ../dat/testEdge.txt -k 4 3

* **Clique of size k***  
To find the size of the maximum clique of a graph use:  
./VC_clique [file type] [filename] -m [optional: num. processors to use]

		# Finds the size of the maximum clique of the graph described by
		# edge list file testEdge.txt 3 processors
		./VC_clique -e ../dat/testEdge.txt -k 3

Terms and conditions
--------------------

Please feel free to use these codes. We only ask that you cite:  

	Jose L. Walteros and Austin L. Buchanan. Why is maximum clique easy in practice?

_This library was implemented for academic purposes. You may use the library with no limitation for academic, commercial, and/or leisure applications without the need an explicit permission, other than the one granted by the MIT type license included in the library. We kindly ask you to aknowledge the library by citing the above paper in any publication that uses this resources._

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Copyright (C) 2017 Jose L. Walteros. All rights reserved.
