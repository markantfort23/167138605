/*footway.cpp*/

//
// A footway / path in the Open Street Map.
// 
// Mark Fortes
// Northwestern University
// CS 211: Winter 2023
// 

#include "footway.h"

using namespace std;


//
// constructor
//
Footway::Footway(long long id)
  : ID(id)
{
}

//
// intersectWithBuilding
// 
// Checks to see if this footway intersects with the given building,
// i.e. shares a node in common. Returns true if so, false if not.
//
bool Footway::intersectWithBuilding(Building& B)
{
  for (long long nodeid : this->NodeIDs)
  {
    if (B.containsThisNode(nodeid))
      return true;
  }

  //
  // if get here, no intersection:
  //
  return false;
}

//
// adds the given nodeid to the end of the vector
//
void Footway::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}
