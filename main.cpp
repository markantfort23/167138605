/*main.cpp*/

//
// Program to input Nodes (positions), Buildings, and footways and utilizes graph ADT to navigate
// from an Open Street Map file.
// 
// Mark Fortes
// Northwestern University
// CS 211: Winter 2023
// 

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

#include "building.h"
#include "buildings.h"
#include "footway.h"
#include "footways.h"
#include "node.h"
#include "nodes.h"
#include "osm.h"
#include "tinyxml2.h"
#include "graph.h"
#include "dist.h"

using namespace std;
using namespace tinyxml2;

class compare //class to use in priority queue to compare
{
   public:   
      bool operator() (const pair<long long, double>& p1, const pair<long long, double>& p2) const{
         if (p1.second > p2.second) return true;
         else if (p1.second < p2.second) return false;
         else return p1.first > p2.first;
      }};

//helper function for navigate function to find distance between points and identify closest node id and distance and more
long long nav_helper(double lat2, double lon2, Footways& footways, Nodes& nodes)
{
  const double INF = numeric_limits<double>::max();
  double min = INF;
  long long id1 = 0;
  long long id2 = 0;

  for (Footway& f:footways.MapFootways)
  {
    for (long long cur:f.NodeIDs)
    {
      double lat = 0.0;
      double lon = 0.0;
      bool isEntrance = false;
      bool isFound = nodes.find(cur, lat, lon, isEntrance);

      assert(isFound); //make sure its found with function
      double distance= distBetween2Points(lat2, lon2, lat, lon);

      if (min > distance) 
      {
        min = distance;
        id1 = cur;
        id2 =f.ID;
      }
    }
  }
  cout << " Closest footway ID " << id2 << ", node ID " << id1 << ", distance " << min << endl;
  
  return id1;
}

//constructs graph iterating through footways and nodes
void buildGraph(Nodes& nodes, graph& G, Footways& footways){
  for (auto& pair : nodes){
    G.addVertex(pair.first); //adds vertices on nodes
  }

  for (Footway footway : footways.MapFootways)
  {
    for(size_t i = 0; i < footway.NodeIDs.size() - 1; i++) // add edges on footways
    {
    long long beg = footway.NodeIDs[i];
    long long dest = footway.NodeIDs[i + 1];
    double lat, lon, lat2, lon2 = 0.0; 
    bool isEntrance = false;
    bool isEntrance2 = false;

    nodes.find(beg, lat, lon, isEntrance);    
    nodes.find(dest, lat2, lon2, isEntrance2);   

    double weight = distBetween2Points(lat, lon, lat2, lon2);        
    G.addEdge(beg, dest, weight);    
    
    weight = distBetween2Points(lat2, lon2, lat, lon);  //both directions  
    G.addEdge(dest, beg, weight); 
  }  
  }
}

//assurance we have built the graph correctly before continuing by ensuring certain ID functions correctly with lookup
void sanityCheck(graph& G, Footways& footways)
{
  for (Footway f : footways.MapFootways){
    if(f.ID == 986532630)
    {
      cout << "Graph check of Footway ID " << f.ID << endl;

      for (size_t i = 0; i < f.NodeIDs.size() - 1; i++)
      {
        long long beg = f.NodeIDs[i];
        long long dest = f.NodeIDs[i + 1]; 
        double weight = 0.0;

        G.getWeight(beg, dest, weight);

        cout << " Edge: (" << beg <<", " << dest << ", " << weight << ")" <<endl;
        
        G.getWeight(dest, beg, weight);

        cout << " Edge: (" << dest <<", " << beg << ", " << weight << ")" <<endl; //both directions
      }
    }
  }
}

//dijkstra algorithm helper function, used to find the shortest path between nodes of the graph
vector<long long> Dijkstra(graph& G,  //return a predecessors array
  long long& sV, 
  map<long long, double>& distances,
  map<long long, long long>& pV)
{
  vector<long long>  at;

  priority_queue<pair<long long, double>, vector<pair<long long, double>>, compare> unvisitedQueue; //priority queue for dijkstra
   
  const double INF = numeric_limits<double>::max();

  for (long long cV : G.getVertices())
  {
      distances[cV] = INF;

      unvisitedQueue.push(make_pair(cV, INF));
      pV[cV] = -1;
   }
   
   distances[sV] = 0;

   unvisitedQueue.push(make_pair(sV, 0.0));
   
   while(!unvisitedQueue.empty())
   {
      long long cV = unvisitedQueue.top().first; // 
      unvisitedQueue.pop();
      
      if (distances[cV] == INF) break;

      else if (find(at.begin(), at.end(), cV) != at.end()) continue;
      else at.push_back(cV);
      
      auto neighbors = G.neighbors(cV);
      for (long long aV : neighbors)
      {
         double edgeW = 0.0;
         G.getWeight(cV, aV, edgeW); 
         
         double altPath = distances[cV] + edgeW;
         
         if(altPath < distances[aV]){
            distances[aV] = altPath;
            pV[aV] = cV;
            unvisitedQueue.push(make_pair(aV, altPath));
         }
      }
   }
   return at;
}

//Input partial or full name of a building and searches for building, output the building’s name and approximate location, repeats for destination unless not found
void navigate(graph G, Buildings& B, Nodes& nodes, Footways& footways)
{  
  const double INF = numeric_limits<double>::max(); //represents infinity

  cout << "Enter start building name (partial or complete)>" << endl;
  string name;
  getline(cin, name); // multiline input
  Building Build1(0, "", "");//virtually empty

  for (Building& Building : B.MapBuildings)
  {
    if (Building.Name.find(name) != string::npos) 
    {
      Build1 = Building;
      break;
    } 
  }
  if (Build1.ID == 0)
  {
    cout << "**Start building not found" << endl;
    return;    
  }
  cout << " Name: " << Build1.Name << endl;   

  auto pair = Build1.getLocation(nodes);

  cout << " Approximate location: (" << pair.first <<", " << pair.second << ")" << endl;

  long long start;
  start = nav_helper(pair.first, pair.second, footways, nodes);

  cout << "Enter destination building name (partial or complete)>" << endl;
  string name2;
  getline(cin, name2); //multiline
  Building Build2(0, "", "");

  for (Building& Building : B.MapBuildings)
  {
    if (Building.Name.find(name2) != string::npos) {
      Build2 = Building;

      break;
    } 
  }

  if (Build2.ID == 0)
  {
    cout << "**Destination building not found" << endl;

    return;    
  }

  cout << " Name: " << Build2.Name << endl;      
  pair = Build2.getLocation(nodes);
  cout << " Approximate location: (" << pair.first <<", " << pair.second << ")" << endl;

  long long dest;
  dest = nav_helper(pair.first, pair.second, footways, nodes);
  
  cout << "Shortest weighted path:" << endl;

  long long sV = start;

  map<long long, long long> pV;
  map<long long, double> distances;

  vector<long long> visited = Dijkstra(G,sV,distances, pV);

  double distanceDest = distances[dest];

  if (distanceDest == INF)
  {
    cout << "**Sorry, destination unreachable" << endl;
  } 
  else
  {
    cout << " # nodes visited: " << visited.size() << endl;
    cout << " Distance: " << distanceDest << " miles"<<endl;
    cout << " Path: ";
    stack<long long> st; //LIFO, elements are inserted/extracted only from one end of the container
    long long vertex = dest;
    st.push(vertex);

    while(pV[vertex] != -1)
    {
      st.push(pV[vertex]); //push to stack
      vertex = pV[vertex];
    }
    while (!st.empty()) 
    {
      vertex = st.top(); //refers to newest element of stack
      st.pop();

      cout << vertex;
      if (st.size() > 0)
      {
        cout << "->";
      }
    }

    cout << endl;
  }
}


//
// main
//
int main()
{
  cout << setprecision(12);
  XMLDocument xmldoc;
  Nodes nodes;
  Buildings buildings;
  Footways footways;
  graph G;

  
  cout << "** NU open street map **" << endl;

  string filename;

  cout << endl;
  cout << "Enter map filename> " << endl;
  getline(cin, filename);

  //
  // 1. load XML-based map file 
  //
  if (!osmLoadMapFile(filename, xmldoc))
  {
    // failed, error message already output
    return 0;
  }
  
  //
  // 2. read the nodes, which are the various known positions on the map:
  //
  nodes.readMapNodes(xmldoc);

  //
  // NOTE: let's sort so we can use binary search when we need 
  // to lookup nodes.
  //
  nodes.sortByID();

  //
  // 3. read the university buildings:
  //
  buildings.readMapBuildings(xmldoc);

  //
  // 4. read the footways, which are the walking paths:
  //
  footways.readMapFootways(xmldoc);

  // builds graph using nodes and footways
  buildGraph(nodes, G, footways);

  //
  // 5. stats
  //
  cout << "# of nodes: " << nodes.getNumMapNodes() << endl;
  cout << "# of buildings: " << buildings.getNumMapBuildings() << endl;
  cout << "# of footways: " << footways.getNumMapFootways() << endl;

  cout << "# of graph vertices: " << G.NumVertices() << endl;  
  cout << "# of graph edges: " << G.NumEdges() << endl;

  while (true)
  {
    string name;

    cout << endl;
    cout << "Enter building name, * to list, @ to navigate, or $ to end> " << endl;

    getline(cin, name);

    if (name == "$") {
      break;
    }
    else if (name == "*") {
      buildings.print();
      
    }
    else if (name == "@") {
      navigate(G, buildings, nodes, footways);
    }
    else if (name == "!")
    {
      sanityCheck(G, footways);
    }
    else {
      buildings.findAndPrint(name, nodes, footways);
    }

  }//while

  //
  // done:
  //
  cout << endl;
  cout << "** Done  **" << endl;
  cout << "# of calls to getID(): " << Node::getCallsToGetID() << endl;
  cout << "# of Nodes created: " << Node::getCreated() << endl;
  cout << "# of Nodes copied: " << Node::getCopied() << endl;
  cout << endl;

  return 0;
}
