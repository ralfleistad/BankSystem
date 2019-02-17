/*
 * TITILE: Customer.h
 * ABSTRACT: This is the interface file that defines the variables and functions in the 'Customer' class.
 * AUTHOR: Ralf Leistad
 * ID: 1995
 * DATE: 11/27/2018
 */

#include <string>
#include <iostream>
using namespace std;

int const MAX_CUSTOMERS = 20;
double const TANSFER_FEE = 5.0;

class Customer
{
public:
    void displayCustomerInfo();
    bool setCustomer(int newSSN, string newFname, string newLname);
    bool setAccount(int newNumber, int newType, double newBal);
    void getAccountInfo();
    void getCustomerInfo();
    bool updateInternalBalance(int sourceType, double amount);
    bool updateBalance(int accountType, double amount, bool addAmount);
    void close(int accountType);
    void customerInfoSearch();
    void customerInfoSearch(int accountType);
    
    // HELPER FUNCTIONS
    string getFirstname();
    string getLastname();
    int getSSN();
    string beautifyDouble(double uglyDouble);
    int getSavingNumber();
    int getCheckingNumber();
    int getSavingType();
    int getCheckingType();
    double getCheckingBalance();
    double getSavingBalance();
    bool hasActiveAccount();
    void printClosingInfo(int accountType);

private:
    int ssn;
    string firstName;
    string lastName;
    
    int savingType = 0; // 2
    int savingNumber = 0;
    double savingBalance = 0.0;
    string savingName = "Saving";
    
    int checkingType = 0; // 1
    int checkingNumber = 0;
    double checkingBalance = 0.0;
    string checkingName = "Checking";
    
    int activeAccounts = 0;
};