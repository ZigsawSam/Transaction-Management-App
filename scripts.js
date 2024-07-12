let transactions = [];

// Function to show a specific section and hide others
function showSection(sectionId) {
    const sections = document.querySelectorAll('.section');
    sections.forEach(section => {
        section.classList.add('hidden');
    });
    document.getElementById(sectionId).classList.remove('hidden');
}

// Initialize by showing the add transaction section when the DOM is fully loaded
document.addEventListener('DOMContentLoaded', () => {
    showSection('add-transaction');
});

// Function to add a transaction
function addTransaction() {
    const description = document.getElementById('add-description').value;
    const amount = parseFloat(document.getElementById('add-amount').value);
    const type = document.getElementById('add-type').value;
    const date = document.getElementById('add-date').value;

    if (description && amount && type && date) {
        const newTransaction = {
            description,
            amount,
            type,
            date,
            index: transactions.length // Simple index based on array length
        };
        transactions.push(newTransaction);
        clearAddTransactionForm();
        alert("Transaction added successfully!");
    } else {
        alert("Please fill in all fields.");
    }
}

// Function to clear the add transaction form
function clearAddTransactionForm() {
    document.getElementById('add-description').value = '';
    document.getElementById('add-amount').value = '';
    document.getElementById('add-type').value = 'Credit';
    document.getElementById('add-date').value = '';
}

// Function to view all transactions
function viewTransactions() {
    const transactionsList = document.getElementById('transactions-list');
    transactionsList.innerHTML = ''; // Clear existing list

    transactions.forEach(transaction => {
        const li = document.createElement('li');
        li.textContent = `${transaction.description} - $${transaction.amount} (${transaction.type}) on ${transaction.date}`;
        transactionsList.appendChild(li);
    });

    showSection('view-transactions');
}

// Function to edit a transaction
function editTransaction() {
    const index = parseInt(document.getElementById('edit-index').value);
    const description = document.getElementById('edit-description').value;
    const amount = parseFloat(document.getElementById('edit-amount').value);
    const type = document.getElementById('edit-type').value;
    const date = document.getElementById('edit-date').value;

    if (index >= 0 && index < transactions.length && description && amount && type && date) {
        transactions[index] = { description, amount, type, date, index };
        alert("Transaction updated successfully!");
        clearEditTransactionForm();
    } else {
        alert("Invalid index or fields.");
    }
}

// Function to clear the edit transaction form
function clearEditTransactionForm() {
    document.getElementById('edit-index').value = '';
    document.getElementById('edit-description').value = '';
    document.getElementById('edit-amount').value = '';
    document.getElementById('edit-type').value = 'Credit';
    document.getElementById('edit-date').value = '';
}

// Function to delete a transaction
function deleteTransaction() {
    const index = parseInt(document.getElementById('delete-index').value);
    
    if (index >= 0 && index < transactions.length) {
        transactions.splice(index, 1); // Remove the transaction
        alert("Transaction deleted successfully!");
        clearDeleteTransactionForm();
    } else {
        alert("Invalid index.");
    }
}

// Function to clear the delete transaction form
function clearDeleteTransactionForm() {
    document.getElementById('delete-index').value = '';
}

// Function to view balance
function displayBalance() {
    const balance = transactions.reduce((total, transaction) => {
        return transaction.type === 'Credit' ? total + transaction.amount : total - transaction.amount;
    }, 0);

    document.getElementById('balance-display').textContent = `Current Balance: $${balance.toFixed(2)}`;
}
