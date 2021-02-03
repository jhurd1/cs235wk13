/***********************************************************************
 * Component:
 *    Assignment 13, Graph
 *    Brother Kirby, CS 235
 * Author:
 *    Daniel Perez, Jamie Hurd, Benjamin Dyas
 * Summary:
 *    Create a graph where the node have directional edges.
 ************************************************************************/

#include "graph.h"

using namespace custom;

/********************************************
 * CONSTRUCTOR:    NON-DEFAULT CONSTRUCTOR
 ********************************************/
Graph::Graph(int num)
{   
    //Get size of Graph
    this->num = num;
    
    //Try to allocate an array to hold the Graph
    try
    {
        this->vertices = new SourceVertex[num];
    }
    catch (std::exception &e)
    {
        throw ("New allocation error");
    }
}

/********************************************
 * COPYCONSTRUCTOR: COPY CONSTRUCTOR
 * PARAMETER:      A const Graph to copy
 ********************************************/
Graph::Graph(const Graph &rhs) 
{
    //Copy properties from rhs
    this->num = rhs.size();
    this->numCapacity = rhs.capacity();

    //try to allocate an array to hold the Graph
    try
    {
        this->vertices = new SourceVertex[num];
    }
    catch (std::exception &e)
    {
        throw ("exception or what not");
    }
    
    //if the RHS vertices exist, copy the over
    for(int i = 0; i < size(); i++)
    {
        //copying of existing source verteces (Adjacency list)
        if(rhs.vertices[i].source != nullptr)
            this->vertices[i].source =  new Vertex(*(rhs.vertices[i].source));
        //copying of existing destinations (second Vertex in edge)
        if(rhs.vertices[i].destinations != nullptr)
            this->vertices[i].destinations = new set<Vertex>( *(rhs.vertices[i].destinations) );
    }
}
    

/********************************************
 * ASSIGNOVERLOAD: ASSIGNMENT OPERATOR
 * PARAMETER:      A const Graph to copy
 ********************************************/
Graph & Graph::operator= (const Graph &rhs)
{   
    //Copy properties from rhs
    this->num = rhs.size();
    this->numCapacity = rhs.capacity();

    // clear this graph before assigning rhs values
    if(this->vertices != nullptr)
    {
        clear();
    }
    
    //try to allocate an array to hold the Graph
    try
    {
        this->vertices = new SourceVertex[num];
    }
    catch (std::exception &e)
    {
        throw ("exception or what not");
    }
    
    //if the RHS vertices exist, copy the over
    for(int i = 0; i < size(); i++)
    {   
        //copying of existing source verteces (Adjacency list)
        if(rhs.vertices[i].source != nullptr)
            this->vertices[i].source =  new Vertex(*(rhs.vertices[i].source));
        //copying of existing destinations (second Vertex in edge)
        if(rhs.vertices[i].destinations != nullptr)
            this->vertices[i].destinations = new set<Vertex>( *(rhs.vertices[i].destinations) );
    }

    return *this;
}

/********************************************
 * FUNCTION:     EMPTY
 * DESCRIPTION:  ...
 * PARAMETER:    ...
 * NOTES:         FOR REFERENCE - Not used in
 *               this implementation
 ********************************************/
// bool Graph :: empty()
// {
//     if (size() == 0)
//     {
//         return true;
//     } else 
//     {
//         return false;
//     }
// }

/********************************************
 * FUNCTION:     CAPACITY
 * DESCRIPTION:  ...
 * PARAMETER:    ...
 * NOTES:         FOR REFERENCE - Not used in
 *               this implementation
 ********************************************/
// int Graph :: capacity()
// {
//     Set s;
//     s.insert (Edge (v1, v2));
//     bool added = false;
//     if (s.insert (v1, v2)) 
//     {
//         added = true;
//         numCapacity++;
//     }
//     return numCapacity;
// }

/********************************************
 * FUNCTION:     CLEAR
 * DESCRIPTION:  ...
 * PARAMETER:    ...
 ********************************************/
void Graph :: clear()
{
    //clean-up/memory release
    if(vertices != nullptr) 
    {
        delete [] vertices;
        //not used, but in case of testing for emptiness
        vertices = nullptr;
    }

}

/********************************************
 * FUNCTION:     ADD (2 Vertex)
 * DESCRIPTION:  ...
 * PARAMETER:    Takes two vertex objects
 ********************************************/
void Graph :: add(Vertex v1, Vertex v2)
{
    //boolean that marks whether a vertex (v1)
    //   already exist in the Graph
    bool isFound = false;
    
    //loop through every member Graph
    for(int i = 0; i < num; i++)
    {   
        //if the vertex exists in the adjacency list
        if(vertices[i].source != nullptr && v1 == *(vertices[i].source) ) 
        {
            //if it made it in here, source (Vertex) exists

            //if destinations (set<Vertex>) exists
            if(vertices[i].destinations != nullptr)
            {
                //simply add an element to the set
                vertices[i].destinations->insert(v2);
                
            }
            //if it doesnt...
            else
            {   
                //... we try to allocate it new...
                try
                {
                    vertices[i].destinations = new set<Vertex>();
                    //and we insert the second vertex in the edge
                    vertices[i].destinations->insert(v2);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
                                
                
            }

            //if it made it in here, it was found
            isFound = true;

        }
    }

    //if the vertex wasn't found in the graph
    if (!isFound)
    {
        //search for an empty space in the whole array
        for(int j = 0; j < num; j++)
        {
            //if there is an open slot (nullptr)
            if(vertices[j].source == nullptr)
            {
                //try to allocate a new source vertex
                try
                {
                    vertices[j].source = new Vertex(v1);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }

                //if destinations (set<Vertex>) exists
                if(vertices[j].destinations != nullptr)
                {
                    vertices[j].destinations->insert(v2);
                    return;
                }
                //if it doesn't...
                else
                {
                    //try to allocate a new set of edge destination vertices 
                    try
                    {
                        set<Vertex> tempSet;
                        vertices[j].destinations = new set<Vertex>(tempSet);
                        //and insert the current destination vertex into it
                        vertices[j].destinations->insert(v2);
                        return;
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
    
                }
            }   
        }
    }
}

/********************************************
 * FUNCTION:     ADD (2 Vertex)
 * DESCRIPTION:  ...
 * PARAMETER:    Takes one vertex object and
 *               one set<vertex>
 ********************************************/
void Graph :: add(Vertex v1, const set<Vertex> & s)
{
    //boolean that marks whether a vertex (v1)
    //   already exist in the Graph
    bool isFound = false;

    //loop through every member Graph
    for(int i = 0; i < num; i++)
    {
        //if the source vertex exists
        if(vertices[i].source != nullptr && v1 == *(vertices[i].source) ) 
        {
            //if it made it in here, source (Vertex) exists

            //if destinations (set<Vertex>) exists
            if(vertices[i].destinations != nullptr)
            {
                //simply add an element to the set
                *(vertices[i].destinations) = s;
                
            }
            //if the edge vertex destinations does not exist
            else
            {
                //try to allocate it new
                try
                {
                    //and pass the set received into it
                    vertices[i].destinations = new set<Vertex>(s);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
                                
                
            }

            //if it made it in here, it was found
            isFound = true;

        }
    }
    
    
    //if the vertex wasn't found in the graph
    if (!isFound)
    {
        //search for an empty space in the whole array
        for(int j = 0; j < num; j++)
        {
            //if there is an open slot (nullptr)
            if(vertices[j].source == nullptr)
            {
                //try to allocate the new source vertex
                try
                {
                    vertices[j].source = new Vertex(v1);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }

                //if destinations (set<Vertex>) exists
                if(vertices[j].destinations != nullptr)
                {   
                    //copy the received set into it
                    *(vertices[j].destinations) = s;
                }
                //if it doesnt exist
                else
                {
                    //try to allocate a new vertex destination set from
                    //   the received set
                    try
                    {
                        vertices[j].destinations = new set<Vertex>(s);
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
    
                }
            }   
        }
    }
}

/********************************************
 * FUNCTION:     ISEDGE
 * DESCRIPTION:  checks if an edge exists
 *               between two vertices
 * PARAMETER:    takes two vertices
 ********************************************/
bool Graph :: isEdge(Vertex v1, Vertex v2) const
{
   // for loop for incrementing vertices index
   for(int i = 0; i < num; i++)
    {
        // found the vertices index where v1 is stored
        if(vertices[i].source != nullptr && v1 == *(vertices[i].source) ) 
        {
            // check if the v1 vertex has edges 
            if(vertices[i].destinations != nullptr)
            {
                // iterator for traversing the destinations
                set<Vertex>::iterator it = vertices[i].destinations->find(v2);
                
                // check if the v2 destination is found
                if(*it == v2 && it != vertices[i].destinations->end())
                {
                    return true; // an edge was found between v1 and v2
                }
                else
                {
                    return false; // an edge was not found between v1 and v2
                }              
            }
            else // if the v1 vertex does not have edges
            {
                return false;
            }
            
        }
    }

    return false;
}

/********************************************
 * FUNCTION:     FINDEDGES
 * DESCRIPTION:  Finds the edges of a vertex
 * PARAMETER:    vertex
 ********************************************/
set<Vertex> Graph :: findEdges(Vertex v)
{
    // for loop for incrementing vertices index
    for(int i = 0; i < num; i++)
    {
        if(vertices[i].source != nullptr && v == *(vertices[i].source) )
        {
            // returns a set of all the found edges of a vertex
            set<Vertex> s(*(vertices[i].destinations));
            return s;
        }
    }
    return set<Vertex>(); // returns a empty set
}

/********************************************
 * FUNCTION:     FINDPATH
 * DESCRIPTION:  Finds a path from a source
 *               vertex to a destination 
 *               vertex
 * PARAMETER:    takes two vertices
 ********************************************/
vector<Vertex> Graph :: findPath (Vertex source, Vertex destination)
{
    custom::queue<int> toVisit; // all the values to search and store here
    vector<Vertex> path; // store the parent values to contain the path here
    
    //Index assignment
    //   This is part of the logic to not have to deal with finding or
    //   storing location data or pointers from the copy of the destination
    //   set returned from findEdges: This way whenever we receive destinations
    //   we can simply access it's location by it's array index
    for(int i = 0; i < num; i++)
    {
        //if the vertex exists
        if(vertices[i].source != nullptr)
        {
            vertices[i].arrayIndex = i;
        }
    }
    

    // find and add the starting point to the set
    int found = -1;

    //Search for the index at which the initial Vertex resides
    //   this is not needed but this way we can acommodate starting from
    //   a different coordinate than from the very beginning
    for(int i = 0; i < num; i++)
    {
        if(vertices[i].source != nullptr && source == *(vertices[i].source) )
        {
           found = i;
        }
    }
    
    toVisit.push(found); // adds the value to the set

    // set <int> :: iterator it; // use an iterator to traverse in breadth first search
    // it = toVisit.begin(); //declare it as s.begin(); get ready to traverse

    // traverse through the edges starting at the source value
    //for (it; it != toVisit.end(); ++it) //now, traverse s and store the vertices in the set
    while (!toVisit.empty())
    {
        //save the front value and pop it out of the set
        int index = toVisit.front();
        toVisit.pop();
        
        if(*(vertices[index].source) == destination) // if the destination is reached
        {
            //this prevents the loop to continue after the vertex is found
            //   marginal optimization
            break;
        }
        else // go through the edges (the breadth-first action truly transpires here)
        {
            // if the destinations set exists   
            if(vertices[index].destinations != nullptr)
            {
                set<Vertex> allEdges(findEdges( *(vertices[index].source) ) );
                set<Vertex>::iterator it = allEdges.begin();
                
                //Loop through all members of the set: This way we make sure we visit
                //   every destination vertex of an edge before moving on to the next
                //   source vertex
                for(;it != allEdges.end(); it++)
                {
                    for(int j = 0; j < num; j++ )
                    {
                        //if the vertex exists and it hasn't been visited before
                        //    notice how we are looking for the match from the destination vertex for
                        //    a particular source vertex but we change the visited variable in
                        //    the adjacency list
                        if(vertices[j].source != nullptr && (*it == *(vertices[j].source)) && vertices[j].visited == false)
                        {
                            toVisit.push(vertices[j].arrayIndex); //path index added to set
                            vertices[j].visited = true; // vertex has been visited
                            vertices[j].parent = index; // parent index is updated to follow the path back 
                        }
                    }

                }
            }
        }
    }
    
    //We traverse the path from the end to the beginning by moving unto the next Vertex's
    //   parent... following the breadcrumbs
    int parentIndex = 0;
    for(int j = 0; j < num; j++ )
        {

            if(vertices[j].source != nullptr && (destination == *(vertices[j].source)))
            {
                
                path.push_back(*(vertices[j].source));
                parentIndex = vertices[j].parent;
                break;
            }
        }
        
    // changes the direction of the found path from destination->source
    // to source->destination
    int maxCount = vertices[0].source->getMax();
    while(maxCount--)
    {
        if( vertices[parentIndex].source != nullptr && *(vertices[parentIndex].source) != source)
        {
            path.push_back(*(vertices[parentIndex].source));
            parentIndex = vertices[parentIndex].parent;

        }
        else
        {
            path.push_back(*(vertices[parentIndex].source));
            break;
        }
        
    }
    
    return path; // returns a path from the source to the destination
}
