/** Arquivo main.cpp. Trabalho final EEL670 2022.2
*   Aluno: Luis Guilherme Neri Ferreira */

#include <string>
#include <stdlib.h>
#include "Bank.h"
#include "Client.h"
#include "Database.h"

int main()
{
    Bank bank;

    while (true) {
        cout << "\n------------------------------------NERIBANK--------------------------------------"
             << "\n\n1: Register"
             << "\n2: Login"
             << "\n3: Simulate 1 month"
             << "\n4: Exit bank"
             << endl;
        int choice;
        cin >> choice;
        int CPF;
        string password;

        try {
            if (choice == 1) {
                string name;
                float cashInHands, salary;

                cout << "Insert your CPF: ";
                cin >> CPF;
                cout << "Insert your name: ";
                cin >> name;
                cout << "Insert your password: ";
                cin >> password;
                cout << "Insert your patrimony: ";
                cin >> cashInHands;
                cout << "Insert your salary: ";
                cin >> salary;

                bank.createAccount(Client(CPF, name.c_str(), password.c_str(), cashInHands, salary));
                cout << "Account registered successfully!" << endl;
            }
            else if (choice == 2) {
                cout << "CPF: ";
                cin >> CPF;
                cout << "password: ";
                cin >> password;

                if (bank.login(CPF, password.c_str())) {
                    while (true) {
                        cout << "\n\n1: My account"
                            << "\n2: Deposit"
                            << "\n3: Withdraw"
                            << "\n4: Take loan"
                            << "\n5: Pay loan"
                            << "\n6: PIX"
                            << "\n7: Exit account"
                            << endl;
                        cin >> choice;
                        float amount;

                        try {
                            if (choice == 1) {
                                bank.checkBalance(CPF);
                            }
                            else if (choice == 2) {
                                cout << "Deposit amount: ";
                                cin >> amount;
                                bank.deposit(CPF, amount);
                                cout << "Successfull deposit!" << endl;
                            }
                            else if (choice == 3) {
                                cout << "Withdraw amount: ";
                                cin >> amount;
                                bank.withdraw(CPF, amount);
                                cout << "Successfull withdraw!" << endl;
                            }
                            else if (choice == 4) {
                                cout << "Loan amount: ";
                                cin >> amount;
                                bank.loan(CPF, amount);
                                cout << "Successfull loan!" << endl;
                            }
                            else if (choice == 5) {
                                bank.payLoan(CPF);
                                cout << "Successfull loan payment!" << endl;
                            }
                            else if (choice == 6) {
                                int to;
                                cout << "Sending amount: ";
                                cin >> amount;
                                cout << "to (CPF): ";
                                cin >> to;
                                bank.pix(CPF, to, amount);
                                cout << "Successfull PIX!" << endl;
                            }
                            else if (choice == 7)
                                break;
                            else
                                cout << "Invalid option." << endl;
                        }
                        catch(exception& e) {
                            cerr << e.what() << endl;
                        }
                    }
                }
            }
            else if (choice == 3) {
                bank.simulateMonth();
                cout << "1 month into the future!" << endl;
            }
            else if (choice == 4)
                break;
            else
                cout << "Invalid option." << endl;
        }
        catch(exception& e) {
            cerr << e.what() << endl;
        }
    }

    return 0;
}
