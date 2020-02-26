#include <iostream>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <occi.h>
#include "dbmethods.h"

//ANDREW GAIR (GAIRAD) MARCH 26 2019

using namespace std;
using namespace oracle::occi;

// read database password from user input
// without showing the password on the screen
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


//-----------------------------------//
//--OPEN CONNECTION TO THE DATABASE--//
//-----------------------------------//
  try{
    //cout << "entered try block at least" << endl;
    Environment *env = Environment::createEnvironment();
    //cout << "made environment" << endl;
    Connection *conn = env->createConnection(userName, password, connectString);
    //cout << "made connection" << endl;

    cout << "Welcome to Andrew's Railway Management System" << endl;

    string userInput;
    string selectInput;

    while(userInput != "q") {
      cout << "Options are:"
           << endl << "'Q' to end the program"
           << endl << "'select' to open selection menu" << endl;

      cout << endl << "Please enter a cmd:   ";
      getline(cin, userInput);

      //--------//
      //--QUIT--//
      if (userInput == "q" || userInput == "Q") {
        cout << "Goodbye!" << endl;
        userInput = "q";
      }

        //----------------//
        //--SELECT STUFF--//
      else if (userInput == "select" || userInput == "Select") {

        //---------------------//
        //--ENTER SELECT LOOP--//
        while(selectInput != "q"){
          cout << "please specify what to select from:" << endl
               << "trainStaff, stationStaff, trains, stops [trainID required], routesThrough, or \'Q\' to quit" << endl;
          getline(cin, selectInput);

          //--------------------//
          //--QUIT FROM SELECT--//
          if (selectInput == "q" || selectInput == "Q") {
            cout << "Exiting Select Menu" << endl;
            selectInput = "q";
          }
          //---------------------//
          //--SELECT TRAINSTAFF--//
          else if (selectInput == "trainStaff") {
            dbmethods.executeQuery_TrainStaff;
          }
            //-----------------------//
            //--SELECT STATIONSTAFF--//
          else if (selectInput == "stationStaff") {

            Statement *stmt = conn->createStatement(dbmethods.stationStaffQ);
            //cout << "made statement for stationStaff" << endl;
            ResultSet *rs = stmt->executeQuery();
            //cout << "made result Set for stationStaff" << endl;
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
          //cout << "closed result set" << endl;      //debugging message to see where program gets to
          conn->terminateStatement(stmt);
          //cout << "closed statement" << endl;       //debugging message to see where program gets to
        }
          //---------------------------------//
          //--SELECT STOPS FROM GIVEN TRAIN--//
        else if (selectInput == "stops") {
          string trainSearch;
          cout << "please enter a valid trainID to look for:" << endl;
          getline(cin, trainSearch);
          string stopsAtQ = dbmethods.stopsQ + trainSearch + "'";
          Statement *stmt = conn->createStatement(stopsAtQ);
          //cout << "made statement for stopsAt" << endl;
          ResultSet *rs = stmt->executeQuery();
          //cout << "made result Set for stopsAt" << endl;
          cout << "+-----------+-------------+----------------+" << endl;
          cout << "| StationID | arrivalTime |  departureTime |" << endl;
          cout << "+-----------+-------------+----------------+" << endl;
          while (rs->next()) {          //output the results of the query to the file
            cout << "| " << setw(9) << rs->getString(1) << " " //stationID
                 << "| " << setw(11) << rs->getString(2) << " " //arrivalTime
                 << "| " << setw(14) << rs->getString(3) << " |" << endl; //departureTime
          }
          cout << "+-----------+-------------+----------------+" << endl;
          stmt->closeResultSet(rs);
          //cout << "closed result set" << endl;      //debugging message to see where program gets to
          conn->terminateStatement(stmt);
          //cout << "closed statement" << endl;       //debugging message to see where program gets to
        }
        else if (selectInput == "trains") {
          string trainSearch;
          cout << "List of all trains currently on the rails:" << endl;

          Statement *stmt = conn->createStatement(dbmethods.trainsQ);
          //cout << "made statement for trainsQ" << endl;
          ResultSet *rs = stmt->executeQuery();
          //cout << "made result Set for trainsQ" << endl;
          cout << "+---------+----------------------+" << endl;
          cout << "| trainID |   currentLocation    |" << endl;
          cout << "+---------+----------------------+" << endl;
          while (rs->next()) {          //output the results of the query to the file
            cout << "| " << setw(7) << rs->getString(1) << " " //trainID
                 << "| " << setw(20) << rs->getString(2) << " |" << endl; //currentLocation
          }
          cout << "+---------+----------------------+" << endl;
          stmt->closeResultSet(rs);
          //cout << "closed result set" << endl;      //debugging message to see where program gets to
          conn->terminateStatement(stmt);
          //cout << "closed statement" << endl;       //debugging message to see where program gets to
        }
        else if (selectInput == "routesThrough") {

            Statement *stmt = conn->createStatement(dbmethods.routesThroughQ);
            //cout << "made statement for routesThroughQ" << endl;
            ResultSet *rs = stmt->executeQuery();
            //cout << "made result Set for routesThroughQ" << endl;
            cout << "+-----------+---------+-------------+---------------+" << endl;
            cout << "| stationID | cargoID | arrivalTime | departureTime |" << endl;
            cout << "+-----------+---------+-------------+---------------+" << endl;
            while (rs->next()) {          //output the results of the query to the file
              cout << "| " << setw(9) << rs->getString(1) << " " //stationID
                   << "| " << setw(7) << rs->getString(2) << " " //cargoID
                   << "| " << setw(11) << rs->getString(3) << " " //arrivalTime
                   << "| " << setw(13) << rs->getString(4) << " |" << endl; //departureTime
            }
            cout << "+-----------+---------+-------------+---------------+" << endl;
            stmt->closeResultSet(rs);
            //cout << "closed result set" << endl;      //debugging message to see where program gets to
            conn->terminateStatement(stmt);
            //cout << "closed statement" << endl;       //debugging message to see where program gets to
          }
        }
      }
    }
  env->terminateConnection(conn);
  //cout << "closed connection" << endl;      //debugging message to see where program gets to
  Environment::terminateEnvironment(env);
  //cout << "closed environment" << endl;     //debugging message to see where program gets to

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
