/*
  <one line to give the program's name and a brief idea of what it does.>
  Copyright (C) 2015  <copyright holder> <email>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "VisitSolver.h"
#include "ExternalSolver.h"
#include <map>
#include <string>
#include <iostream>
#include <typeinfo>
#include <sstream>
#include <fstream>
#include <math.h> 
#include <vector>
#include <algorithm>
#include <random>
#include <algorithm>

#include "armadillo"
#include <initializer_list>

using namespace std;
using namespace arma;

map <string, vector<double> > region_mapping;

double adiajency_matrix[30][30] = {0.0};

extern "C" ExternalSolver* create_object(){
  return new VisitSolver();
}

extern "C" void destroy_object(ExternalSolver *externalSolver){
  delete externalSolver;
}

VisitSolver::VisitSolver(){

}

VisitSolver::~VisitSolver(){

}

void VisitSolver::loadSolver(string *parameters, int n){
  starting_position = "r0";
  string Paramers = parameters[0];

  // Indirect variables: Whatever compuations we do, those values are stored in these
  // indirect variable.
  char const *x[]={"dummy"};

  // Direct variables: These are the normal PDDL variable that just stored values.
  char const *y[]={"act-cost","triggered"};
  parseParameters(Paramers);
  affected = list<string>(x,x+1);
  dependencies = list<string>(y,y+2);

  // We are loading the way point file in the planner.
  string waypoint_file = "visits_domain/waypoint.txt"; 
  parseWaypoint(waypoint_file);
}

double minimalPath(string from, string to){
  int from_index = stoi(from.substr(1,2));
  int to_index = stoi(to.substr(1,2));

  // initialize distances and visited arrays
  vector<double> distances(30, numeric_limits<double>::infinity());
  vector<bool> visited(30, false);
  distances[from_index] = 0.0;

  // run Dijkstra's algorithm
  for (int i = 0; i < 30 - 1; i++) {
    // find the unvisited node with the smallest distance
    int min_index = -1;
    double min_distance = numeric_limits<double>::infinity();
    for (int j = 0; j < 30; j++) {
      if (!visited[j] && distances[j] < min_distance) {
        min_index = j;
        min_distance = distances[j];
      }
    }

    // mark the node as visited
    visited[min_index] = true;

    // update distances to neighboring nodes
    for (int j = 0; j < 30; j++) {
      if (!visited[j] && adiajency_matrix[min_index][j] > 0.0) {
        double new_distance = distances[min_index] + adiajency_matrix[min_index][j];
        if (new_distance < distances[j]) {
          distances[j] = new_distance;
        }
      }
    }
  }

  return distances[to_index];
}

map<string,double> VisitSolver::callExternalSolver(map<string,double> initialState,bool isHeuristic){
  map<string, double> toReturn;
  map<string, double>::iterator iSIt = initialState.begin();
  map<string, double>::iterator isEnd = initialState.end();
  double dummy;
  double act_cost;

  map<string, double> trigger;

  for(;iSIt!=isEnd;++iSIt){

    string parameter = iSIt->first;
    string function = iSIt->first;
    double value = iSIt->second;

    function.erase(0,1);
    function.erase(function.length()-1,function.length());
    int n=function.find(" ");

    if(n!=-1){
      string arg=function;
      string tmp = function.substr(n+1,5);

      function.erase(n,function.length()-1);
      arg.erase(0,n+1);

      // This is the triggered function from which we our communicating with planner
      if(function=="triggered"){
        trigger[arg] = value>0?1:0;
        if (value>0){

          // Here we are storing these strings "from" and "to" are regions, need to extract wps (poses)
          string from = tmp.substr(0,2);
          string to = tmp.substr(3,2);

          act_cost = minimalPath(from, to);
          cout << endl;
          cout << "Distance from " << from << " to " << to << ": " << act_cost << endl;
        }
      }
    }
    else{
      if(function=="dummy"){
        dummy = value;
      }
      else if(function=="act-cost"){
        act_cost = value;
      }
    }
  }

  double results = calculateExtern(dummy, act_cost);

  if (ExternalSolver::verbose){
    cout << "(dummy) " << results << endl;
  }

  toReturn["(dummy)"] = results;
  return toReturn;
}

list<string> VisitSolver::getParameters(){
  return affected;
}

list<string> VisitSolver::getDependencies(){
  return dependencies;
}


void VisitSolver::parseParameters(string parameters){
  int curr, next;
  string line;
  ifstream parametersFile(parameters.c_str());
  if (parametersFile.is_open()){
    while (getline(parametersFile,line)){
      curr=line.find(" ");
      string region_name = line.substr(0,curr).c_str();
      curr=curr+1;
      while(true ){
        next=line.find(" ",curr);
        region_mapping[region_name].push_back(line.substr(curr,next-curr).c_str());
        if (next ==-1)
          break;
          curr=next+1;
      }                
    }
  }
}

double VisitSolver::calculateExtern(double external, double total_cost){
  double cost = total_cost;
  return cost;
}

// Structure to store the edge of the graph
struct Edge {
    int source;
    int destination;
    double weight;
};

// Function to compare two edges according to their weight
bool edgeComparison(const Edge& edge1, const Edge& edge2) {
    return edge1.weight < edge2.weight;
}

// Function to find the set of an element i
int findSet(vector<int>& set, int x) {
    if (set[x] == x)
        return x;
    return findSet(set, set[x]);
}

// Function to do union of two sets x and y (uses union by rank)
void unionSet(vector<int>& set, int x, int y) {
    int rootX = findSet(set, x);
    int rootY = findSet(set, y);
    set[rootX] = rootY;
}

// Function to construct MST using Kruskal's algorithm
void kruskal(vector<Edge>& edges, int numNodes, int numMaxConnection) {
    // Order edges in increasing order of weight
    sort(edges.begin(), edges.end(), edgeComparison);

    vector<int> sets(numNodes);
    for (int i = 0; i < numNodes; i++)
        sets[i] = i;

    vector<int> connections(numNodes);
    for (int i = 0; i < numNodes; i++)
        connections[i] = 0;

    int edgesAdded = 0;
    int indexEdge = 0;

    while (edgesAdded < numNodes - 1 && indexEdge < edges.size()) {
        Edge currentEdge = edges[indexEdge++];
        int rootSource = findSet(sets, currentEdge.source);
        int rootDestination = findSet(sets, currentEdge.destination);

        if (rootSource != rootDestination && connections[currentEdge.source] < numMaxConnection && connections[currentEdge.destination] < numMaxConnection) {
            unionSet(sets, rootSource, rootDestination);
            adiajency_matrix[currentEdge.source][currentEdge.destination] = currentEdge.weight;
            adiajency_matrix[currentEdge.destination][currentEdge.source] = currentEdge.weight;
            connections[currentEdge.source]++;
            connections[currentEdge.destination]++;
            edgesAdded++;
        }
    }
}

// Function to find the waypoint forbidden
bool checkForbiddenWaypoint(double x, double y, const vector<tuple<double, double, double>>& forbiddenWaypoint) {
  for (const auto& coord : forbiddenWaypoint) {
    if (x == get<0>(coord) && y == get<1>(coord)) {
      return true;  // The coordinates are forbidden
    }
  }
  return false;  // The coordinates are allowed
}

// Function to round a double to a given number of decimal places
double roundDecimal(double value, int decimalPlaces) {
  double multiplier = pow(10.0, decimalPlaces);
  return round(value * multiplier) / multiplier;
}

// Function to compute the distance between two waypoints
double computeDistance(tuple<double, double, double> wp1, tuple<double, double, double> wp2) {
  double dx = get<0>(wp2) - get<0>(wp1);
  double dy = get<1>(wp2) - get<1>(wp1);
  return sqrt(dx * dx + dy * dy);
}

// Function to connect the waypoints in the graph
void connectWaypoints(const vector<tuple<double, double, double>>& randomWaypoints) {
  int numNodes = 30;  // Number of nodes in the graph
  int maxConnections = 4; // Maximum number of connections per node
  
  ofstream outfile("../visits_domain/connections.txt");
  if (!outfile.is_open()) {
    cerr << "Unable to open connections.txt for writing." << endl;
    return;
  }

  int numWaypoints = randomWaypoints.size();

  vector<Edge> edges;

  for (int i=0;i<numWaypoints;i++){
    for (int j=0;j<numWaypoints;j++){
      if (i!=j){
        double distance = computeDistance(randomWaypoints[i], randomWaypoints[j]);
        edges.push_back({i, j, distance});
      }
    }
  }
  kruskal(edges, numWaypoints, maxConnections);
  for (int i=0;i<numWaypoints;i++){
    outfile << "Waypoint " << i << " is connected to: ";
    for (int j=0;j<numWaypoints;j++){
        if (adiajency_matrix[i][j] != 0.0)
        {
          
        outfile << j << " ";
        }
    }
    outfile << endl;
  }
}

// Function to generate random waypoints
void generateRandomWaypoints()
{
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<double> dist(-3.0, 3.0);  // Assuming the environment dimensions are 6m x 6m

  ofstream outfile("../visits_domain/waypoint.txt");
  if (!outfile.is_open()) {
    cerr << "Unable to open waypoint.txt for writing." << endl;
    return;
  }

  vector<tuple<double, double, double>> forbiddenWaypoint = { {0, 0, 0}, {3, 0, 0} };  // Specify the forbidden coordinates here
  vector<tuple<double, double, double>> randomWaypoint(30, make_tuple(0.0, 0.0, 0.0));

  // set the default waypoints (the ones that are forbidden)
  outfile << "wp0" << " [" << "0" << "," << "0" << "," << "0]";                 // The zero waypoint is the starting position of the robot       
  outfile << endl << "wp1" << " [" << "-2.5" << "," << "2.5" << "," << "0]";    // The first waypoint is the first group table
  outfile << endl << "wp2" << " [" << "2.5" << "," << "2.5" << "," << "0]";     // The second waypoint is the second group table
  outfile << endl << "wp3" << " [" << "-2.5" << "," << "-2.5" << "," << "0]";   // The third waypoint is the third group table
  outfile << endl << "wp4" << " [" << "2.5" << "," << "-2.5" << "," << "0]";    // The fourth waypoint is the fourth group table
  outfile << endl << "wp5" << " [" << "3" << "," << "0" << "," << "0]";         // The fifth waypoint is the delivery table

  // Insert the forbidden waypoints into the vector of forbidden waypoints
  randomWaypoint[0] = make_tuple(0.0, 0.0, 0.0);
  randomWaypoint[1] = make_tuple(-2.5, 2.5, 0.0);
  randomWaypoint[2] = make_tuple(2.5, 2.5, 0.0);
  randomWaypoint[3] = make_tuple(-2.5, -2.5, 0.0);
  randomWaypoint[4] = make_tuple(2.5, -2.5, 0.0);
  randomWaypoint[5] = make_tuple(3.0, 0.0, 0.0);

  // Generate random waypoints
  for (int i = 6; i < 30; ++i) {
    string waypoint_name = "wp" + to_string(i);
    double x, y/*, theta*/;

    // Generate random coordinates with only 2 decimal places
    do {
      x = roundDecimal(dist(gen), 2);
      y = roundDecimal(dist(gen), 2);
      //theta = roundDecimal(dist(gen), 2);

      // Add all the way points to the forbidden coordinates
      randomWaypoint[i] = make_tuple(x, y, 0); // Add the waypoint to the vector of waypoints

    } while ((x >= 2.0 && y >= 2.0) || (x >= 2 && y <= -2) || (x <= -2.0 && y <= -2.0) 
              || (x <= -2 && y >= 2) || checkForbiddenWaypoint(x, y, forbiddenWaypoint));  // Check if the generated coordinates are forbidden

    // Write waypoint to file
    outfile << endl << waypoint_name << " [" << x << "," << y << "," << "0]";
  }

  connectWaypoints(randomWaypoint);

  outfile.close();
  cout << "Waypoints have been written to 'waypoints.txt'." << endl;
}

void VisitSolver::parseWaypoint(string waypoint_file){
  
  generateRandomWaypoints(); // generate random waypoints and write to file
  int curr, next;
  string line;
  double pose1, pose2, pose3;
  ifstream parametersFile(waypoint_file);

  if (parametersFile.is_open()){
    while (getline(parametersFile,line)){
      curr=line.find("[");

      string waypoint_name = line.substr(0,curr).c_str();

      curr=curr+1;
      next=line.find(",",curr);

      pose1 = (double)atof(line.substr(curr,next-curr).c_str());
      curr=next+1; next=line.find(",",curr);

      pose2 = (double)atof(line.substr(curr,next-curr).c_str());
      curr=next+1; next=line.find("]",curr);

      pose3 = (double)atof(line.substr(curr,next-curr).c_str());

      waypoint[waypoint_name] = vector<double> {pose1, pose2, pose3};
    }
  }
}