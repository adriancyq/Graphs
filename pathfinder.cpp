/*
 * ActorGraph.h
 * Author: Jonathan Chiu (A12113428), Adrian Cordova (A12010305)
 * Date:   CSE 100 Winter 2017 2/28/17
 *
 * This file is meant to exist as a way to find the shortest path 
 * from one actor to another actor through shared movies.
 */

 #include <sstream>
 #include <cstdlib>
 #include <iostream>
 #include <fstream>
 #include <vector>
 #include <queue>
 #include <string>
 #include "ActorGraph.hpp"

 int pathfinder(int argc, char** argv){
 	string weightedArg = argv[2];
 	bool Weighted;
 	ActorGraph graph;
 	int zero = 0;
 	
 	//setting a boolean to use in load later
 	if(weightedArg.compare("w") == zero) Weighted = true;
 	else if(weightedArg.compare("u") == zero) Weighted = false;
 	else { return -1;}

 	ofstream outputFile(argv[4]);
 	ifstream inputFile(argv[3]);
 	outputFile << "(actor)--[movie#@year]-->(actor)--..." << endl;

 	//loading from a file
 	bool loadSuccessful = graph.loadFromFile(argv[1], Weighted);
 	graph.adjacencyVector();
 	unordered_map<string, ActorNode*> tempNode = graph.getActorMap();

 	while( inputFile ){
 		string line;
 		if(!getline( inputFile, line)) break;
 		istringstream lineStream(line);
 		vector<string> distance;
 		while( lineStream ){
 			string nextLine;
 			if(!getline( lineStream, nextLine, '\t')) break;
 			distance.push_back(nextLine);
 		}
 		if(distance.size() != 2) continue;
 		graph.refreshDistance();

 		string firstActor(distance[0]);
 		string lastActor(distance[1]);

 		if( (tempNode.find(firstActor) == tempNode.end()) || (tempNode.find(lastActor) == tempNode.end()) ) return -1;

 		//if weighted
 		if(Weighted == true){
 			graph.Dijkstra(tempNode[firstActor]);
 			graph.printOutWeighted(tempNode[firstActor], tempNode[lastActor], outputFile);
 		}

 		//if unweighted
 		if(Weighted == false){
 			graph.BFSTraverse(tempNode[firstActor]);
 			graph.printOut(tempNode[firstActor], tempNode[lastActor], outputFile);
 		}
 		outputFile << endl;
 	}
 		inputFile.close();
 		outputFile.flush();
 		outputFile.close();
 		return zero;
 }

 void implementation(){
 	cout << "You must give 4 arguments!" << endl;
 }

 	int main(int argc, char **argv){
 		int five = 5;
 		if(argc != five){
 			implementation();
 			return -1;
 		}
 		if(pathfinder(argc, argv) == - 1){
 			implementation();
 			return -1;
 		}
 		return zero;
 	}