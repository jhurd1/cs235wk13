/***********************************************************************
 * Component:
 *    Assignment 13, Graph
 *    Brother Kirby, CS 235
 * Author:
 *    Daniel Perez, Jamie Hurd, Benjamin Dyas
 * Summary:
 *    Create a graph where the node have directional edges.
 ************************************************************************/


#ifndef GRAPH
#define GRAPH

#include "set.h"
#include "vertex.h"
#include "list.h"
#include "queue.h"
#include "vector.h"

namespace custom
{
   /************************************************
    * CLASS: GRAPH
    * Container class element that contains info
    * for an array-of-sets graph
    ***********************************************/
   class Graph
   {
      private:
         //support class: It holds a source vertex and
         //   all possible vertices that it can form an edge
         //   with; denoted by source and destinations.
         //   it also holds useful information like a flag
         //   for whether a vertex has been visited yet
         //   and which vertex it was visited from (parent).
         //   arrayIndex helps to avoid unnecessary searches
         //   and copies by holding a value of where the 
         //   array is storing a vertex (by it's index)
         class SourceVertex 
         {
            public:
            SourceVertex() : source{nullptr}, destinations{nullptr},
                             arrayIndex{-1}, visited{false}, parent{-1} {}
            ~SourceVertex() 
            {
               if(source)
                  delete source;
               if(destinations)
                  destinations->clear();
            }

            Vertex * source;
            set<Vertex> * destinations;
            int arrayIndex;
            int parent;
            bool visited;

         };

         //other private member variables
         SourceVertex * vertices;
         int num;
         int numCapacity;
         

      public:
         //CONSTRUCTORS AND DESTRUCTOR
         Graph (int num);
         Graph (const Graph & rhs);
         ~Graph () { clear(); }
         
         // standard Graph interfaces
         int size () const { return num; }
         int capacity() const {return numCapacity;}
         void clear ();
         void add (Vertex v1, Vertex v2);
         void add (Vertex v1, const set<Vertex> & s);
         bool isEdge( Vertex v1, Vertex v2) const;
         set<Vertex> findEdges (Vertex v);
         vector<Vertex> findPath (Vertex source, Vertex destination);
         

         //OVERLOADED OPERATORS
            Graph & operator= (const Graph &rhs);

   };
}
#endif // GRAPH_H
