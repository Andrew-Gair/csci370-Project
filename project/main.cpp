#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cctype>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <occi.h>


//ANDREW GAIR (GAIRAD) MARCH 26 2019

using namespace std;
using namespace oracle::occi;

// read database password from user input
// without showing the password on the screen
// PRE-BUILT FUNCTION FROM LAB
string readPassword();


int main(){
//-----------------------------------//
//--GET LOGIN DETAILS FROM THE USER--//
//-----------------------------------//
  cout << "Credientials for Database login" << endl;
  cout << "Username:" << endl;
  string userName;
  getline(cin, userName);

  cout << "Password:" << endl;
  string password;
  password = readPassword();
  const string connectString = "sunfire.csci.viu.ca";

//---------------------//
//--CREATE STATEMENTS--//
//---------------------//
  string trainStaffQ;
  trainStaffQ = trainStaffQ + "select *"
                + "  from trainStaff";
  string stationStaffQ;
  stationStaffQ = stationStaffQ + "select *"
                  + "from stationStaff";
  string stopsQ;
  stopsQ = stopsQ + "select stationID, stationName, arrivalTime, departureTime "
           + "from stopsAt natural join trains natural join stations "
           + "where trainID = '";
  string trainsQ;
  trainsQ = trainsQ + "select *"
            + "from trains";
  string trafficQ;
  trafficQ = trafficQ + "select trainID, arrivalTime, departureTime "
                  + "from stations natural join stopsAt "
                  + "where stationID = '";
  string stationsQ;
  stationsQ = stationsQ + "select *"
              + "from stations";
  string routesThroughQ;
  routesThroughQ = routesThroughQ + "select *"
                   + "from routesThrough";
  string cargoQ;
  cargoQ = cargoQ + "select *"
           + "from cargo";
  string cargoStopsQ;
  cargoStopsQ =  cargoStopsQ + " select stationName, arrivalTime, departureTime "
                  + "from cargo natural join routesThrough natural join stations "
                  + "where cargoID = '";
                  
//-----------------------------------//
//--OPEN CONNECTION TO THE DATABASE--//
//-----------------------------------//
  try{
    Environment *env = Environment::createEnvironment();
    Connection *conn = env->createConnection(userName, password, connectString);

    cout << "Welcome to Andrew's Railway Management System" << endl;

    string userInput;
    string selectInput;

//-- Begin the 'main menu' loop
    while(userInput != "quit") {
      userInput = "";
      selectInput = "";
      cout << "Options are:"
           << endl << "\'quit\' to end the program"
           << endl << "\'select\' to open selection menu" << endl;

      cout << endl << "Please enter a cmd:   ";
      getline(cin, userInput);


      //--------//
      //--QUIT--//
      if (userInput == "quit" || userInput == "Quit") {
        cout << "Goodbye!" << endl;
        userInput = "quit";
      }

      //----------------//
      //--SELECT STUFF--//
      else if (userInput == "select" || userInput == "Select") {

        //------------------------------------//
        //--LOOP UNTIL USER REQUESTS TO STOP--//
        while(selectInput != "back") {
          cout << "please specify what to select from:" << endl
               << "\'staff\' to see station or train staff" << endl
               << "\'stops\' to see stops a train makes throughout the day" << endl
               << "\'routes\' to see cargo as it travels" << endl
               << "\'traffic\' to see which trains stop at a station" << endl
               << "\'back\' to leave select menu" << endl;
          cout << endl << "Please enter a cmd:   ";
          getline(cin, selectInput);

          if(selectInput == "back" || selectInput == "Back"){
            cout << "leaving select menu" << endl;
            selectInput = "back";
          }

          if(selectInput == "staff" || selectInput == "Staff") {
              cout << endl << endl << "please enter one of the following to select which staff will be shown:" << endl
                   << "\'train\'" << endl << "\'station\'" << endl << "\'back\'" << endl;
              cout << endl << "Please enter a cmd:   ";
              getline(cin, selectInput);
              //--SELECT TRAINSTAFF--//
              if (selectInput == "train" || selectInput == "Train") {
                  Statement *stmt = conn->createStatement(trainStaffQ);
                  ResultSet *rs = stmt->executeQuery();
                  cout << "+--------+-----------------+-----------------+------------+-----------+" << endl;
                  cout << "| EmpNum |      Lname      |      Fname      |  Position  |  trainID  |" << endl;
                  cout << "+--------+-----------------+-----------------+------------+-----------+" << endl;
                  while (rs->next()) {          //output the results of the query to the file
                      cout << "| " << setw(6) << rs->getString(1) << " "    //empNum
                           << "| " << setw(15) << rs->getString(2) << " "  //Lname
                           << "| " << setw(15) << rs->getString(3) << " "  //Fname
                           << "| " << setw(10) << rs->getString(4) << " "  //Position
                           << "| " << setw(9) << rs->getString(5) << " |" << endl; //TrainID
                  }
                  cout << "+--------+-----------------+-----------------+------------+-----------+" << endl;
                  stmt->closeResultSet(rs);
                  conn->terminateStatement(stmt);
              }
              //--SELECT STATIONSTAFF--//
              else if (selectInput == "station" || selectInput == "Station") {

                  Statement *stmt = conn->createStatement(stationStaffQ);
                  ResultSet *rs = stmt->executeQuery();
                  cout << "+--------+-----------------+-----------------+------------+-----------+----------------------+"
                          << endl;
                  cout << "| EmpNum |      Lname      |      Fname      |  Position  | StationID |      StationName     |"
                          << endl;
                  cout << "+--------+-----------------+-----------------+------------+-----------+----------------------+"
                          << endl;
                  while (rs->next()) {          //output the results of the query to the file
                      cout << "| " << setw(6) << rs->getString(1) << " " //empNum
                           << "| " << setw(15) << rs->getString(2) << " " //Lname
                           << "| " << setw(15) << rs->getString(3) << " " //Fname
                           << "| " << setw(10) << rs->getString(4) << " " //Position
                           << "| " << setw(9) << rs->getString(5) << " " //stationID
                           << "| " << setw(20) << rs->getString(6) << " |" << endl; //stationName
                  }
                  cout << "+--------+-----------------+-----------------+------------+-----------+----------------------+"
                          << endl;
                  stmt->closeResultSet(rs);
                  conn->terminateStatement(stmt);
              }
          }
          //---------------------------------//
          //--SELECT STOPS FROM GIVEN TRAIN--//
          else if (selectInput == "stops") {
            //-- List all trains on the rails so user can select one to view --//
            string trainSearch = "";
            cout << endl << endl << "List of all trains currently on the rails:" << endl;
            Statement *stmt = conn->createStatement(trainsQ);
            ResultSet *rs = stmt->executeQuery();
            cout << "+---------+----------------------+" << endl;
            cout << "| trainID |   currentLocation    |" << endl;
            cout << "+---------+----------------------+" << endl;
            while (rs->next()) {          //output the results of the query to the file
              cout << "| " << setw(7) << rs->getString(1) << " " //trainID
                   << "| " << setw(20) << rs->getString(2) << " |" << endl; //currentLocation
            }
            cout << "+---------+----------------------+" << endl;
            stmt->closeResultSet(rs);
            conn->terminateStatement(stmt);

            //-- get the user to select one the available trains --//
            cout << "please enter a valid trainID to look for, or enter \'back\' to go back to select menu:" << endl;
            cout << endl << "Please enter a cmd:   ";
            getline(cin, trainSearch);
            if (trainSearch == "back"|| trainSearch == "Back") {
              cout << "back to select menu" << endl;
            }
            //--check that the user entered value can be converted cleanly to an integer
            //--and then check that the value is only 6 digits long (the trainID is 6 digits)
            else if((atoi(trainSearch.c_str())) && (trainSearch.length() == 6)){
              string stopsAtQ = stopsQ + trainSearch + "'" + "order by arrivalTime"; //finalize the stopsAt query with user input
              Statement *stmt = conn->createStatement(stopsAtQ);
              ResultSet *rs = stmt->executeQuery();
              cout << endl << "The train with ID " << trainSearch << " stops at the following stations:" << endl;
              cout << "+-----------+----------------------+-------------+----------------+" << endl;
              cout << "| StationID |      StationName     | arrivalTime |  departureTime |" << endl;
              cout << "+-----------+----------------------+-------------+----------------+" << endl;
              while (rs->next()) {          //output the results of the query to the file
                cout << "| " << setw(9) << rs->getString(1) << " " //stationID
                     << "| " << setw(20) << rs->getString(2) << " " //stationName
                     << "| " << setw(11) << rs->getString(3) << " " //arrivalTime
                     << "| " << setw(14) << rs->getString(4) << " |" << endl; //departureTime
              }
              cout << "+-----------+----------------------+-------------+----------------+" << endl;
              stmt->closeResultSet(rs);
              conn->terminateStatement(stmt);
            }
            else{
                cout << endl << endl << "----------------------------------------" << endl;
                cout << "That input didn't work, please try again" << endl;
                cout << "----------------------------------------" << endl;            }
          }

          else if (selectInput == "traffic") {
              //-- List all trains that stop at the station --//
              string stationSearch = "";
              Statement *stmt = conn->createStatement(stationsQ);
              ResultSet *rs = stmt->executeQuery();
              cout << endl << endl << "List of all stations on this rail network:" << endl;
              cout << "+-----------+----------------------+" << endl;
              cout << "| StationID |      StationName     |" << endl;
              cout << "+-----------+----------------------+" << endl;
              while (rs->next()) {          //output the results of the query to the file
                  cout << "| " << setw(9) << rs->getString(1) << " " //stationID
                       << "| " << setw(20) << rs->getString(2) << " |" << endl; //stationName
              }
              cout << "+-----------+----------------------+" << endl;
              stmt->closeResultSet(rs);
              conn->terminateStatement(stmt);

              //-- get the user to select one the available trains --//
              cout << "please enter a valid stationID to look for, or enter \'back\' to go back to select menu:" << endl;
              cout << endl << "Please enter a cmd:   ";
              getline(cin, stationSearch);
              if (stationSearch == "back"|| stationSearch == "Back") {
                  cout << "back to select menu" << endl;
              }
              //--check that the user entered value can be converted cleanly to an integer
              //--and then check that the value is only 6 digits long (as stationID is also 6 digits)
              else if((atoi(stationSearch.c_str())) && (stationSearch.length() == 6)){
                  string trafficStopsQ = trafficQ + stationSearch + "'" + "order by arrivalTime"; //finalize the stopsAt query with user input
                  Statement *stmt = conn->createStatement(trafficStopsQ);
                  ResultSet *rs = stmt->executeQuery();
                  cout << endl << endl << "The station with ID " << stationSearch
                       << " has the following trains stop at it:" << endl;
                  cout << "+---------+-------------+----------------+" << endl;
                  cout << "| trainID | arrivalTime |  departureTime |" << endl;
                  cout << "+---------+-------------+----------------+" << endl;
                  while (rs->next()) {          //output the results of the query to the file
                      cout << "| " << setw(7) << rs->getString(1) << " " //stationID
                           << "| " << setw(11) << rs->getString(2) << " " //stationName
                           << "| " << setw(14) << rs->getString(3) << " |" << endl; //arrivalTime
                  }
                  cout << "+---------+-------------+----------------+" << endl;
                  stmt->closeResultSet(rs);
                  conn->terminateStatement(stmt);
              }
              else{
                  cout << endl << endl << "----------------------------------------" << endl;
                  cout << "That input didn't work, please try again" << endl;
                  cout << "----------------------------------------" << endl;
              }
          }

          //--See cargo details--//
          else if (selectInput == "routes") {
            string cargoSearch = "";
            Statement *stmt = conn->createStatement(cargoQ);
            ResultSet *rs = stmt->executeQuery();
            cout << "+---------+--------------+--------------+---------+" << endl;
            cout << "| cargoID |     Type     | destination  | trainID |" << endl;
            cout << "+---------+--------------+--------------+---------+" << endl;
            while (rs->next()) {          //output the results of the query to the file
              cout << "| " << setw(7) << rs->getString(1) << " " //cargoID
                   << "| " << setw(12) << rs->getString(2) << " " //type
                   << "| " << setw(12) << rs->getString(3) << " " //destination
                   << "| " << setw(7) << rs->getString(4) << " |" << endl; //trainID
            }
            cout << "+---------+--------------+--------------+---------+" << endl;
            stmt->closeResultSet(rs);
            conn->terminateStatement(stmt);

            cout << "please enter a valid cargoID to look for, or enter \'back\' to go back to select menu:" << endl;
            cout << endl << "Please enter a cmd:   ";
            getline(cin, cargoSearch);
            if (cargoSearch == "back"|| cargoSearch == "Back") {
                cout << "back to select menu" << endl;
            }
            //--check that the user entered value can be converted cleanly to an integer
            //--and check that the user value is 6 digits long (cargoID is also 6 digits long).
            else if((atoi(cargoSearch.c_str())) && (cargoSearch.length() == 6)){
                //do error checking on stationSearch here, check to ensure stationID is from the table
                string cargoDetailsQ = cargoStopsQ + cargoSearch + "'" + "order by arrivalTime"; //finalize the stopsAt query with user input
                Statement *stmt = conn->createStatement(cargoDetailsQ);
                ResultSet *rs = stmt->executeQuery();
                cout << endl << endl << "The cargo with ID " << cargoSearch
                     << " has the following stops scheduled:" << endl;
                cout << "+----------------------+-------------+----------------+" << endl;
                cout << "|      stationName     | arrivalTime |  departureTime |" << endl;
                cout << "+----------------------+-------------+----------------+" << endl;
                while (rs->next()) {          //output the results of the query to the file
                    cout << "| " << setw(20) << rs->getString(1) << " " //stationName
                         << "| " << setw(11) << rs->getString(2) << " " //arrivalTime
                         << "| " << setw(14) << rs->getString(3) << " |" << endl; //departureTime
                }
                cout << "+----------------------+-------------+----------------+" << endl;
                stmt->closeResultSet(rs);
                conn->terminateStatement(stmt);
             }
            else{
                cout << endl << endl << "----------------------------------------" << endl;
                cout << "That input didn't work, please try again" << endl;
                cout << "----------------------------------------" << endl;            }
          }

        }
      }
    }
  env->terminateConnection(conn);
  Environment::terminateEnvironment(env);
  }
  
//---------------------------------//
//--IF THE CONNECTION WAS REFUSED--//
//---------------------------------//
  catch(SQLException & e) {
    cout << e.what() << endl; //detailed error message
    return 0;
  }
  
  return 0; //end of main
}


//-------------------------//
//--FUNCTION DECLARATIONS--//
//-------------------------//
string readPassword()
{
    struct termios settings;
    tcgetattr( STDIN_FILENO, &settings );
    settings.c_lflag =  (settings.c_lflag & ~(ECHO));
    tcsetattr( STDIN_FILENO, TCSANOW, &settings );

    string password = "";
    getline(cin, password);

    settings.c_lflag = (settings.c_lflag |   ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &settings );
    return password;
}