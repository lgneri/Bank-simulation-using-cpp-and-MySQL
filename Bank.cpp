#include "Bank.h"

using namespace std;

Bank::Bank() {
    connection = db.connect();
}

Bank::~Bank() {
    mysql_close(connection);
}

void Bank::createAccount(Client client) {
    db.addClient(connection, client);
}

bool Bank::login(int CPF, const char* password) {
    if (!db.clientExists(connection, CPF)) {
        throw runtime_error("Error: There is no account registered to this CPF.");
    }

    return db.correctPassword(connection, CPF, password);
}

void Bank::checkBalance(int CPF) {
    map<string, char*> clientInfo = db.getClientInfo(connection, CPF);

    cout << "\nYour account:" << endl
         << "Balance: " << clientInfo["balance"] << endl
         << "Money owed: " << clientInfo["moneyOwed"] << endl;
}

void Bank::deposit(int CPF, float amount) {
    db.cashTransaction(connection, CPF, amount);
}

void Bank::withdraw(int CPF, float amount) {
    amount = amount * -1;
    db.cashTransaction(connection, CPF, amount);
}

void Bank::pix(int from, int to, float amount) {
    db.pix(connection, from, to, amount);
}

