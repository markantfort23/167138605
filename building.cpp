/*building.cpp*/

//
// A building in the Open Street Map.
// 
// Mark Fortes
// Northwestern University
// CS 211: Winter 2023
// 

#include <iostream>

#include "building.h"
#include "node.h"
#include "nodes.h"

using namespace std;


//
// constructor
//
Building::Building(long long id, string name, string streetAddr)
  : ID(id), Name(name), StreetAddress(streetAddr)
{
}

//
// containsThisNode
//
// Returns true if the building's nodes contains the given node id,
// false if not.
//
bool Building::containsThisNode(long long nodeid)
{
  for (long long id : this->NodeIDs)
  {
    if (nodeid == id)
      return true;
  }

  // 
  // if get here, not found:
  //
  return false;
}

//
// print
// 
// prints information about a building --- id, name, etc. -- to
// the console. The function is passed the Nodes for searching 
// purposes.
//
void Building::print(Nodes& nodes)
{
  cout << this->Name << endl;
  cout << "Address: " << this->StreetAddress << endl;
  cout << "Building ID: " << this->ID << endl;
  cout << "Approximate Location: " << "(" << Building::getLocation(nodes).first << ", " << Building::getLocation(nodes).second << ")" << endl;

  cout << "Nodes:" << endl;
  for (long long nodeid : this->NodeIDs)
  {
    cout << "  " << nodeid << ": ";

    double lat = 0.0;
    double lon = 0.0;
    bool entrance = false;

    bool found = nodes.find(nodeid, lat, lon, entrance);

    if (found) {
      cout << "(" << lat << ", " << lon << ")";

      if (entrance)
        cout << ", is entrance";

      cout << endl;
    }
    else {
      cout << "**NOT FOUND**" << endl;
    }
  }//for
}

//
// adds the given nodeid to the end of the vector.
//
void Building::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}

//lookup each node id and retrieve the node’s avg latitude and longitude
pair<double, double> Building::getLocation(Nodes& N) 
{
  double lat = 0.0;
  double lon = 0.0;
  double total_lat = 0.0;
  double total_lon = 0.0;
  double avg_lat = 0.0;
  double avg_lon = 0.0;
  bool isEntrance = false;

  for (long long n:NodeIDs)
  {
    N.find(n, lat, lon, isEntrance);
    total_lat = total_lat + lat;
    total_lon = total_lon + lon;
  }
  avg_lat = total_lat / NodeIDs.size();
  avg_lon = total_lon / NodeIDs.size();
  return make_pair(avg_lat, avg_lon); //return avg lat and lon
}