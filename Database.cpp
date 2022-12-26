#include <typeinfo>
#include <map>
#include <string.h>
#include "Database.h"

using namespace std;

Database::Database() {}

MYSQL* Database::connect() {
    MYSQL* connection = mysql_init(NULL); // mysql instance

    //connect database
    if(!mysql_real_connect(connection, server, user, password, database, 0, NULL, 0)){
        cout << "Connection Error: " << mysql_error(connection) << endl;
        exit(1);
    }

    return connection;
}

MYSQL_RES* Database::performQuery(MYSQL* connection, const char* query) {
    //send query to db
    if(mysql_query(connection, query)) {
        cout << "MySQL Query Error: " << mysql_error(connection) << endl;
        exit(1);
    }

    return mysql_use_result(connection);
}

void Database::addClient(MYSQL* connection, Client client) {
    char query[1024];
    if (clientExists(connection, client.getCPF()))
        throw runtime_error("Error: This CPF already has an account.");

    sprintf(query, "INSERT INTO Accounts VALUES (%i, '%s', '%s', %f, %f, 0, 0)", client.getCPF(), client.getName(), client.getPassword(), client.getCashInHands(), client.getSalary());
    performQuery(connection, query);
}

bool Database::clientExists(MYSQL* connection, int CPF) {
    char query[1024];
    sprintf(query, "SELECT CPF FROM Accounts WHERE CPF=%i;", CPF);
    MYSQL_RES* res = performQuery(connection, query);

    bool exists = (mysql_fetch_row(res) != NULL);
    mysql_free_result(res);

    return exists;
}

bool Database::correctPassword(MYSQL* connection, int CPF, const char* password) {
    char query[1024];
    sprintf(query, "SELECT password FROM Accounts WHERE CPF=%i;", CPF);
    MYSQL_RES* res = performQuery(connection, query);

    if (strcmp(mysql_fetch_row(res)[0], password) != 0)
        throw runtime_error("Error: Incorrect password.");

    mysql_free_result(res);

    return true;
}

map<string, char*> Database::getClientInfo(MYSQL* connection, int CPF) {
    MYSQL_ROW row;
    MYSQL_RES* res;
    char query[1024];
    map<string, char*> clientInfo;

    sprintf(query, "SELECT * FROM Accounts WHERE CPF=%i;", CPF);
    res = performQuery(connection, query);
    row = mysql_fetch_row(res);

    clientInfo["CPF"] = row[0];
    clientInfo["fullName"] = row[1];
    clientInfo["password"] = row[2];
    clientInfo["cashInHands"] = row[3];
    clientInfo["salary"] = row[4];
    clientInfo["balance"] = row[5];
    clientInfo["moneyOwed"] = row[6];

    mysql_free_result(res);

    return clientInfo;
}

void Database::modify(MYSQL* connection, int CPF, const char* collumn, float value) {
    char query[1024];
    sprintf(query, "UPDATE Accounts SET %s = %f WHERE CPF = %i;", collumn, value, CPF);
    performQuery(connection, query);
}

void Database::cashTransaction(MYSQL* connection, int CPF, float value) {
    MYSQL_RES* res;
    float inHands;
    float balance;
    char query[1024];

    sprintf(query, "SELECT cashInHands FROM Accounts WHERE CPF=%i;", CPF);
    res = performQuery(connection, query);
    inHands = atof(mysql_fetch_row(res)[0]);
    mysql_free_result(res);

    if (value > inHands)
        throw runtime_error("Error: You must have in hands the amount of money you are trying to deposit.");

    inHands -= value;

    sprintf(query, "SELECT balance FROM Accounts WHERE CPF=%i;", CPF);
    res = performQuery(connection, query);
    balance = atof(mysql_fetch_row(res)[0]);

    if (value < balance * -1)
        throw runtime_error("Error: Cannot withdraw more money than you have in your account.");

    balance += value;
    mysql_free_result(res);

    modify(connection, CPF, "balance", balance);
    modify(connection, CPF, "cashInHands", inHands);
}

void Database::pix(MYSQL* connection, int from, int to, float value) {
    MYSQL_RES* res;
    float balance;
    char query[1024];

    if (!clientExists(connection, to))
        throw runtime_error("Error: CPF youre trying to send money to doesnt have an account.");

    sprintf(query, "SELECT balance FROM Accounts WHERE CPF=%i;", from);
    res = performQuery(connection, query);
    balance = atof(mysql_fetch_row(res)[0]);

    if (value > balance)
        throw runtime_error("Error: Cannot send more money than you have in your account.");

    balance -= value;
    mysql_free_result(res);
    modify(connection, from, "balance", balance);

    sprintf(query, "SELECT balance FROM Accounts WHERE CPF=%i;", to);
    res = performQuery(connection, query);
    balance = atof(mysql_fetch_row(res)[0]);
    balance += value;
    mysql_free_result(res);

    modify(connection, to, "balance", balance);
}