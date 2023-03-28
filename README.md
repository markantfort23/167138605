# Open Street Map Navigation of Northwestern's Campus

**Performs basic navigation of Northwestern campus Open Street Map. Builds a graph ADT of the NU campus from nodes and footways given an OSM file, and uses Dijkstra’s algorithm to navigate between buildings.**

A **building** Defines a campus building with a name (e.g. "Mudd"), a street address (e.g. "2233 Tech Dr"), and the IDs of the nodes that define the position / outline of the building.

A **node** is a point on the map, with a unique ID and the position in GPS (lat, lon) terms. A node may also track other information, in particular whether this node denotes the entrance to a building.

A **footway** is a path in the map. The ID uniquely identifies the footway. The vector defines points (Nodes) along the footway, and it always contains at least two points.

## To run:

To build:
````shell
make build
````

To run files:
````shell
./a.out
````

