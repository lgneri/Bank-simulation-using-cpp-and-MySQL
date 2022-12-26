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
             << "\n3: Exit bank"
			 << endl;
        int choice;
        cin >> choice;
        int CPF;
        string password;

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
                        << "\n?: Exit account"
                        << endl;
                    cin >> choice;

                    if (choice == 1) {
                        bank.checkBalance(CPF);
                    }
                    else if (choice == 2) {
                        float amount;
                        cout << "Deposit amount: ";
                        cin >> amount;
                        bank.deposit(CPF, amount);
                    }
                    else
                        break;
                }
            }
        }
        else if (choice == 3)
            break;
    }

    /*string nome;
    cin >> nome;
    const char *senha = "pass";

    Client cliente(3, nome.c_str(), senha, 200, 4000);
    try {
        bank.createAccount(cliente);
    }
    catch(exception& e) {
        cerr << "Error: " << e.what() << endl;
    }*/

    //cout << bank.login(123, "senha");

    //bank.checkBalance(123);

    //bank.pix(123, 123455, 2100);

    /*MYSQL *con;	// the connection
    MYSQL_RES *res;	// the results
    MYSQL_ROW row;	// the results rows (array)

    Database db;

    // connect to the mysql database
    con = db.connect();

    res = db.performQuery(con, "SELECT * FROM Accounts;");

    cout << ("Database Output:\n") << endl;

    while ((row = mysql_fetch_row(res)) != NULL) {
        // the below row[] parametes may change depending on the size of the table and your objective
        cout << row[0] << " | " << row[1] << " | " << row[2] << " | " << row[3] << " | " << row[4] << " | " << row[5] << " | " << row[6] << endl << endl;
    }

    // clean up the database result
    mysql_free_result(res);

    // close database connection
    mysql_close(con);*/

    return 0;
}