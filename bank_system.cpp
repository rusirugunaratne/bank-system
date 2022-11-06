/*
* Quest No : 3
* Name     : Gunaratne U.B.R.A
* Index No : 19/ENG/023
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <cstdlib>
#include <iomanip>

using namespace std;

/*
* Global Variables
*/
bool IS_SATURDAY_UPDATED = false;
bool IS_SUNDAY_UPDATED = false;
double BANK_BALANCE = 0;
string PREVIOUS_DATE = " ";
string OVERDRAFT_DAY = "20210101";

/*
* Class Account
*/
class Account
{
public:
    string openingDate;
    string accountNumber;
    long double openingBalance;
    long double finalBalance;
    string previousDate;
};

/*
* Class Transaction
*/
class Transaction
{
public:
    string transactionDate;
    string accountNumber;
    int transactionType;
    double transationAmount;
    double updatedBalance;
};

/*
* Function - update bank
* Update the bank when a transaction happens
*/
void updateBank(string date, int transcationType, double transactionAmount)
{
    ofstream bankAccount;
    bankAccount.open("BankAccount.txt", std::ios_base::app);
    BANK_BALANCE += transactionAmount;

    if (transactionAmount < 0)
    {
        transactionAmount *= -1;
    }

    bankAccount << fixed << setprecision(2) << date << "," << transcationType << "," << transactionAmount << "," << BANK_BALANCE << endl;
}

/*
* Function - add charge and overdraft at the end of the day
*/
void addChargeOrOverdraft(string date, vector<Account> &accounts)
{
    if (OVERDRAFT_DAY != date)
    {
        for (int i = 0; i < accounts.size(); i++)
        {

            ofstream accountFile;
            accountFile.open(accounts[i].accountNumber + ".txt", std::ios_base::app);
            if (accounts[i].finalBalance < 1000 && accounts[i].finalBalance >= 0)
            {
                accounts[i].finalBalance = accounts[i].finalBalance - 10;
                accountFile << fixed << setprecision(2) << OVERDRAFT_DAY << "," << '4' << "," << 10.00 << "," << accounts[i].finalBalance << endl;
                updateBank(OVERDRAFT_DAY, 4, 10);
            }
            if (accounts[i].finalBalance < 0)
            {
                accounts[i].finalBalance = accounts[i].finalBalance - 50;
                accountFile << fixed << setprecision(2) << OVERDRAFT_DAY << "," << '4' << "," << 50.00 << "," << accounts[i].finalBalance << endl;
                updateBank(OVERDRAFT_DAY, 4, 50);
            }
            accountFile.close();
        }
        OVERDRAFT_DAY = date;
    }
}

/*
* Function - add interest
* add an interest daily
*/
void addInterest(string date, vector<Account> &accounts)
{

    if (PREVIOUS_DATE != date)
    {
        PREVIOUS_DATE = date;

        for (int i = 0; i < accounts.size(); i++)
        {

            ofstream accountFile;
            accountFile.open(accounts[i].accountNumber + ".txt", std::ios_base::app);
            if (accounts[i].finalBalance > 0)
            {
                double interest = accounts[i].finalBalance * (0.02 / 100.00);
                accounts[i].finalBalance += interest;
                accountFile << fixed << setprecision(2) << date << "," << 3 << "," << interest << "," << accounts[i].finalBalance << endl;
                double bankDeduction = (-1) * interest;
                updateBank(date, 3, bankDeduction);
            }
            accountFile.close();
        }
    }
}

/*
* Function - add weekend interest
* add the interest for the weekends
*/
void addWeekendInterest(string date, vector<Account> &accounts)
{
    if (date == "20210107" && !IS_SATURDAY_UPDATED)
    {
        for (int i = 0; i < accounts.size(); i++)
        {

            ofstream accountFile;
            accountFile.open(accounts[i].accountNumber + ".txt", std::ios_base::app);
            if (accounts[i].finalBalance > 0)
            {
                double interest = accounts[i].finalBalance * (0.02 / 100.00);
                accounts[i].finalBalance += interest;
                accountFile << fixed << setprecision(2) << "20210105"
                            << "," << 3 << "," << interest << "," << accounts[i].finalBalance << endl;
                double bankDeduction = (-1) * interest;
                updateBank(date, 3, bankDeduction);
            }
            accountFile.close();
        }

        IS_SATURDAY_UPDATED = true;
    }
    if (date == "20210107" && !IS_SUNDAY_UPDATED)
    {
        for (int i = 0; i < accounts.size(); i++)
        {

            ofstream accountFile;
            accountFile.open(accounts[i].accountNumber + ".txt", std::ios_base::app);
            if (accounts[i].finalBalance > 0)
            {
                double interest = accounts[i].finalBalance * (0.02 / 100.00);
                accounts[i].finalBalance += interest;
                accountFile << fixed << setprecision(2) << "20210106"
                            << "," << 3 << "," << interest << "," << accounts[i].finalBalance << endl;
                double bankDeduction = (-1) * interest;
                updateBank(date, 3, bankDeduction);
            }
            accountFile.close();
        }

        IS_SUNDAY_UPDATED = true;
    }
}

/*
* Function - create accounts
* create text files for each account
*/
void createAccounts(vector<Account> &accounts)
{
    ifstream balanceText("balance.txt");
    string balanceString;
    string line;
    while (getline(balanceText, line))
    {
        if (line.empty())
        {
            continue;
        }
        Account account;
        stringstream ss(line);
        getline(ss, account.openingDate, ',');
        getline(ss, account.accountNumber, ',');
        getline(ss, balanceString, ',');

        account.openingBalance = stod(balanceString); //Having trouble
        ofstream newFile(account.accountNumber + ".txt");
        newFile << fixed << setprecision(2) << account.openingDate << "," << '0' << "," << account.openingBalance << "," << account.openingBalance << endl;
        account.finalBalance = account.openingBalance;
        account.previousDate = " ";
        accounts.push_back(account);
    }
}

/*
* Function - add transactions to each account
*/
void addTransactions(vector<Account> &accounts)
{
    ifstream transactions("transaction.txt");
    string transactionTypeString;
    string transactionAmountString;
    string line;
    while (getline(transactions, line))
    {
        if (line.empty())
        {
            continue;
        }
        Transaction transaction;
        stringstream ss(line);
        getline(ss, transaction.transactionDate, ',');
        getline(ss, transaction.accountNumber, ',');
        getline(ss, transactionTypeString, ',');
        getline(ss, transactionAmountString, ',');
        transaction.transactionType = stoi(transactionTypeString);
        transaction.transationAmount = stod(transactionAmountString);
        int accountIndex = 0;
        for (int i = 0; i < accounts.size(); i++)
        {
            if (transaction.accountNumber == accounts[i].accountNumber)
            {
                accountIndex = i;
            }
        }
        addChargeOrOverdraft(transaction.transactionDate, accounts);
        addWeekendInterest(transaction.transactionDate, accounts);
        addInterest(transaction.transactionDate, accounts);
        ofstream accountFile;
        accountFile.open(accounts[accountIndex].accountNumber + ".txt", std::ios_base::app);
        switch (transaction.transactionType)
        {
        case 1:
            accounts[accountIndex].finalBalance = accounts[accountIndex].finalBalance + transaction.transationAmount;
            updateBank(transaction.transactionDate, transaction.transactionType, transaction.transationAmount);
            break;
        case 2:
            accounts[accountIndex].finalBalance = accounts[accountIndex].finalBalance - transaction.transationAmount;
            double reducedAmount = -1 * transaction.transationAmount;
            updateBank(transaction.transactionDate, transaction.transactionType, reducedAmount);
            break;
        }
        accountFile << fixed << setprecision(2) << transaction.transactionDate << "," << transaction.transactionType << "," << transaction.transationAmount << "," << accounts[accountIndex].finalBalance << endl;
    }
    addChargeOrOverdraft(" ", accounts);
}

/*
* Function - get the opening balance of the bank
*/
void initializeBankBalance(vector<Account> &accounts)
{
    for (int i = 0; i < accounts.size(); i++)
    {
        BANK_BALANCE += accounts[0].openingBalance;
    }
    ofstream bankAccount("BankAccount.txt");
    bankAccount << BANK_BALANCE << endl;
}

/*
* Function - display the account balance of every account
*/
void viewBalanceOfAll(vector<Account> accounts)
{
    cout << "+----------------------------------------+" << endl;
    cout << "|  ACC Number          ACC Balance       |" << endl;
    cout << "+----------------------------------------+" << endl;

    for (int i = 0; i < accounts.size(); i++)
    {
        cout << fixed << setprecision(2) << "  " << accounts[i].accountNumber << "                 " << accounts[i].finalBalance << endl;
        cout << "+----------------------------------------+" << endl;
    }
}

/*
* Function - calculate the starting and ending balance of a selected account
*/
void startingAndEndBalance(vector<Account> accounts)
{
    string accountNumber;
    string date;
    cout << "  Enter the account number: ";
    cin >> accountNumber;
    cout << "  Enter the date: ";
    cin >> date;
    ifstream accounntFile(accountNumber + ".txt");
    vector<Transaction> transactions;
    string transactionType;
    string transactionAmount;
    string finalBalance;
    string line;
    while (getline(accounntFile, line))
    {
        Transaction transaction;
        stringstream ss(line);
        getline(ss, transaction.transactionDate, ',');
        getline(ss, transactionType, ',');
        getline(ss, transactionAmount, ',');
        getline(ss, finalBalance, ',');
        if (date == transaction.transactionDate)
        {
            transaction.transactionType = stoi(transactionType);
            transaction.transationAmount = stod(transactionAmount);
            transaction.updatedBalance = stod(finalBalance);
            transactions.push_back(transaction);
        }
    }
    cout << endl;
    cout << fixed << setprecision(2) << "  Opening Balance of the day:    " << transactions[0].updatedBalance + transactions[0].transationAmount << endl;
    cout << fixed << setprecision(2) << "  Day end Balance           :    " << transactions[transactions.size() - 1].updatedBalance << endl;
}

/*
* Function - Displays main menu
*/
void mainMenu(vector<Account> accounts)
{
    cout << "+----------------------------------------+" << endl;
    cout << "|               BANK SYSTEM              |" << endl;
    cout << "+----------------------------------------+" << endl;
    int option = 0;
    while (option != 4)
    {
        cout << "Select any option" << endl;
        cout << "  1- View Bank Vault" << endl;
        cout << "  2- View current balance of all accounts" << endl;
        cout << "  3- See starting balance and End of the day balance" << endl;
        cout << "  4- Exit" << endl;
        while (true)
        {
            cout << "  Enter your option: ";
            cin >> option;
            if (option == 1 || option == 2 || option == 3 || option == 4)
            {
                break;
            }
        }
        switch (option)
        {
        case 1:
        {
            cout << fixed << setprecision(2) << "  Bank Vault Balance: " << BANK_BALANCE << endl;
        }
        break;
        case 2:
            viewBalanceOfAll(accounts);
            cout << endl;
            break;
        case 3:
            startingAndEndBalance(accounts);
            cout << endl;
            break;
        case 4:
        {
            cout << "  THANK YOU! HAVE A NICE DAY" << endl;
            cout << endl;
            exit;
        }
        break;
        default:
        {
            cout << "\t\t\tInvalid Input! Try Again";
        }
        }
    }
}

/*
* Function - main function
*/
int main()
{
    vector<Account> accounts;
    createAccounts(accounts);
    initializeBankBalance(accounts);
    addTransactions(accounts);
    mainMenu(accounts);
}