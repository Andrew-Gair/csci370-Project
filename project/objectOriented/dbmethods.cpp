#include <iostream>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <occi.h>
#include "dbmethods.h"
using namespace std;

dbmethods::dbmethods() //constructor
{
}
dbmethods::~dbmethods() //deconstuctor
{
}

dbmethods::executeQuery_TrainStaff() {

    Statement *stmt = conn->createStatement(trainStaffQ);
    ResultSet *rs = stmt->executeQuery();
    cout << "+--------+-----------------+-----------------+------------+-----------+" << endl;
    cout << "| EmpNum |      Lname      |      Fname      |  Position  |  trainID  |" << endl;
    cout << "+--------+-----------------+-----------------+------------+-----------+" << endl;
    while (rs->next()) {  //output the results of the query to the file
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