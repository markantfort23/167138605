/*nodes.cpp*/

//
// A collection of nodes in the Open Street Map.
// 
// Mark Fortes
// Northwestern University
// CS 211: Winter 2023

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <algorithm>
#include <cassert>

#include "nodes.h"
#include "osm.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


//
// readMapNodes
//
// Given an XML document, reads through the document and 
// stores all the nodes into the given vector. Each node
// is a point on the map, with a unique id along with 
// (lat, lon) position. Some nodes are entrances to buildings,
// which we capture as well.
//
void Nodes::readMapNodes(XMLDocument& xmldoc)
{
  XMLElement* osm = xmldoc.FirstChildElement("osm");
  assert(osm != nullptr);

  //
  // Parse the XML document node by node: 
  //
  XMLElement* node = osm->FirstChildElement("node");

  while (node != nullptr)
  {
    const XMLAttribute* attrId = node->FindAttribute("id");
    const XMLAttribute* attrLat = node->FindAttribute("lat");
    const XMLAttribute* attrLon = node->FindAttribute("lon");

    assert(attrId != nullptr);
    assert(attrLat != nullptr);
    assert(attrLon != nullptr);

    long long id = attrId->Int64Value();
    double latitude = attrLat->DoubleValue();
    double longitude = attrLon->DoubleValue();

    //
    // is this node an entrance? Check for a 
    // standard entrance, the main entrance, or
    // one-way entrance.
    //
    bool entrance = false;

    if (osmContainsKeyValue(node, "entrance", "yes") ||
      osmContainsKeyValue(node, "entrance", "main") ||
      osmContainsKeyValue(node, "entrance", "entrance"))
    {
      entrance = true;
    }
    // Add node to vector:
    this->MapNodes.emplace(id, Node(id, latitude, longitude, entrance));

    // next node element in the XML doc:
    node = node->NextSiblingElement("node");
  }
}

//
// sortByID
//
// sorts the nodes into ascending order by ID.
//
void Nodes::sortByID()
{
}

//
// find
// 
// Searches the nodes for the one with the matching ID, returning
// true if found and false if not. If found, a copy of the node 
// is returned via the node parameter, which is passed by reference.
//
bool Nodes::find(long long id, double& lat, double& lon, bool& isEntrance)
{
  auto ptr = this->MapNodes.find(id);

  if (ptr == this->MapNodes.end()) {  // not found:
    return false;
  }
  else {  // found:
    lat = ptr->second.getLat();
    lon = ptr->second.getLon();
    isEntrance = ptr->second.getIsEntrance();

    return true;
  }
}

//
// accessors / getters
//
int Nodes::getNumMapNodes() {
  return (int) this->MapNodes.size();
}
// allow foreach through the map
std::map<long long, Node>::iterator Nodes::begin()
{
  return this->MapNodes.begin();
}
std::map<long long, Node>::iterator Nodes::end()
{
  return this->MapNodes.end();
}
