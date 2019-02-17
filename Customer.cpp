/*
 * TITILE: Customer.cpp
 * ABSTRACT: This is the implementation of the 'Customer' class.
 *           Most functionality that directly affects or interacts with a Customer-object is defined here.
 * AUTHOR: Ralf Leistad
 * ID: 1995
 * DATE: 11/27/2018
 */

#include "Customer.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

//--- Display Customer Info
void Customer::displayCustomerInfo()
{
    cout << "\tSSN: " << ssn << endl;
    cout << "\tName: " << firstName << " " << lastName << endl << endl;
}

//--- Create Customer
bool Customer::setCustomer(int newSSN, string newFname, string newLname)
{   
    firstName = newFname;
    lastName = newLname;
    ssn = newSSN;
}

//--- Set account to customer
bool Customer::setAccount(int newNumber, int newType, double newBal)
{
    if(newType < 1 || newType > 2)
    {
        cout << "ERROR - Invalid account type!" << endl;
        return false;
    }
    
    if(newBal < 0.0)
    {
        cout << "ERROR - Invalid initial balance!" << endl;
        return false;
    }
    
    if(newType == 1)
    {
        checkingNumber = newNumber;
        checkingType = newType;
        checkingBalance = newBal;
        
        activeAccounts++;
    }
    else
    {
        savingNumber = newNumber;
        savingType = newType;
        savingBalance = newBal;
        
        activeAccounts++;
    }

    return true;
}

//--- Get Account info
void Customer::getAccountInfo()
{
    string result = "";
    
    if(checkingType == 1)
    {
        result += "\t" + to_string(checkingNumber) + ": " + beautifyDouble(checkingBalance) + " (" + checkingName + ")\n";
    }
    
    if(savingType == 2)
    {
        result += "\t" + to_string(savingNumber) + ": " + beautifyDouble(savingBalance) + " (" + savingName + ")\n";
    }
    
    cout << result;
}

//--- Get Customer info
void Customer::getCustomerInfo()
{
    string result = "\t";
    
    result += firstName + " " + lastName + ": " + to_string(ssn) + " - ";
    
    if(checkingType == 1 && savingType == 2)
    {
        result += to_string(checkingNumber) + "(" + beautifyDouble(checkingBalance) + "), ";
        result += to_string(savingNumber) + "(" + beautifyDouble(savingBalance) + ")\n";
    }
    else if(checkingType == 1)
    {
        result += to_string(checkingNumber) + "(" + beautifyDouble(checkingBalance) + ")\n";
    }
    else if(savingType == 2)
    {
        result += to_string(savingNumber) + "(" + beautifyDouble(savingBalance) + ")\n";
    }
    
    cout << result;
}

//--- Update the balance of both accounts when transfering between two accounts with same owner
bool Customer::updateInternalBalance(int sourceType, double amount)
{
    if(sourceType == 1)
    {
        if(getCheckingBalance() >= amount)
        {
            checkingBalance -= amount;
            savingBalance += amount;
            return true;
        }
        else
        {
            return false;
        }
    }
    else if(sourceType == 2)
    {
        if(getSavingBalance() >= amount)
        {
            savingBalance -= amount;
            checkingBalance += amount;
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

//--- Update balance of account when two different owners
bool Customer::updateBalance(int accountType, double amount, bool addAmount)
{
    if(addAmount)
    {
        if(accountType == 1)
        {
            checkingBalance += (amount - TANSFER_FEE);
            return true;
        }
        else if(accountType == 2)
        {
            savingBalance += (amount - TANSFER_FEE);
            return true;
        }
    }
    else if(!addAmount)
    {
        if(accountType == 1)
        {
            if(getCheckingBalance() >= amount)
            {
                checkingBalance -= amount;
                return true;
            }
            else
            {
                return false;
            }
        }
        else if(accountType == 2)
        {
            if(getSavingBalance() >= amount)
            {
                savingBalance -= amount;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}

//--- Close account
void Customer::close(int accountType)
{
    if(accountType == 1)
    {
        printClosingInfo(1);
        checkingNumber = 0;
        checkingType = 0;
        checkingBalance = 0.0;
        activeAccounts--;
        return;
    }
    else
    {
        printClosingInfo(2);
        savingNumber = 0;
        savingType = 0;
        savingBalance = 0.0;
        activeAccounts--;
    }
    return;
}

//--- Print customer info by searching for lastname
void Customer::customerInfoSearch()
{
    if(checkingType == 1)
    {
        cout << "\n\tName: " << firstName << " " << lastName << endl;
        cout << "\tAccount Number: " << checkingNumber << endl;
        cout << "\tAccount Type: Checking" << endl;
        cout << "\tBalance: " << beautifyDouble(checkingBalance) << endl;
    }
    
    if(savingType == 2)
    {
        cout << "\tName: " << firstName << " " << lastName << endl;
        cout << "\tAccount Number: " << savingNumber << endl;
        cout << "\tAccount Type: Saving" << endl;
        cout << "\tBalance: " << beautifyDouble(savingBalance) << endl; 
    }   
}

//--- Overloaded search function that takes a account type as argument
void Customer::customerInfoSearch(int accountType)
{
    if(accountType == 1)
    {
        cout << "\n\tName: " << firstName << " " << lastName << endl;
        cout << "\tAccount Number: " << checkingNumber << endl;
        cout << "\tAccount Type: Checking" << endl;
        cout << "\tBalance: " << beautifyDouble(checkingBalance) << endl;
    }
    else
    {
        cout << "\n\tName: " << firstName << " " << lastName << endl;
        cout << "\tAccount Number: " << savingNumber << endl;
        cout << "\tAccount Type: Saving" << endl;
        cout << "\tBalance: " << beautifyDouble(savingBalance) << endl; 
    }
}



/****************************
 *  HELPER FUNCTIONS BELOW  *
 ****************************/

//--- Get firstname
string Customer::getFirstname()
{
    return firstName;
}

//--- Get lastname
string Customer::getLastname()
{
    return lastName;
}

//-- Get SSN
int Customer::getSSN()
{
    return ssn;
}

string Customer::beautifyDouble(double uglyDouble)
{
    stringstream prettyDouble;
    prettyDouble << fixed << setprecision(2) << uglyDouble;
    return "$" + prettyDouble.str();
}

//--- Get account number for saving account
int Customer::getSavingNumber()
{
    return savingNumber;
}

//--- Get account number for checking account
int Customer::getCheckingNumber()
{
    return checkingNumber;
}

//--- Return saving type
int Customer::getSavingType()
{
    return savingType;
}

//--- Return checking type
int Customer::getCheckingType()
{
    return checkingType;
}

//--- Return balance of checking account
double Customer::getCheckingBalance()
{
    return checkingBalance;
}

//--- Return balance of saving account
double Customer::getSavingBalance()
{
    return savingBalance;
}

//-- Return true if 'activeAccounts' is greater than 0
bool Customer::hasActiveAccount()
{
    if(activeAccounts == 0)
    {
        return false;
    }
    
    return true;
}

//-- Print info about account that is being closed
void Customer::printClosingInfo(int accountType)
{
    if(accountType == 1)
    {
        cout << "\tName: " << firstName << " " << lastName << endl;
        cout << "\tAccount Number: " << checkingNumber << endl;
        cout << "\tAccount Type: Checking" << endl;
        cout << "\tBalance: " << beautifyDouble(checkingBalance) << endl;
    }
    else
    {
        cout << "\tName: " << firstName << " " << lastName << endl;
        cout << "\tAccount Number: " << savingNumber << endl;
        cout << "\tAccount Type: Saving" << endl;
        cout << "\tBalance: " << beautifyDouble(savingBalance) << endl; 
    }
}