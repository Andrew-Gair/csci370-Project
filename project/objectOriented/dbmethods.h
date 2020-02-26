#ifndef DBMETHODS_H
#define DBMETHODS_H
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <occi.h>
using namespace std;

class dbmethods {
private:

public:
    dbmethods();
    ~dbmethods();
    executeQuery(string query);
//---------------------------------//
//--CREATE A BUNCH OF THE QUERIES--//
//---------------------------------//
    string trainStaffQ;
    trainStaffQ = trainStaffQ + "select *"
                  + "  from trainStaff";
    string stationStaffQ;
    stationStaffQ = stationStaffQ + "select *"
                    + "from stationStaff";
    string stopsQ;
    stopsQ = stopsQ + "select stationID, arrivalTime, departureTime "
             + "from stopsAt "
             + "where trainID = '"; //a trainID will be added later when user inputs it
    string trainsQ;
    trainsQ = trainsQ + "select *"
              + "from trains";
    string routesThroughQ;
    routesThroughQ = routesThroughQ + "select *"
                     + "from routesThrough";

};



#endif