//
//  main.cpp
//  AirportConnections
//
//  Created by adeeb mohammed on 14/05/20.
//  Copyright © 2020 adeeb mohammed. All rights reserved.
// find the number of minimum connections needed for
// an airport to reach all the airport

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

class AirportNode{
public:
    std::string airportName;
    std::vector<std::string> connections;
    std::vector<std::string> unreachableConnections;
    bool isReachable;
    
    AirportNode(std::string airportName){
        this->airportName = airportName;
        this->isReachable = true;
        connections = {};
        unreachableConnections = {};
    }
};

std::unordered_map<std::string, AirportNode*> createGraph(std::vector<std::vector<std::string>> routes,
                                                          std::vector<std::string> airports){
    std::unordered_map<std::string, AirportNode*> airportGraph = {};
    // creeating basic airport graph
    for (auto airport : airports) {
        airportGraph[airport] = new AirportNode(airport);
    }
    
    // inserting direct reachable connections from each airport
    for (auto route : routes) {
        auto airport    = route[0];
        auto connection = route[1];
        airportGraph[airport]->connections.push_back(connection);
    }
    
    return airportGraph;
}

// traverse the airports untill
void depthFirstTraverseAirports(std::unordered_map<std::string, AirportNode*> airportGraph,
                                std::string airportName,
                                std::unordered_map<std::string, bool> &visitedAirports
                                ){
    if (visitedAirports.find(airportName) != visitedAirports.end()) {
        return;
    }
    // mark the airport as visited
    visitedAirports.insert({airportName,true});
    // store all the connections
    auto connections = airportGraph[airportName]->connections;
    for (auto connection : connections) {
        depthFirstTraverseAirports(airportGraph, connection, visitedAirports);
    }
    
}

// get the unreachable airports from starting airport
// initially all airports are marked as reachable.
// then if any of the airports are unreachable from the staring airport.

std::vector<AirportNode*> getUnreachableAirportNode(std::unordered_map<std::string, AirportNode*> airportGraph,
                                                    std::vector<std::string> airports,
                                                    std::string startingAirport){
    
    std::vector<AirportNode*> unreachableAirportNodes = {};
    std::unordered_map<std::string, bool> visitedAiports;
    depthFirstTraverseAirports(airportGraph, startingAirport,visitedAiports);
    
    for (auto airport : airports) {
        if(visitedAiports.find(airport) != visitedAiports.end()) {
            continue;
        }
        airportGraph[airport]->isReachable = false;
        unreachableAirportNodes.push_back(airportGraph[airport]);
    }
    return unreachableAirportNodes;
}
// depth first traverse the unreachable airport
void depthFirstAddUnreachableAirportNodes(std::unordered_map<std::string, AirportNode*> airportGraph,
                                          std::unordered_map<std::string, bool> &visitedAirports,
                                          std::vector<std::string> &unreachableConnections,
                                          std::string airportName){
    if (airportGraph[airportName]->isReachable == true) {
        return;
    }
    if (visitedAirports.find(airportName) != visitedAirports.end()) {
        return;
    }
    visitedAirports.insert({airportName,true});
    unreachableConnections.push_back(airportName);
    std::vector<std::string> connections = airportGraph[airportName]->connections;
    for (auto connection : connections) {
        depthFirstAddUnreachableAirportNodes(airportGraph, visitedAirports, unreachableConnections, connection);
    }
}


void markUnreachableConnections(std::unordered_map<std::string, AirportNode*> airportGraph,
                                std::vector<AirportNode*>unreachableAirportNodes){
    // marking for visited unreachable airports
    
    // iterate through each of the unreachable airport nodes to find further connection
    for (auto airportNode : unreachableAirportNodes) {
        std::unordered_map<std::string, bool> visitedAirports = {};
        std::vector<std::string> unreachableConnections = {};
        depthFirstAddUnreachableAirportNodes(airportGraph,
                                             visitedAirports,
                                             unreachableConnections,
                                             airportNode->airportName);
        airportNode->unreachableConnections = unreachableConnections;
    }
}

int getMinNewConnections(std::unordered_map<std::string, AirportNode*> airportGraph,
                         std::vector<AirportNode*>unreachableAirportNodes
                         ){
    int minConnections = 0;
    std::sort(unreachableAirportNodes.begin(), unreachableAirportNodes.end(),
              [](AirportNode *a1,
                 AirportNode *a2)->bool{
        return a2->unreachableConnections.size() <
        a1->unreachableConnections.size();
    });
    for (auto airportNode : unreachableAirportNodes) {
        if (airportNode->isReachable) {
            continue;
        }
        minConnections++;
        for (auto connection : airportNode->unreachableConnections) {
            airportGraph[connection]->isReachable = true;
        }
    }
    return minConnections;
}

int airportConnections(std::vector<std::string> airports ,
                       std::vector<std::vector<std::string>> routes,
                       std::string startingAirport){
    int minConnections = 0;
    auto airportGraph = createGraph(routes, airports);
    auto unreachableAirportNodes = getUnreachableAirportNode(airportGraph, airports, startingAirport);
    markUnreachableConnections(airportGraph, unreachableAirportNodes);
    minConnections = getMinNewConnections(airportGraph, unreachableAirportNodes);
    return minConnections;
}



int main(int argc, const char * argv[]) {
    std::cout << "Program for airport connection" << std::endl;
    std::vector<std::string> airports = {
      "BGI", "CDG", "DEL", "DOH", "DSM", "EWR", "EYW", "HND", "ICN",
      "JFK", "LGA", "LHR", "ORD", "SAN", "SFO", "SIN", "TLV", "BUD",
    };
    std::vector<std::vector<std::string>> routes = {
      {"DSM", "ORD"},
      {"ORD", "BGI"},
      {"BGI", "LGA"},
      {"SIN", "CDG"},
      {"CDG", "SIN"},
      {"CDG", "BUD"},
      {"DEL", "DOH"},
      {"DEL", "CDG"},
      {"TLV", "DEL"},
      {"EWR", "HND"},
      {"HND", "ICN"},
      {"HND", "JFK"},
      {"ICN", "JFK"},
      {"JFK", "LGA"},
      {"EYW", "LHR"},
      {"LHR", "SFO"},
      {"SFO", "SAN"},
      {"SFO", "DSM"},
      {"SAN", "EYW"},
    };
    std::cout << "Min number of connections :" << airportConnections(airports, routes, "LGA")<< std::endl;
    return 0;
}
