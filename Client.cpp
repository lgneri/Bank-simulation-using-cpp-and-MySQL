#include "Client.h"

using namespace std;

Client::Client(int CPF, const char* name, const char* password, float cashInHands, float salary) {
    this->CPF = CPF;
    this->name = name;
    this->password = password;
    this->cashInHands = cashInHands;
    this->salary = salary;
}

const char* Client::getName() {
    return name;
}

int Client::getCPF() {
    return CPF;
}

const char* Client::getPassword() {
    return password;
}

float Client::getCashInHands() {
    return cashInHands;
}

float Client::getSalary() {
    return salary;
}