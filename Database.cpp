/** Arquivo Database.cpp. Trabalho final EEL670 2022.2
*   Aluno: Luis Guilherme Neri Ferreira */

#include <map>
#include <vector>
#include <string.h>
#include "Database.h"

using namespace std;

Database::Database() {}

MYSQL* Database::connect() {
    MYSQL* connection = mysql_init(NULL);

    if(!mysql_real_connect(connection, server, user, password, database, 0, NULL, 0)){
        cout << "Connection Error: " << mysql_error(connection) << endl;
        exit(1);
    }

    return connection;
}

MYSQL_RES* Database::performQuery(MYSQL* connection, const char* query) {
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

    if (strcmp(mysql_fetch_row(res)[0], password) != 0) {
        mysql_free_result(res);
        throw runtime_error("Error: Incorrect password.");
    }

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
    map<string, char*> clientInfo;
    float cashInHands;
    float balance;

    clientInfo = getClientInfo(connection, CPF);
    cashInHands = atof(clientInfo["cashInHands"]);
    balance = atof(clientInfo["balance"]);

    if (value > cashInHands)
        throw runtime_error("Error: You must have in hands the amount of money you are trying to deposit.");

    if (value < balance * -1)
        throw runtime_error("Error: Cannot withdraw more money than you have in your account.");

    cashInHands -= value;
    balance += value;

    modify(connection, CPF, "balance", balance);
    modify(connection, CPF, "cashInHands", cashInHands);
}

void Database::pix(MYSQL* connection, int from, int to, float value) {
    map<string, char*> clientInfo;
    float balance;

    if (!clientExists(connection, to))
        throw runtime_error("Error: CPF youre trying to send money to doesnt have an account.");

    clientInfo = getClientInfo(connection, from);
    balance = atof(clientInfo["balance"]);

    if (value > balance)
        throw runtime_error("Error: Cannot send more money than you have in your account.");

    balance -= value;
    modify(connection, from, "balance", balance);

    clientInfo = getClientInfo(connection, to);
    balance = atof(clientInfo["balance"]);
    balance += value;
    modify(connection, to, "balance", balance);
}

void Database::loan(MYSQL* connection, int CPF, float value) {
    map<string, char*> clientInfo;
    float moneyOwed;
    float balance;

    clientInfo = getClientInfo(connection, CPF);
    moneyOwed = atof(clientInfo["moneyOwed"]);
    balance = atof(clientInfo["balance"]);

    moneyOwed += value;
    balance += value;

    modify(connection, CPF, "balance", balance);
    modify(connection, CPF, "moneyOwed", moneyOwed);
}

void Database::payLoan(MYSQL* connection, int CPF) {
    map<string, char*> clientInfo;
    float moneyOwed;
    float balance;

    clientInfo = getClientInfo(connection, CPF);
    moneyOwed = atof(clientInfo["moneyOwed"]);
    balance = atof(clientInfo["balance"]);

    if (moneyOwed > balance)
        throw runtime_error("Error: You do not have enough money in your account to pay your loan yet.");

    balance -= moneyOwed;
    moneyOwed = 0;

    modify(connection, CPF, "balance", balance);
    modify(connection, CPF, "moneyOwed", moneyOwed);
}

void Database::simulateMonth(MYSQL* connection) {
    MYSQL_ROW row;
    MYSQL_RES* res;
    vector<int> CPFs;

    res = performQuery(connection, "SELECT CPF FROM Accounts;");

    while ((row = mysql_fetch_row(res)) != NULL) {
        CPFs.push_back(atoi(row[0]));
    }

    mysql_free_result(res);

    for (int CPF: CPFs) {
        map<string, char*> clientInfo = getClientInfo(connection, CPF);

        float cashInHands = atof(clientInfo["cashInHands"]);
        float moneyOwed = atof(clientInfo["moneyOwed"]);
        float salary = atof(clientInfo["salary"]);

        cashInHands = cashInHands + salary;
        moneyOwed = moneyOwed * interest;

        modify(connection, CPF, "cashInHands", cashInHands);
        modify(connection, CPF, "moneyOwed", moneyOwed);
    }
}
