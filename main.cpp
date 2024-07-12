#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <limits>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;

// Define a structure to hold the transaction details
struct Transaction {
    int index;
    string description;
    double amount;
    string type; // "Credit" or "Debit"
    string date; // Format: YYYY-MM-DD
};

// Function to save transactions to a file
void saveTransactions(const vector<Transaction>& transactions) {
    ofstream outFile("transactions.txt");
    for (const auto& transaction : transactions) {
        outFile << transaction.index << ","
                << transaction.description << ","
                << transaction.amount << ","
                << transaction.type << ","
                << transaction.date << endl;
    }
    outFile.close();
    cout << "Transactions saved successfully.\n";
}

// Function to load transactions from a file
void loadTransactions(vector<Transaction>& transactions) {
    ifstream inFile("transactions.txt");
    if (!inFile) {
        cout << "No saved transactions found.\n";
        return;
    }

    transactions.clear();
    Transaction transaction;
    string line;

    while (getline(inFile, line)) {
        size_t pos = 0;
        pos = line.find(",");
        transaction.index = stoi(line.substr(0, pos));
        line.erase(0, pos + 1);

        pos = line.find(",");
        transaction.description = line.substr(0, pos);
        line.erase(0, pos + 1);

        pos = line.find(",");
        transaction.amount = stod(line.substr(0, pos));
        line.erase(0, pos + 1);

        pos = line.find(",");
        transaction.type = line.substr(0, pos);
        line.erase(0, pos + 1);

        transaction.date = line;
        transactions.push_back(transaction);
    }
    inFile.close();
    cout << "Transactions loaded successfully.\n";
}

// Function to validate the date format (YYYY-MM-DD)
bool validateDate(const string& date) {
    if (date.size() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    for (size_t i = 0; i < date.size(); ++i) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }
    return true;
}

// Function to add a new transaction
void addTransaction(vector<Transaction>& transactions) {
    Transaction newTransaction;
    int typeChoice;

    cout << "Enter transaction description: ";
    cin.ignore(); // Ignore any remaining newline character
    getline(cin, newTransaction.description);

    cout << "Enter transaction amount: ";
    while (true) {
        if (cin >> newTransaction.amount && newTransaction.amount > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        } else {
            cout << "Invalid amount. Please enter a positive number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    cout << "Enter transaction type (1 for Credit, 2 for Debit): ";
    while (true) {
        if (cin >> typeChoice && (typeChoice == 1 || typeChoice == 2)) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        } else {
            cout << "Invalid choice. Enter 1 for Credit or 2 for Debit: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    newTransaction.type = (typeChoice == 1) ? "Credit" : "Debit";

    cout << "Enter transaction date (YYYY-MM-DD): ";
    while (true) {
        getline(cin, newTransaction.date);
        if (validateDate(newTransaction.date)) {
            break;
        } else {
            cout << "Invalid date format. Please enter date in YYYY-MM-DD format: ";
        }
    }

    newTransaction.index = transactions.size(); // Unique index
    transactions.push_back(newTransaction);
    cout << "Transaction added successfully.\n";
}

// Function to view transactions
void viewTransactions(const vector<Transaction>& transactions, bool sortByDate = false) {
    vector<Transaction> transactionsToView = transactions;

    if (sortByDate) {
        sort(transactionsToView.begin(), transactionsToView.end(), [](const Transaction& a, const Transaction& b) {
            return a.date < b.date;
        });
    }

    cout << "\nTransactions:\n";
    cout << left << setw(8) << "Index" << "|" << setw(30) << "Description" << "|" << setw(10) << "Amount" << "|" << setw(10) << "Type" << "|" << setw(12) << "Date" << endl;

    for (int i = 0; i < 75; ++i) {
        if (i == 7 || i == 38 || i == 49 || i == 60) {
            cout << "+";
        } else {
            cout << "-";
        }
    }
    cout << endl;

    for (const auto& transaction : transactionsToView) {
        cout << left << setw(8) << transaction.index << "|" << setw(30) << transaction.description << "|"
             << setw(10) << fixed << setprecision(2) << transaction.amount << "|"
             << setw(10) << transaction.type << "|" << setw(12) << transaction.date << endl;

        for (int j = 0; j < 75; ++j) {
            if (j == 7 || j == 38 || j == 49 || j == 60) {
                cout << "+";
            } else {
                cout << "-";
            }
        }
        cout << endl;
    }
}

// Function to delete a transaction
void deleteTransaction(vector<Transaction>& transactions) {
    int index;

    cout << "Enter the index of the transaction to delete: ";
    while (!(cin >> index) || index < 0 || index >= transactions.size()) {
        cout << "Invalid index. Please enter a valid index: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    const auto& transaction = transactions[index];
    cout << "You are about to delete the following transaction:\n";
    cout << left << setw(8) << "Index" << "|" << setw(30) << "Description" << "|" << setw(10) << "Amount" << "|" << setw(10) << "Type" << "|" << setw(12) << "Date" << endl;

    for (int i = 0; i < 75; ++i) {
        if (i == 7 || i == 38 || i == 49 || i == 60) {
            cout << "+";
        } else {
            cout << "-";
        }
    }
    cout << endl;

    cout << left << setw(8) << transaction.index << "|" << setw(30) << transaction.description << "|"
         << setw(10) << fixed << setprecision(2) << transaction.amount << "|"
         << setw(10) << transaction.type << "|" << setw(12) << transaction.date << endl;

    cout << "Are you sure you want to delete this transaction? (y/n): ";
    char confirm;
    cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        transactions.erase(transactions.begin() + index);
        cout << "Transaction deleted successfully.\n";
    } else {
        cout << "Transaction deletion cancelled.\n";
    }
}

// Function to edit a transaction
void editTransaction(vector<Transaction>& transactions) {
    int index;
    cout << "Enter the index of the transaction to edit: ";
    while (!(cin >> index) || index < 0 || index >= transactions.size()) {
        cout << "Invalid index. Please enter a valid index: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    auto& transaction = transactions[index];
    cout << "Current Transaction Details:\n";
    cout << left << setw(8) << "Index" << "|" << setw(30) << "Description" << "|" << setw(10) << "Amount" << "|" << setw(10) << "Type" << "|" << setw(12) << "Date" << endl;

    for (int i = 0; i < 75; ++i) {
        if (i == 7 || i == 38 || i == 49 || i == 60) {
            cout << "+";
        } else {
            cout << "-";
        }
    }
    cout << endl;

    cout << left << setw(8) << transaction.index << "|" << setw(30) << transaction.description << "|"
         << setw(10) << fixed << setprecision(2) << transaction.amount << "|"
         << setw(10) << transaction.type << "|" << setw(12) << transaction.date << endl;

    cout << "Enter new description (or press Enter to keep current): ";
    cin.ignore(); 
    string newDescription;
    getline(cin, newDescription);
    if (!newDescription.empty()) {
        transaction.description = newDescription;
    }

    cout << "Enter new amount (or press Enter to keep current): ";
    string amountStr;
    getline(cin, amountStr);
    if (!amountStr.empty()) {
        transaction.amount = stod(amountStr);
    }

    cout << "Enter new type (1 for Credit, 2 for Debit, or press Enter to keep current): ";
    string typeChoiceStr;
    getline(cin, typeChoiceStr);
    if (!typeChoiceStr.empty()) {
        int typeChoice = stoi(typeChoiceStr);
        transaction.type = (typeChoice == 1) ? "Credit" : "Debit";
    }

    cout << "Enter new date (YYYY-MM-DD, or press Enter to keep current): ";
    string newDate;
    getline(cin, newDate);
    if (!newDate.empty() && validateDate(newDate)) {
        transaction.date = newDate;
    } else if (!newDate.empty()) {
        cout << "Invalid date format. Keeping current date.\n";
    }

    cout << "Transaction updated successfully.\n";
}

// Function to view the current balance
void viewBalance(const vector<Transaction>& transactions) {
    double balance = 0.0;

    for (const auto& transaction : transactions) {
        if (transaction.type == "Credit") {
            balance += transaction.amount;
        } else if (transaction.type == "Debit") {
            balance -= transaction.amount;
        }
    }

    cout << "Current Balance: $" << fixed << setprecision(2) << balance << endl;
}

// Function to search transactions
void searchTransactions(const vector<Transaction>& transactions) {
    int searchChoice;
    cout << "Search by: 1. Description  2. Amount  3. Date: ";
    cin >> searchChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (searchChoice) {
        case 1: {
            string searchDescription;
            cout << "Enter description to search for: ";
            getline(cin, searchDescription);
            cout << "\nTransactions matching description '" << searchDescription << "':\n";
            for (const auto& transaction : transactions) {
                if (transaction.description.find(searchDescription) != string::npos) {
                    cout << left << setw(8) << transaction.index << "|" << setw(30) << transaction.description << "|"
                         << setw(10) << fixed << setprecision(2) << transaction.amount << "|"
                         << setw(10) << transaction.type << "|" << setw(12) << transaction.date << endl;
                }
            }
            break;
        }
        case 2: {
            double searchAmount;
            cout << "Enter amount to search for: ";
            cin >> searchAmount;
            cout << "\nTransactions matching amount $" << fixed << setprecision(2) << searchAmount << ":\n";
            for (const auto& transaction : transactions) {
                if (transaction.amount == searchAmount) {
                    cout << left << setw(8) << transaction.index << "|" << setw(30) << transaction.description << "|"
                         << setw(10) << fixed << setprecision(2) << transaction.amount << "|"
                         << setw(10) << transaction.type << "|" << setw(12) << transaction.date << endl;
                }
            }
            break;
        }
        case 3: {
            string searchDate;
            cout << "Enter date to search for (YYYY-MM-DD): ";
            getline(cin, searchDate);
            if (validateDate(searchDate)) {
                cout << "\nTransactions on date '" << searchDate << "':\n";
                for (const auto& transaction : transactions) {
                    if (transaction.date == searchDate) {
                        cout << left << setw(8) << transaction.index << "|" << setw(30) << transaction.description << "|"
                             << setw(10) << fixed << setprecision(2) << transaction.amount << "|"
                             << setw(10) << transaction.type << "|" << setw(12) << transaction.date << endl;
                    }
                }
            } else {
                cout << "Invalid date format.\n";
            }
            break;
        }
        default:
            cout << "Invalid choice.\n";
            break;
    }
}

// Function to clear all transactions
void clearTransactions(vector<Transaction>& transactions) {
    transactions.clear();
    cout << "All transactions deleted.\n";
}

// Main function
int main() {
    vector<Transaction> transactions;

    loadTransactions(transactions);

    int choice;
    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Add Transaction\n";
        cout << "2. View Transactions\n";
        cout << "3. View Transactions by Date\n";
        cout << "4. Delete Transaction\n";
        cout << "5. View Balance\n";
        cout << "6. Edit Transaction\n";
        cout << "7. Search Transactions\n";
        cout << "8. Save Transactions\n";
        cout << "9. Clear Transactions\n";
        cout << "10. Load Transactions\n";
        cout << "11. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice) || choice < 1 || choice > 11) {
            cout << "Invalid choice. Please enter a number between 1 and 11.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: addTransaction(transactions); break;
            case 2: viewTransactions(transactions); break;
            case 3: viewTransactions(transactions, true); break; // Sort by date
            case 4: deleteTransaction(transactions); break;
            case 5: viewBalance(transactions); break;
            case 6: editTransaction(transactions); break;
            case 7: searchTransactions(transactions); break;
            case 8: saveTransactions(transactions); break;
            case 9: clearTransactions(transactions); break;
            case 10: loadTransactions(transactions); break;
            case 11: cout << "Exiting program.\n"; return 0;
            default: cout << "Invalid choice.\n"; break;
        }
    }

    return 0;
}
