#include <iostream>
#include <vector>
#include <string>
#include <limits> // Required for std::numeric_limits
#include <cstdio> // For sscanf

#include "ExpenseManager.h" // Assuming ExpenseManager.h is in include/
#include "Expense.h"        // Assuming Expense.h is in include/
#include "fmt/core.h"       // For fmt::print (optional, can use iostream)

// Function to display the main menu
void displayMenu() {
    fmt::print("\n--- Expense Tracker Menu ---\n");
    fmt::print("1. Add Expense\n");
    fmt::print("2. View All Expenses\n");
    fmt::print("3. Delete Expense\n");
    fmt::print("4. Save Expenses to File\n");
    fmt::print("5. Load Expenses from File\n");
    fmt::print("6. Summarize Expenses\n"); // New option
    fmt::print("7. Exit\n");             // Exit is now 7
    fmt::print("Enter your choice: ");
}

// Helper to get valid integer input
int getIntInput() {
    int choice;
    while (!(std::cin >> choice)) {
        fmt::print("Invalid input. Please enter a number: ");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // consume newline
    return choice;
}

// Helper to get valid double input
double getDoubleInput() {
    double value;
    while (!(std::cin >> value)) {
        fmt::print("Invalid input. Please enter a numeric value: ");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // consume newline
    return value;
}

// Helper function to print a collection of expenses
void printExpenses(const std::vector<expense_tracker::Expense>& expenses, const std::string& header) {
    if (expenses.empty()) {
        fmt::print("{}\nNo expenses found.\n", header);
        return;
    }
    fmt::print("{}\n", header);
    fmt::print("--------------------------------------------------------------------\n");
    fmt::print("{:<4} | {:<10} | {:<20} | {:<10} | {:<10} | {:<10}\n", "Idx", "Date", "Description", "Amount", "Category", "Type");
    fmt::print("--------------------------------------------------------------------\n");
    for (size_t i = 0; i < expenses.size(); ++i) {
        const auto& exp = expenses[i];
        std::string dateStr = fmt::format("{}-{:02}-{:02}", exp.date.year, exp.date.month, exp.date.day);
        std::string typeStr = (exp.transaction_type == expense_tracker::TransactionType::CREDIT) ? "Credit" : "Cash";
        fmt::print("{:<4} | {:<10} | {:<20} | {:<10.2f} | {:<10} | {:<10}\n",
                   i, dateStr, exp.description, exp.amount, exp.category, typeStr);
    }
    fmt::print("--------------------------------------------------------------------\n");
}

expense_tracker::Date getDateInput(const std::string& prompt) {
    std::string dateStr;
    int year = 0, month = 0, day = 0; // Initialize to invalid
    while (true) {
        fmt::print(prompt);
        std::getline(std::cin, dateStr);
        if (sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day) == 3) {
            // Basic validation, can be improved (e.g. using date library for validation)
            if (year > 0 && month >= 1 && month <= 12 && day >= 1 && day <= 31) { // Simplified validation
                break;
            }
        }
        fmt::print("Invalid date format or value. Please use YYYY-MM-DD: ");
    }
    return expense_tracker::Date(year, month, day);
}

// Function to handle adding an expense
void addExpenseUI(expense_tracker::ExpenseManager& manager) {
    std::string description, category, dateStr, typeStr;
    double amount;
    int year, month, day;

    fmt::print("Enter description: ");
    std::getline(std::cin, description);

    fmt::print("Enter amount: ");
    amount = getDoubleInput();

    fmt::print("Enter date (YYYY-MM-DD): ");
    std::getline(std::cin, dateStr);
    // Basic parsing for YYYY-MM-DD, can be made more robust
    // sscanf or manual parsing
    if (sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
        fmt::print("Invalid date format. Expense not added.\n");
        return; // Exit if date is bad
    }
    expense_tracker::Date date(year, month, day);


    fmt::print("Enter category: ");
    std::getline(std::cin, category);

    fmt::print("Enter transaction type (Cash/Credit): ");
    std::getline(std::cin, typeStr);
    expense_tracker::TransactionType type = (typeStr == "Credit" || typeStr == "credit") ?
                                            expense_tracker::TransactionType::CREDIT :
                                            expense_tracker::TransactionType::CASH;

    manager.addExpense(expense_tracker::Expense(description, amount, date, category, type));
    fmt::print("Expense added.\n");
}

// Function to handle deleting an expense
void deleteExpenseUI(expense_tracker::ExpenseManager& manager) {
    manager.viewExpensesSummary(); // Show expenses with indices
    if (manager.getAllExpenses().empty()) {
        return;
    }
    fmt::print("Enter index of expense to delete: ");
    int index = getIntInput();
    if (manager.deleteExpense(static_cast<size_t>(index))) {
        fmt::print("Expense deleted.\n");
    } else {
        fmt::print("Invalid index or expense not found.\n");
    }
}

void summarizeByDayUI(expense_tracker::ExpenseManager& manager) {
    fmt::print("--- Summarize by Day ---\n");
    expense_tracker::Date date = getDateInput("Enter date (YYYY-MM-DD): ");
    std::vector<expense_tracker::Expense> expenses = manager.getExpensesByDay(date);
    printExpenses(expenses, fmt::format("--- Expenses for {} ---", fmt::format("{}-{:02}-{:02}", date.year, date.month, date.day)));
}

void summarizeByMonthUI(expense_tracker::ExpenseManager& manager) {
    fmt::print("--- Summarize by Month ---\n");
    fmt::print("Enter year (YYYY): ");
    int year = getIntInput();
    fmt::print("Enter month (MM): ");
    int month = getIntInput();
    if (month < 1 || month > 12) {
        fmt::print("Invalid month. Aborting.\n");
        return;
    }
    std::vector<expense_tracker::Expense> expenses = manager.getExpensesByMonth(month, year);
    printExpenses(expenses, fmt::format("--- Expenses for {}-{:02} ---", year, month));
}

void summarizeByYearUI(expense_tracker::ExpenseManager& manager) {
    fmt::print("--- Summarize by Year ---\n");
    fmt::print("Enter year (YYYY): ");
    int year = getIntInput();
    std::vector<expense_tracker::Expense> expenses = manager.getExpensesByYear(year);
    printExpenses(expenses, fmt::format("--- Expenses for {} ---", year));
}

void summarizeByDateRangeUI(expense_tracker::ExpenseManager& manager) {
    fmt::print("--- Summarize by Date Range ---\n");
    expense_tracker::Date startDate = getDateInput("Enter start date (YYYY-MM-DD): ");
    expense_tracker::Date endDate = getDateInput("Enter end date (YYYY-MM-DD): ");
    // Basic validation: ensure startDate <= endDate
    long startDateVal = startDate.year * 10000 + startDate.month * 100 + startDate.day;
    long endDateVal = endDate.year * 10000 + endDate.month * 100 + endDate.day;
    if (startDateVal > endDateVal) {
        fmt::print("Start date cannot be after end date. Aborting.\n");
        return;
    }
    std::vector<expense_tracker::Expense> expenses = manager.getExpensesByDateRange(startDate, endDate);
    printExpenses(expenses, fmt::format("--- Expenses from {}-{:02}-{:02} to {}-{:02}-{:02} ---",
        startDate.year, startDate.month, startDate.day, endDate.year, endDate.month, endDate.day));
}

void displaySummarizationMenu() {
    fmt::print("\n--- Summarization Menu ---\n");
    fmt::print("1. Summarize by Day\n");
    fmt::print("2. Summarize by Month\n");
    fmt::print("3. Summarize by Year\n");
    fmt::print("4. Summarize by Date Range\n");
    fmt::print("5. Back to Main Menu\n");
    fmt::print("Enter your choice: ");
}

void handleSummarizationMenu(expense_tracker::ExpenseManager& manager) {
    int choice;
    do {
        displaySummarizationMenu();
        choice = getIntInput();
        switch (choice) {
            case 1: summarizeByDayUI(manager); break;
            case 2: summarizeByMonthUI(manager); break;
            case 3: summarizeByYearUI(manager); break;
            case 4: summarizeByDateRangeUI(manager); break;
            case 5: fmt::print("Returning to main menu...\n"); break;
            default: fmt::print("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}

int main() {
    expense_tracker::ExpenseManager manager;
    std::string defaultFilename = "expenses.csv";

    // Attempt to load expenses on startup silently or with a small message
    // manager.loadExpenses(defaultFilename); // This is handled in step 4, but good to note.
    // For now, user explicitly loads.

    int choice;
    do {
        displayMenu();
        choice = getIntInput();

        switch (choice) {
            case 1:
                addExpenseUI(manager);
                break;
            case 2:
                manager.viewExpensesSummary();
                break;
            case 3:
                deleteExpenseUI(manager);
                break;
            case 4:
                if (manager.saveExpenses(defaultFilename)) {
                    fmt::print("Expenses saved to {}\n", defaultFilename);
                } else {
                    fmt::print("Failed to save expenses.\n");
                }
                break;
            case 5:
                if (manager.loadExpenses(defaultFilename)) {
                    fmt::print("Expenses loaded from {}\n", defaultFilename);
                } else {
                    fmt::print("Failed to load expenses.\n");
                }
                break;
            case 6:
                handleSummarizationMenu(manager); // New case
                break;
            case 7:
                fmt::print("Exiting application.\n"); // Exit is now 7
                break;
            default:
                fmt::print("Invalid choice. Please try again.\n");
        }
    } while (choice != 7); // Exit condition is now 7

    return 0;
}
