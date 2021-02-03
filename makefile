###############################################################
# Program:
#     Assignment 13, Graph
#     Brother Kirby, CS235
# Author:
#     Daniel Perez, Jamie Hurd, Benjamin Dyas
# Summary:
#     This program contains the implementation of a Graph.
#     The graph will be used to find the correct path in
#     different size mazes.
# Time:
#     Pair programming: 18 hours
#     Daniel:           02 hours
#     Benjamin:         02 hours
#		Jamie:            02 hours
# The most difficult part:
#     Daniel:   The most difficult part was understanding how
#               the Vertex support classes worked (we had to 
#               modify it to initialize to -1 so that it didn't
#               confuse the default value "0" with another valid
#               input). Then implementing the process of a
#               breadth first search was complex at first.
#               Finally we spent a couple of hours figuring out
#               how to find the path based on Dijkstra's shortest
#               path algorithm.
#	   Benjamin: The most difficult part was trying out
#               different methods to implement the graph.
#               we came up with ideas on a drawing board and
#               hid road blocks as we were writting the code to
#               make it functional. It took a few tries but we
#               were able to get the graph working.
#	   Jamie:    The most difficult part was figuring out how
#               to store the graph values. We worked through
#               multiple concepts and settled on the
#               implentation of a support class within the
#               graph. This allowed us to create a storage of a
#               source vertex and its associated destinations.
#               This class is also able to store the index value
#               of the parent vertex so we can traverse a path
#               back from the destination to the source.
###############################################################

##############################################################
# The main rule
##############################################################
a.out: assignment13.o graph.o maze.o
	g++ -o a.out assignment13.o graph.o maze.o -g -std=c++11
	tar -cf assignment13.tar *.h *.cpp makefile

##############################################################
# The individual components
#      assignment13.o     : the driver program
#      graph.o            : the graph code
#      maze.o             : the maze reading and solving code
##############################################################
assignment13.o: graph.h vertex.h assignment13.cpp
	g++ -c assignment13.cpp -g -std=c++11

graph.o: graph.h set.h vertex.h graph.cpp
	g++ -c graph.cpp -g -std=c++11

maze.o: maze.cpp maze.h vertex.h graph.h
	g++ -c maze.cpp -g -std=c++11
