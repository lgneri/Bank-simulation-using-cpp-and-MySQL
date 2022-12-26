#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>

using namespace std;

class Client
{
    private:
        const char* name;
        const char* password;
        int CPF;
        float cashInHands;
        float salary;
    public:
        Client(int, const char*, const char*, float, float);
        const char* getName();
        const char* getPassword();
        int getCPF();
        float getCashInHands();
        float getSalary();
};

#endif