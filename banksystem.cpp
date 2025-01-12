#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <iomanip>
#include <cstdlib>

using namespace std;

class Account {
public:
    string name;
    string fatherName;
    string address;
    string pin;
    float balance;

    Account() {
        name = "";
        fatherName = "";
        address = "";
        pin = "";
        balance = 0.0;
    }

    Account(string n, string f, string addr, string p, float bal = 0.0) {
        name = n;
        fatherName = f;
        address = addr;
        pin = p;
        balance = bal; 
    }

    void saveAccountData(ofstream &outFile, int accountNumber) {
        outFile << accountNumber << "\n";
        outFile << name << "\n";
        outFile << fatherName << "\n";
        outFile << address << "\n";
        outFile << pin << "\n";
        outFile << balance << "\n";
    }

    static Account loadAccountData(ifstream &inFile) {
        int accNum;
        string n, f, addr, p;
        float bal;

        inFile >> accNum;
        inFile.ignore(); 
        getline(inFile, n);
        getline(inFile, f);
        getline(inFile, addr);
        getline(inFile, p);
        inFile >> bal;
        inFile.ignore(); 

        return Account(n, f, addr, p, bal);
    }
};


class BankSystem {
private:
    map<int, Account> accounts;
    int currentAccountNumber = 1001;  

public:

    void displayTitle() {
        cout << "\n\n\n";
        cout << setw(40) << setfill(' ') << "***** Bank Management System *****\n";
        cout << "\n\n";
    }

    void clearScreen() {
        #ifdef _WIN32
            system("CLS"); 
        #else
            system("clear"); 
        #endif
    }

    
    void loadAccounts() {
        ifstream inFile("accounts.txt");

        if (inFile.is_open()) {
            while (!inFile.eof()) {
                Account acc = Account::loadAccountData(inFile);
                if (!acc.name.empty()) {
                    accounts[currentAccountNumber] = acc;
                    currentAccountNumber++;
                }
            }
            inFile.close();
        }
    }

    
    void saveAccounts() {
        ofstream outFile("accounts.txt");

        if (outFile.is_open()) {
            for (auto &account : accounts) {
                account.second.saveAccountData(outFile, account.first);
            }
            outFile.close();
        }
    }

    
    void openNewAccount() {
        clearScreen();
        displayTitle();

        string name, fatherName, address, pin;
        float initialDeposit;

        cout << "Enter your name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter your father's name: ";
        getline(cin, fatherName);
        cout << "Enter your address: ";
        getline(cin, address);
        cout << "Enter your pin: ";
        cin >> pin;

        
        cout << "Enter initial deposit amount: ";
        cin >> initialDeposit;

        Account newAccount(name, fatherName, address, pin, initialDeposit);
        accounts[currentAccountNumber] = newAccount;
        cout << "Account created successfully! Your account number is " << currentAccountNumber << endl;
        currentAccountNumber++;

        saveAccounts(); 
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get(); 
    }

   
    void getAccountInfo() {
        clearScreen();
        displayTitle();

        int accountNumber;
        string pin;
        cout << "Enter your account number: ";
        cin >> accountNumber;
        cout << "Enter your pin: ";
        cin >> pin;

        if (accounts.find(accountNumber) != accounts.end() && accounts[accountNumber].pin == pin) {
            Account acc = accounts[accountNumber];
            cout << "\nAccount Information\n";
            cout << "Name: " << acc.name << endl;
            cout << "Father's Name: " << acc.fatherName << endl;
            cout << "Address: " << acc.address << endl;
            cout << "Balance: " << acc.balance << endl;
        } else {
            cout << "Invalid account number or pin!" << endl;
        }

        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get(); 
    }

    
    void depositAmount() {
        clearScreen();
        displayTitle();

        int accountNumber;
        string pin;
        float amount;

        cout << "Enter your account number: ";
        cin >> accountNumber;
        cout << "Enter your pin: ";
        cin >> pin;

        if (accounts.find(accountNumber) != accounts.end() && accounts[accountNumber].pin == pin) {
            cout << "Enter amount to deposit: ";
            cin >> amount;

            if (amount > 0) {
                accounts[accountNumber].balance += amount;
                cout << "Deposit successful! New balance: " << accounts[accountNumber].balance << endl;
                saveAccounts(); 
            } else {
                cout << "Invalid deposit amount!" << endl;
            }
        } else {
            cout << "Invalid account number or pin!" << endl;
        }

        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    
    void withdrawAmount() {
        clearScreen();
        displayTitle();

        int accountNumber;
        string pin;
        float amount;

        cout << "Enter your account number: ";
        cin >> accountNumber;
        cout << "Enter your pin: ";
        cin >> pin;

        if (accounts.find(accountNumber) != accounts.end() && accounts[accountNumber].pin == pin) {
            cout << "Enter amount to withdraw: ";
            cin >> amount;

            if (amount > 0 && accounts[accountNumber].balance >= amount) {
                accounts[accountNumber].balance -= amount;
                cout << "Withdrawal successful! New balance: " << accounts[accountNumber].balance << endl;
                saveAccounts(); 
            } else {
                cout << "Invalid amount or insufficient funds!" << endl;
            }
        } else {
            cout << "Invalid account number or pin!" << endl;
        }

        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    
    void transferAmount() {
        clearScreen();
        displayTitle();

        int senderAccountNumber, receiverAccountNumber;
        string pin;
        float amount;

        cout << "Enter sender account number: ";
        cin >> senderAccountNumber;
        cout << "Enter sender pin: ";
        cin >> pin;

        if (accounts.find(senderAccountNumber) != accounts.end() && accounts[senderAccountNumber].pin == pin) {
            cout << "Enter receiver account number: ";
            cin >> receiverAccountNumber;

            if (accounts.find(receiverAccountNumber) != accounts.end()) {
                cout << "Enter amount to transfer: ";
                cin >> amount;

                if (amount > 0 && accounts[senderAccountNumber].balance >= amount) {
                    accounts[senderAccountNumber].balance -= amount;
                    accounts[receiverAccountNumber].balance += amount;
                    cout << "Transfer successful! Sender new balance: " << accounts[senderAccountNumber].balance
                         << ", Receiver new balance: " << accounts[receiverAccountNumber].balance << endl;
                    saveAccounts(); 
                } else {
                    cout << "Invalid amount or insufficient funds!" << endl;
                }
            } else {
                cout << "Invalid receiver account number!" << endl;
            }
        } else {
            cout << "Invalid sender account number or pin!" << endl;
        }

        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get(); 
    }


    void resetPin() {
        clearScreen();
        displayTitle();

        int accountNumber;
        string name;
        cout << "Enter your account number: ";
        cin >> accountNumber;
        cout << "Enter your name: ";
        cin.ignore();
        getline(cin, name);

        if (accounts.find(accountNumber) != accounts.end() && accounts[accountNumber].name == name) {
            string newPin;
            cout << "Enter new pin: ";
            cin >> newPin;

            accounts[accountNumber].pin = newPin;
            cout << "Pin reset successful!" << endl;
            saveAccounts(); 
        } else {
            cout << "Account number or name is incorrect!" << endl;
        }

        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get(); 
    }

    
    void resetData() {
        clearScreen();
        displayTitle();

        string confirmation;
        cout << "Are you sure you want to reset all data? (Y/N): ";
        cin >> confirmation;

        if (confirmation == "Y" || confirmation == "y") {
            accounts.clear();
            currentAccountNumber = 1001;
            cout << "All data has been reset." << endl;
            saveAccounts(); 
        } else {
            cout << "Data reset cancelled!" << endl;
        }

        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();  
    }

    void showMenu() {
        loadAccounts(); 
        int choice;

        while (true) {
            clearScreen();
            displayTitle();

            cout << "1. Open New Account\n";
            cout << "2. Get Account Information\n";
            cout << "3. Deposit Amount\n";
            cout << "4. Withdraw Amount\n";
            cout << "5. Transfer Amount\n";
            cout << "6. Reset Pin\n";
            cout << "7. Reset Data\n";
            cout << "8. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    openNewAccount();
                    break;
                case 2:
                    getAccountInfo();
                    break;
                case 3:
                    depositAmount();
                    break;
                case 4:
                    withdrawAmount();
                    break;
                case 5:
                    transferAmount();
                    break;
                case 6:
                    resetPin();
                    break;
                case 7:
                    resetData();
                    break;
                case 8:
                    return;
                default:
                    cout << "Invalid choice!" << endl;
                    break;
            }
        }
    }
};

int main() {
    BankSystem bankSystem;
    bankSystem.showMenu(); 
    return 0;
}
