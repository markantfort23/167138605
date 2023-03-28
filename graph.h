/*graph.h*/

//
// Graph class implemented as ...
// 
//    Using a map of vertices with the key as the name and the value being incremented numbers to be able to index.
//    The edges are stored in a vector of maps and maps of maps are used to do quick loops and get fast time complexity
//    over an adjacency matrix and other methods
// 
// For this implementation of graph, the vertices are 
// type long long, and the weights are doubles.
//
// Mark Fortes
//
// Northwestern University
// CS 211: Winter 2023
//

#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <map>
#include <vector>
#include <cassert>
#include <queue>
#include <algorithm>
#include <stack>
#include <utility>
#include <set>

using namespace std;


class graph
{
private:
  map<long long, int> Vertices;
  int edgeCount;
  vector<map<int, double>> Edges;
  map<int, long long> forPrint;

public:
  //
  // default constructor: no parameters
  //
  graph();

  //
  // NumVertices
  //
  // Returns the # of vertices currently in the graph.
  //
  int NumVertices() const;

  //
  // NumEdges
  //
  // Returns the # of edges currently in the graph.
  //
  int NumEdges() const;

  //
  // addVertex
  //
  // Adds the vertex v to the graph and returns true. However,
  // if v is already in the graph, it is not added again and 
  // false is returned.
  //
  bool addVertex(long long v);

  //
  // addEdge
  //
  // Adds the edge (from, to, weight) to the graph, and returns
  // true. If the vertices do not exist then no edge is added
  // and false is returned.
  //
  // NOTE: if the edge already exists, the existing edge weight
  // is overwritten with the new edge weight.
  //
  bool addEdge(long long from, long long to, double weight);

  //
  // getWeight
  //
  // Returns the weight associated with a given edge.  If 
  // the edge exists, the weight is returned via the reference
  // parameter and true is returned. If the edge does not 
  // exist, the weight parameter is unchanged and false is
  // returned.
  //
  bool getWeight(long long from, long long to, double& weight) const;

  //
  // neighbors
  //
  // Returns a set containing the neighbors of v, i.e. all
  // vertices that can be reached from v along one edge. If
  // v does not exist, an empty set is returned.
  //
  set<long long> neighbors(long long v) const;

  //
  // getVertices
  //
  // Returns a vector containing all the vertices currently in
  // the graph. There is no guaranteed order to the vertices 
  // in the returned vector; the order should be considered 
  // random.
  //
  vector<long long> getVertices() const;

  //
  // print
  // 
  // Prints the internal state of the graph to the given 
  // output stream for debugging purposes. The output will
  // contain the # of vertices, the # of edges, the vertices,
  // and the edges. Vertices are output as strings and the 
  // weights as doubles.
  //
  // Example:
  //    graph G;
  //    ...
  //    G.print(cout);  // print to console
  //
  void print(ostream& output) const;

};