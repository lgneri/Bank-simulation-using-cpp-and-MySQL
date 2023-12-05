/** Arquivo Database.h. Trabalho final EEL670 2022.2
*   Aluno: Luis Guilherme Neri Ferreira */

#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <cstdio>
#include <mysql/mysql.h>
#include "Client.h"

using namespace std;

class Database
{
    private:
        const char* server = "localhost";
        const char* user = "lgneri";
        const char* password = "senhaPublica";
        const char* database = "neriBank";
        const float interest = 1.02;
        MYSQL_RES* performQuery(MYSQL*, const char*);
        void modify(MYSQL*, int, const char*, float);
    public:
        MYSQL* connect();
        Database();
        void addClient(MYSQL*, Client);
        bool clientExists(MYSQL*, int);
        bool correctPassword(MYSQL*, int, const char*);
        map<string, char*> getClientInfo(MYSQL*, int);
        void cashTransaction(MYSQL*, int, float);
        void pix(MYSQL*, int, int, float);
        void loan(MYSQL*, int, float);
        void payLoan(MYSQL*, int);
        void simulateMonth(MYSQL*);
};

#endif
