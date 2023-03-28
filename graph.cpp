
/*graph.cpp*/

//
// Graph class implemented by using a map of vertices with the key as the string name and the value being incremented numbers to be able to index.
//    The edges are stored in a vector of maps and maps of maps are used to do quick loops and get fast time complexity
//    over an adjacency matrix and other methods
// 
// Mark Fortes
// Northwestern University
// CS 211: Winter 2023
// 

#include <iostream>
#include "graph.h"

using namespace std;

// default constructor: no parameters
graph::graph() {
    this->edgeCount = 0;
}

// NumVertices
// Returns the # of vertices currently in the graph.
int graph::NumVertices() const {
    return this->Vertices.size();
}

// NumEdges
// Returns the # of edges currently in the graph.
int graph::NumEdges() const {
    return this->edgeCount;
}

// addVertex
// Adds the vertex v to the graph and returns true. However,
// if v is already in the graph, it is not added again and false is returned.
bool graph::addVertex(long long v) {
    if (this->Vertices.find(v) != this-> Vertices.end()) return false;
    else {
        this->Vertices[v] = this->NumVertices();
        this->forPrint[this->NumVertices()] = v; //to reverse for print function
        map<int, double> init; //initial edge
        Edges.push_back(init);
        return true;
    }
}

// addEdge
// Adds the edge (from, to, weight) to the graph, and returns
//true. If the vertices do not exist then no edge is added and false is returned.
// NOTE: if the edge already exists, the existing edge weight
// is overwritten with the new edge weight.
bool graph::addEdge(long long from, long long to, double weight) {
    if (this-> Vertices.find(from) == this-> Vertices.end() || this->Vertices.find(to) == this->Vertices.end()) return false;
    int pos = this->Vertices[from];
    if (this->Edges[pos].find(to) == this->Edges[pos].end()) // if edge does not already exist
        edgeCount = edgeCount + 1;
    this->Edges[pos][to] = weight; // also updates weight if edge already exists
    return true;
}

// getWeight
// Returns the weight associated with a given edge.  If 
// the edge exists, the weight is returned via the reference
// parameter and true is returned. If the edge does not 
// exist, the weight parameter is unchanged and false is returned.
 bool graph::getWeight(long long from, long long to, double& weight) const {
    if (this-> Vertices.find(from) == this-> Vertices.end() || this-> Vertices.find(to) == this-> Vertices.end()) return false;

    int pos = this->Vertices.find(from) -> second;
    auto ref = this->Edges[pos];
    if (this->Edges[pos].find(to) != this->Edges[pos].end() ) { //edge exists
        weight = ref[to]; //weight returned via reference parameter
        return true;
    }
    return false;
} 

// neighbors
// Returns a set containing the neighbors of v, i.e. all
// vertices that can be reached from v along one edge. If
// v does not exist, an empty set is returned.
set<long long> graph::neighbors(long long v) const {
    set<long long> neigh;  
    if (this-> Vertices.find(v) == this-> Vertices.end()) {
        return neigh; //empty set
    } 
    int pos = this-> Vertices.find(v)->second;
    auto e = this->Edges[pos]; 

    for (auto vertices : e) {
        neigh.insert(vertices.first);
    }
    return neigh;
}

// getVertices
// Returns a vector containing all the vertices currently in
// the graph
vector<long long> graph::getVertices() const {
    vector<long long> vertices;   

    for(auto pair : this->Vertices) {
        vertices.push_back(pair.first);
    }
    return vertices;
}

// print
// Prints the internal state of the graph to the given 
// output stream for debugging purposes
void graph::print(ostream& output) const {
    output << "# of vertices: " << this->NumVertices() << endl;
    output << "# of edges: " << this->NumEdges() << endl;
    output << "Vertices: " << endl;

    for (auto pair : this->Vertices) {
        output << " " << pair.first << endl;
    }
    output << endl;

    output << "Edges: " << endl;
    for (int i = 0; i < this->NumVertices(); i++) {  
        auto count = this->forPrint.find(i); 
        output << count->second << ":" <<endl;
        for (auto pair : this->Edges[i]) {
            output << " " << pair.first << ": " << pair.second << endl;
        }
    }
    output << endl;
}