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
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include "armadillo"
#include <initializer_list>

using namespace std;
using namespace arma;



map <string, vector<double> > region_mapping;

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

  char const *x[]={"dummy"};
  char const *y[]={"act-cost","triggered"};
  parseParameters(Paramers);
  affected = list<string>(x,x+1);
  dependencies = list<string>(y,y+2);

  string waypoint_file = "visits_domain/waypoint.txt";   // change this to the correct path
  parseWaypoint(waypoint_file);

  string landmark_file = "visits_domain/landmark.txt";  // change this to the correct path
  parseLandmark(landmark_file);


        //startEKF();
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
      if(function=="triggered"){
        trigger[arg] = value>0?1:0;
        if (value>0){

      string from = tmp.substr(0,2);   // from and to are regions, need to extract wps (poses)
      string to = tmp.substr(3,2);


     // distance_euc(from, to);

    }
  }
}else{
  if(function=="dummy"){
    dummy = value;

  }else if(function=="act-cost"){
    act_cost = value;
                 } //else if(function=="dummy1"){
                    //duy = value;              
                    ////cout << parameter << " " << value << endl;
                 //}
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
       //float random1 = static_cast <float> (rand())/static_cast <float>(RAND_MAX);
       double cost = 2;//random1;
       return cost;
     }

bool checkForbiddenCoordinates(double x, double y, /*double theta,*/ const vector<tuple<double, double, double>>& forbiddenCoordinates) {
  for (const auto& coord : forbiddenCoordinates) {
    if (x == get<0>(coord) && y == get<1>(coord) /*&& theta == get<2>(coord)*/) {
      return true;  // The coordinates are forbidden
    }
  }
  return false;  // The coordinates are allowed
}

double roundDecimal(double value, int decimalPlaces) {
  double multiplier = pow(10.0, decimalPlaces);
  return round(value * multiplier) / multiplier;
}

void generateRandomWaypoints()
{
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<double> dist(0.0, 6.0);  // Assuming the environment dimensions are 6m x 6m

  ofstream outfile("../visits_domain/waypoint.txt");
  if (!outfile.is_open()) {
    cerr << "Unable to open file for writing." << endl;
    return;
  }

  vector<tuple<double, double, double>> forbiddenCoordinates = { {0, 0, 0}, {-2.5, 2.5, 0}, {2.5, 2.5, 0}, {-2.5, -2.5, 0}, {2.5, -2.5, 0}, {3, 0, 0} };  // Specify the forbidden coordinates here

  // set the default waypoints (the ones that are forbidden)
  outfile << "wp0" << " [" << "0" << "," << "0" << "," << "0]";                 // The zero waypoint is the starting position of the robot       
  outfile << endl << "wp1" << " [" << "-2.5" << "," << "2.5" << "," << "0]";    // The first waypoint is the first group table
  outfile << endl << "wp2" << " [" << "2.5" << "," << "2.5" << "," << "0]";     // The second waypoint is the second group table
  outfile << endl << "wp3" << " [" << "-2.5" << "," << "-2.5" << "," << "0]";   // The third waypoint is the third group table
  outfile << endl << "wp4" << " [" << "2.5" << "," << "-2.5" << "," << "0]";    // The fourth waypoint is the fourth group table
  outfile << endl << "wp5" << " [" << "3" << "," << "0" << "," << "0]";         // The fifth waypoint is the delivery table

  for (int i = 6; i < 30; ++i) {
    string waypoint_name = "wp" + to_string(i);
    double x, y/*, theta*/;

    // Generate random coordinates with only 2 decimal places
    do {
      x = roundDecimal(dist(gen), 2);
      y = roundDecimal(dist(gen), 2);
      //theta = roundDecimal(dist(gen), 2);
    } while (checkForbiddenCoordinates(x, y, /*theta,*/ forbiddenCoordinates));  // Check if the generated coordinates are forbidden

    // Write waypoint to file
    outfile << endl << waypoint_name << " [" << x << "," << y << "," << "0]";
  }

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

   void VisitSolver::parseLandmark(string landmark_file){

     int curr, next;
     string line;
     double pose1, pose2, pose3;
     ifstream parametersFile(landmark_file);
     if (parametersFile.is_open()){
      while (getline(parametersFile,line)){
       curr=line.find("[");
       string landmark_name = line.substr(0,curr).c_str();
       
       curr=curr+1;
       next=line.find(",",curr);

       pose1 = (double)atof(line.substr(curr,next-curr).c_str());
       curr=next+1; next=line.find(",",curr);

       pose2 = (double)atof(line.substr(curr,next-curr).c_str());
       curr=next+1; next=line.find("]",curr);

       pose3 = (double)atof(line.substr(curr,next-curr).c_str());

       landmark[landmark_name] = vector<double> {pose1, pose2, pose3};
     }
   }
   
 }


  //void VisitSolver::distance_euc( string from, string to){
  //} 







