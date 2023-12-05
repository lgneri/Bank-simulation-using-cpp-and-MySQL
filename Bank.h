/** Arquivo Bank.h. Trabalho final EEL670 2022.2
*   Aluno: Luis Guilherme Neri Ferreira */

#ifndef BANK_H
#define BANK_H

#include <iostream>
#include <map>
#include "Database.h"
#include "Client.h"

using namespace std;

class Bank
{
    private:
        MYSQL* connection;
        Database db;
    public:
        Bank();
        ~Bank();
        void createAccount(Client);
        bool login(int, const char*);
        void checkBalance(int);
        void deposit(int, float);
        void withdraw(int, float);
        void pix(int, int, float);
        void loan(int, float);
        void payLoan(int);
        void simulateMonth();
};

#endif
