/*
 * TITILE: driver.cpp
 * ABSTRACT: This is the driver program for the 'Customer' class.
 *           Except for allocating and deallocating memory this code is mainly used to validate information and actions.
 *           
 *           There are allowed a maximum of 20 customers in this program, and each customer can have one checking-
 *           and one saving account. When creating a customer their SSN and account number is checked for validity.
 *           If a account is closed and that customer no longer have any active accounts the customer is deleted from the system.
 * AUTHOR: Ralf Leistad
 * ID: 1995
 * DATE: 11/27/2018
 */

#include "Customer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

// Non-member functions / menu options
void readFromFile(Customer *arr[], int &customers, int &accounts);
void displayAllCustomers(Customer *arr[]);
void bankInfo(Customer *arr[], int customers, int accounts);
bool transferMoney(Customer *arr[]);
bool closeAccount(Customer *arr[], int &customers, int &accounts);
void searchCustomerInfo(Customer *arr[]);


// Non-member functions / helper
void displayMenu();
int firstNullptr(Customer *arr[]);
bool invalidCustomer(Customer *arr[], int newSSN, string newFname, string newLname);
bool validAccountNumber(Customer *arr[], int newNumber);
int existingCustomer(Customer *arr[], int newSSN, string newFname, string newLname);
bool existingAccountType(Customer *arr[], int index, int newType);
int getSSN(Customer *arr[], int accountNumber);
int customerIndex(Customer *arr[], int customerSSN);
int accountType(Customer *arr[], int sourceAccount);
void printSuccessTransfer(Customer *arr[], int indexOfCustomer, int accountType);
string prettyDouble(double uglyDouble);
bool printInfoFromLastName(Customer *arr[], string lastname);
void sort(Customer *arr[]);


int main()
{
    int customerCounter = 0;
    int accountCounter = 0;
    
    // Creating array with size = 20
    Customer * cust_ptr[MAX_CUSTOMERS];
    
    // Initialize pointers with nullptr
    for (int i = 0; i < MAX_CUSTOMERS; i++)
    {
        cust_ptr[i] = nullptr;
    }
    
    
    cout << "Welcome to CSUMB Bank" << endl;
    char option = '1';
    while(option != '7')
    {
        displayMenu();
        cout << "Your choice: ";
        cin >> option;
        
        switch(option)
        {
            case '1':
                readFromFile(cust_ptr, customerCounter, accountCounter);
                break;
                
            case '2':
                closeAccount(cust_ptr, customerCounter, accountCounter);
                break;
                
            case '3':
                searchCustomerInfo(cust_ptr);
                break;
                
            case '4':
                bankInfo(cust_ptr, customerCounter, accountCounter);
                break;
                
            case '5':
                transferMoney(cust_ptr);
                break;
                
            case '6':
                displayAllCustomers(cust_ptr);
                break;
                
            case '7':
                cout << "Bye!" << endl;
                exit(1);
                break;
                
            default:
                cout << "Invalid option..." << endl;
                break;
        }
    }
    
    // Delete all memory and set to nullptr
    for(int i = 0; i < MAX_CUSTOMERS; i++)
    {
        delete cust_ptr[i];
        cust_ptr[i] = nullptr;
    }
    
    
    return 0;
}

//--- Read From File
void readFromFile(Customer *arr[], int &customers, int &accounts)
{
    string filename;
    cout << "Enter file name: ";
    cin >> filename;
    
    fstream file(filename);
    
    if(file.fail())
    {
        cout << "Failed reading from file..." << endl;
        return;
    }
    
    int tempSize;
    file >> tempSize;
    
    int tempSSN;
    string tempFname;
    string tempLname;
    int tempNumber;
    int tempType;
    double tempBal;
    
    cout << "Read data...\n";
    
    // Loop enough times to create all objects
    for(int i = 0; i < tempSize; i++)
    {
        if(customers == MAX_CUSTOMERS)
        {
            cout << "ERROR - There are already " << MAX_CUSTOMERS << " customers in the system!" << endl;
            break;
        }
        
        file >> tempSSN;
        file >> tempFname;
        file >> tempLname;
        file >> tempNumber;
        file >> tempType;
        file >> tempBal;
        
        int index = firstNullptr(arr);
        
        if(tempNumber < 0)
        {
            cout << "Account Creation Failed - Account number cannot be negative" << endl;
        }
        else if(!validAccountNumber(arr, tempNumber))
        {
            cout << "Account Creation Failed - Account number " << tempNumber << " already exists." << endl;
        }
        else if(invalidCustomer(arr, tempSSN, tempFname, tempLname))
        {
            cout << "Account Creation Failed - A customer with SSN " << tempSSN << " already exists." << endl;
        }
        else if(existingCustomer(arr, tempSSN, tempFname, tempLname) != -1)
        {
            int customerIndex = existingCustomer(arr, tempSSN, tempFname, tempLname);
            
            if(existingAccountType(arr, customerIndex, tempType))
            {
                cout << "Account Creation Failed - " << tempFname << " " << tempLname << " cannot have two ";
                if(tempType == 1)
                {
                    cout << "checking accounts." << endl;
                }
                else
                {
                    cout << "saving accounts." << endl;
                }
            }
            else
            {
                arr[customerIndex]->setAccount(tempNumber, tempType, tempBal);
                accounts++;   
            }
        }
        else if(index != -1)
        {
            arr[index] = new Customer;
            arr[index]->setCustomer(tempSSN, tempFname, tempLname);
            arr[index]->setAccount(tempNumber, tempType, tempBal);
            customers++;
            accounts++;
        }
    }
    sort(arr);
    cout << "Done.\n";
}


//--- Display all customers
void displayAllCustomers(Customer *arr[])
{
    cout << "========== Customer List ==========" << endl;
    
    for(int i = 0; i < MAX_CUSTOMERS; i++)
    {
        if(arr[i] != nullptr)
        {
            arr[i]->displayCustomerInfo();
        }
    }
    
    cout << "===================================" << endl;
}

//--- Get print bank info (accounts and customers)
void bankInfo(Customer *arr[], int customers, int accounts)
{
    cout << "Number of Accounts: " << accounts << endl;
    
    for(int i = 0; i < MAX_CUSTOMERS; i++)
    {
        if(arr[i] != nullptr)
        {
            arr[i]->getAccountInfo();
        }
    }
    
    cout << endl << "Number of Customers: " << customers << endl;
    for(int i = 0; i < MAX_CUSTOMERS; i++)
    {
        if(arr[i] != nullptr)
        {
            arr[i]->getCustomerInfo();
        }
    }
}

//--- Transfer money between accounts
bool transferMoney(Customer *arr[])
{
    int source;
    int destination;
    double amount;
    
    cout << "Source Account: ";
    cin >> source;
    
    cout << "Destination Account: ";
    cin >> destination;
    
    cout << "Money Amount: ";
    cin >> amount;
    
    int sourceCustomer = getSSN(arr, source);
    int destinationCustomer = getSSN(arr, destination);
    
    if(source == destination)
    {
        cout << "Transfer Fail - Source and destination is same account" << endl;
        return false;
    }
    
    if(sourceCustomer == -1 || destinationCustomer == -1)
    {
        cout << "Transfer Fail - Incorrect account number" << endl;
        return false;
    }
    
    if(sourceCustomer == destinationCustomer)
    {
        int index = customerIndex(arr, sourceCustomer);
        int type = accountType(arr, source);
        
        if(!arr[index]->updateInternalBalance(type, amount))
        {
            cout << "Transfer Fail - Insufficient amount" << endl;
            return false;
        }
        else
        {
            cout << "Transfer Succeed." << endl << "New Balance" << endl;
            printSuccessTransfer(arr, index, type);
            
            ((type == 1) ? printSuccessTransfer(arr, index, 2) : printSuccessTransfer(arr, index, 1));
        }
    }
    else if(sourceCustomer != destinationCustomer)
    {
        amount += TANSFER_FEE;
        int sourceIndex = customerIndex(arr, sourceCustomer);
        int destinationIndex = customerIndex(arr, destinationCustomer);
        
        int sourceType = accountType(arr, source);
        int destinationType = accountType(arr, destination);
        
        bool addMoney = false;
        if(!arr[sourceIndex]->updateBalance(sourceType, amount, addMoney))
        {
            cout << "Transfer Fail - Insufficient amount" << endl;
            return false; 
        }
        addMoney = true;
        arr[destinationIndex]->updateBalance(destinationType, amount, addMoney);
        
        cout << "Transfer Succeed." << endl << "New Balance" << endl;
        
        printSuccessTransfer(arr,sourceIndex, sourceType);
        printSuccessTransfer(arr, destinationIndex, destinationType);
    }
    return true;
}

bool closeAccount(Customer *arr[], int &customers, int &accounts)
{
    int accountNumber;
    int customerSSN;
    
    cout << "Enter Account Number: ";
    cin >> accountNumber;
    
    if(validAccountNumber(arr, accountNumber))
    {
        cout << "Incorrect account number." << endl;
        return false;
    }
    
    cout << "Enter Customer SSN: ";
    cin >> customerSSN;
    
    int index = customerIndex(arr, customerSSN);
    
    if(index == -1)
    {
        cout << "Incorrect SSN." << endl;
        return false;
    }
    
    cout << "Accout Closed" << endl;
    int type = accountType(arr, accountNumber);
    arr[index]->close(type);
    accounts--;
    
    if(!arr[index]->hasActiveAccount())
    {
        cout << "Customer " << arr[index]->getFirstname() << " " << arr[index]->getLastname() << " is also deleted." << endl;
        delete arr[index];
        arr[index] = nullptr;
        customers--;
    }
    return true;
}

//-- Get customer info by searching for SSN, lastname or accountnumber
void searchCustomerInfo(Customer *arr[])
{
    int option;
    int number;
    string name;
    
    int index = 0;
    int accType = 0;
    int customerSSN = 0;
    
    cout << "\nSSN(1) / Last Name(2) / Account Number(3): ";
    cin >> option;
    
    switch(option)
    {
        case 1:
            cout << "Enter SSN: ";
            cin >> number;
            
            index = customerIndex(arr, number);
            
            if(index == -1)
            {
                cout << "No customer with the SSN." << endl;
                return;
            }
            
            arr[index]->customerInfoSearch();
            
            break;
        
        case 2:
            cout << "Enter a last name: ";
            cin >> name;
            
            if(!printInfoFromLastName(arr, name))
            {
                cout << "No customer with the last name." << endl;
            }
            
            break;
            
        case 3:
            cout << "Enter an account number: ";
            cin >> number;
            
            if(validAccountNumber(arr, number))
            {
                cout << "No customer with the account number." << endl;
                return;
            }
            
            accType = accountType(arr, number);
            customerSSN = getSSN(arr, number);
            index = customerIndex(arr, customerSSN);
            
            arr[index]->customerInfoSearch(accType);
            
            break;
            
        default:
            cout << "ERROR - Invalid option" << endl;
    }
}



/**********************
 *  HELPER FUNCTIONS  *
 **********************/

//--- Display Menu
void displayMenu()
{
    cout << "\nSelect your choice:\n";
    cout << "\t1. Read customer data file\n";
    cout << "\t2. Close an account\n";
    cout << "\t3. Customer info\n";
    cout << "\t4. Bank info\n";
    cout << "\t5. Transfer money\n";
    cout << "\t6. Customer list\n";
    cout << "\t7. Exit\n";
}

//--- Returns the index of first nullptr found
int firstNullptr(Customer *arr[])
{
    for(int i = 0; i < MAX_CUSTOMERS; i++)
    {
        if(arr[i] == nullptr)
        {
            return i;
        }
    }
    return -1;
}

//--- Check if a customer with the given SSN is already created but different name
bool invalidCustomer(Customer *arr[], int newSSN, string newFname, string newLname)
{
    for(int i = 0; i < MAX_CUSTOMERS; i++)
    {
        if(arr[i] != nullptr)
        {
            if(arr[i]->getSSN() == newSSN)
            {
                if(arr[i]->getFirstname() != newFname || arr[i]->getLastname() != newLname)
                {
                    //cout << "Account Creation Failed - A customer with SSN " << newSSN << " already exists." << endl;
                    return true;
                }
            }
        }
    }
    return false;
}

//--- Checks if the account number is already in use
bool validAccountNumber(Customer *arr[], int newNumber)
{
    bool alreadyInUse = true;
    
    for(int i = 0; i < MAX_CUSTOMERS; i++)
    {
        if(arr[i] != nullptr)
        {
            if(arr[i]->getSavingNumber() == newNumber)
            {
                alreadyInUse = false;
            }
            else if(arr[i]->getCheckingNumber() == newNumber)
            {
                alreadyInUse = false;
            }
        }
    }
    return alreadyInUse;
}

//--- Check if the customer exists already
int existingCustomer(Customer *arr[], int newSSN, string newFname, string newLname)
{
    for(int i = 0; i < MAX_CUSTOMERS; i++)
    {
        if(arr[i] != nullptr)
        {
            if(arr[i]->getSSN() == newSSN)
            {
                if(arr[i]->getFirstname() == newFname || arr[i]->getLastname() == newLname)
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

//--- Check if customer already has that account type
bool existingAccountType(Customer *arr[], int index, int newType)
{
    if(arr[index]->getCheckingType() == newType)
    {
        return true;
    }
    else if(arr[index]->getSavingType() == newType)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//--- Return the SSN for the customer with the given account
int getSSN(Customer *arr[], int accountNumber)
{
    
    for(int i = 0; i < MAX_CUSTOMERS; i++)
    {
        if(arr[i] != nullptr)
        {
            if(arr[i]->getCheckingNumber() == accountNumber)
            {
                return arr[i]->getSSN();
            }
            else if(arr[i]->getSavingNumber() == accountNumber)
            {
                return arr[i]->getSSN();
            }
        }
    }
    return -1;
}

//--- Return the index of the customer with the given SSN
int customerIndex(Customer *arr[], int customerSSN)
{
    for(int i = 0; i < MAX_CUSTOMERS; i++)
    {
        if(arr[i] != nullptr)
        {
            if(arr[i]->getSSN() == customerSSN)
            {
                return i;
            }
        }
    }
    return -1;
}

//--- Return the account type of the given account
int accountType(Customer *arr[], int sourceAccount)
{
    for(int i = 0; i < MAX_CUSTOMERS; i++)
    {
        if(arr[i] != nullptr)
        {
            if(arr[i]->getCheckingNumber() == sourceAccount)
            {
                return 1;
            }
            else if(arr[i]->getSavingNumber() == sourceAccount)
            {
                return 2;
            }
        }
    }
    return -1;
}

//--- Print information if transfer is successful
void printSuccessTransfer(Customer *arr[], int indexOfCustomer, int accountType)
{
    int accountNumber;
    double bal;
    
    if(accountType == 1)
    {
        accountNumber = arr[indexOfCustomer]->getCheckingNumber();
        bal = arr[indexOfCustomer]->getCheckingBalance();
        
        cout << "\t" << accountNumber << ": " << prettyDouble(bal) << endl;
    }
    else if(accountType == 2)
    {
        accountNumber = arr[indexOfCustomer]->getSavingNumber();
        bal = arr[indexOfCustomer]->getSavingBalance();
        
        cout << "\t" << accountNumber << ": " << prettyDouble(bal) << endl;
    }
}

//--- Limit double to two decimalpoints
string prettyDouble(double uglyDouble)
{
    stringstream beautifulDouble;
    beautifulDouble << fixed << setprecision(2) << uglyDouble;
    return "$" + beautifulDouble.str();
}

//--- Print info based on last name
bool printInfoFromLastName(Customer *arr[], string lastname)
{
    bool found = false;
    
    for(int i = 0; i < MAX_CUSTOMERS; i++)
    {
        if(arr[i] != nullptr)
        {
            if(arr[i]->getLastname() == lastname)
            {
                arr[i]->customerInfoSearch();
                found = true;
            }
        }
    }
    return found;
}

//--- Sort the array based on SSN
void sort(Customer *arr[])
{
    for(int i = 0; i < MAX_CUSTOMERS; i++)
    {
        if(arr[i] != nullptr)
        {
            int j = i;
            while(j < MAX_CUSTOMERS && arr[j] != nullptr)
            {
                if(arr[i]->getSSN() > arr[j]->getSSN())
                {
                    Customer * temp[1];
                    temp[0] = arr[i];
                        
                    arr[i] = arr[j];
                    arr[j] = temp[0];
                }
                j++;
            }
        }
    }
}

