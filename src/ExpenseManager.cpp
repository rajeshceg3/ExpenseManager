#include "ExpenseManager.h"
#include <algorithm> // For std::remove_if if needed, or for sorting later
#include <iostream>  // For viewExpensesSummary and potential error messages
#include <fstream>   // For load/save
#include <sstream>   // For parsing lines in load/save
#include "fmt/core.h"
#include "csv.h" // From fast-cpp-csv-parser

// For date parsing - will use the 'date' library once linked.
// For now, a simple placeholder or direct parsing.
#include "date/date.h" // Assuming this will be available via Conan

namespace expense_tracker {

ExpenseManager::ExpenseManager() {
    // Initialize data_filename_, perhaps with a default
    // For now, it can be set when load/save are called.
}

void ExpenseManager::addExpense(const Expense& expense) {
    expenses_.push_back(expense);
}

bool ExpenseManager::deleteExpense(size_t index) {
    if (index < expenses_.size()) {
        expenses_.erase(expenses_.begin() + index);
        return true;
    }
    return false; // Index out of bounds
}

const std::vector<Expense>& ExpenseManager::getAllExpenses() const {
    return expenses_;
}

// Placeholder for viewExpensesSummary - will be detailed later
void ExpenseManager::viewExpensesSummary() const {
    if (expenses_.empty()) {
        std::cout << "No expenses recorded." << std::endl;
        return;
    }
    std::cout << "--- All Expenses ---" << std::endl;
    for (size_t i = 0; i < expenses_.size(); ++i) {
        const auto& exp = expenses_[i];
        std::cout << i << ". "
                  << exp.date << " | "
                  << exp.description << " | $"
                  << exp.amount << " | "
                  << exp.category << " | "
                  << (exp.transaction_type == TransactionType::CREDIT ? "Credit" : "Cash")
                  << std::endl;
    }
    std::cout << "--------------------" << std::endl;
}


// Date parsing helper: "YYYY-MM-DD"
Date ExpenseManager::parseDateString(const std::string& dateStr) const {
    Date date{};
    if (dateStr.length() == 10 && dateStr[4] == '-' && dateStr[7] == '-') {
        std::string Y = dateStr.substr(0, 4);
        std::string M = dateStr.substr(5, 2);
        std::string D = dateStr.substr(8, 2);
        try {
            date.year = std::stoi(Y);
            date.month = std::stoi(M);
            date.day = std::stoi(D);

            // Basic validation, can be improved with date library
            if (date.month < 1 || date.month > 12 || date.day < 1 || date.day > 31) {
                 // Invalid date, return a default or throw an error
                 // For now, returning a zeroed date.
                 return Date{};
            }
        } catch (const std::invalid_argument& ia) {
            // std::cerr << "Invalid date string format: " << dateStr << std::endl;
            return Date{}; // Return a default/invalid date
        } catch (const std::out_of_range& oor) {
            // std::cerr << "Date string value out of range: " << dateStr << std::endl;
            return Date{}; // Return a default/invalid date
        }
    }
    return date; // Return potentially zeroed date if format is wrong
}


// Implementations for loadExpenses, saveExpenses, and other summarization functions
// will be added in subsequent steps.

// For now, basic implementations of summarization functions returning empty vectors
// or delegating to viewExpensesSummary to allow compilation.

std::vector<Expense> ExpenseManager::getExpensesByDay(const Date& date) const {
    std::vector<Expense> result;
    // Logic to filter expenses by day
    for (const auto& exp : expenses_) {
        if (exp.date.year == date.year && exp.date.month == date.month && exp.date.day == date.day) {
            result.push_back(exp);
        }
    }
    return result;
}

std::vector<Expense> ExpenseManager::getExpensesByMonth(int month, int year) const {
    std::vector<Expense> result;
    // Logic to filter expenses by month and year
    for (const auto& exp : expenses_) {
        if (exp.date.year == year && exp.date.month == month) {
            result.push_back(exp);
        }
    }
    return result;
}

std::vector<Expense> ExpenseManager::getExpensesByYear(int year) const {
    std::vector<Expense> result;
    // Logic to filter expenses by year
    for (const auto& exp : expenses_) {
        if (exp.date.year == year) {
            result.push_back(exp);
        }
    }
    return result;
}

std::vector<Expense> ExpenseManager::getExpensesByDateRange(const Date& startDate, const Date& endDate) const {
    std::vector<Expense> result;
    // Logic to filter expenses by date range.
    // This requires comparing Date objects. For simplicity, we'll assume year/month/day comparison.
    // A more robust solution would convert Dates to a comparable format (e.g., days since epoch).
    for (const auto& exp : expenses_) {
        // Simple comparison: (Y,M,D) tuple comparison
        bool afterOrOnStart = (exp.date.year > startDate.year) ||
                              (exp.date.year == startDate.year && exp.date.month > startDate.month) ||
                              (exp.date.year == startDate.year && exp.date.month == startDate.month && exp.date.day >= startDate.day);

        bool beforeOrOnEnd = (exp.date.year < endDate.year) ||
                             (exp.date.year == endDate.year && exp.date.month < endDate.month) ||
                             (exp.date.year == endDate.year && exp.date.month == endDate.month && exp.date.day <= endDate.day);

        if (afterOrOnStart && beforeOrOnEnd) {
            result.push_back(exp);
        }
    }
    return result;
}


// Implementations for loadExpenses and saveExpenses will be added in the data persistence step.
// For now, dummy implementations to allow compilation:
bool ExpenseManager::loadExpenses(const std::string& filename) {
    std::string filepath = "data/" + filename;
    expenses_.clear(); // Clear existing expenses before loading

    try {
        // Using CSVReader: 5 columns, trim whitespace around fields.
        // The library handles quoted fields by default.
        io::CSVReader<5, io::trim_chars<' '>> in(filepath);

        // Read the header row and expect these names.
        // ignore_extra_column allows the CSV to have more columns than we read, though we expect 5.
        in.read_header(io::ignore_extra_column, "Date", "Description", "Amount", "Category", "Type");

        std::string dateStr, description, category, typeStr;
        double amount;

        while (in.read_row(dateStr, description, amount, category, typeStr)) {
            Date date = parseDateString(dateStr);
            // Check if parseDateString indicated an error (e.g., returned 0,0,0 for y,m,d)
            // and ensure it wasn't intentionally "0000-00-00" or an equivalent "zero" date string.
            if (date.year == 0 && date.month == 0 && date.day == 0 && !dateStr.empty() && dateStr != "0-00-00" && dateStr != "0000-00-00") {
                std::cerr << "Warning: Could not parse date string: '" << dateStr << "'. Skipping row." << std::endl;
                continue;
            }

            TransactionType transactionTypeVal;
            if (typeStr == "Credit") {
                transactionTypeVal = TransactionType::CREDIT;
            } else if (typeStr == "Cash") {
                transactionTypeVal = TransactionType::CASH;
            } else {
                std::cerr << "Warning: Unknown transaction type: '" << typeStr << "'. Skipping row." << std::endl;
                continue;
            }

            expenses_.emplace_back(description, amount, date, category, transactionTypeVal);
        }
        // Successfully loaded (or file was empty/header-only, which is also success)
        return true;
    } catch (const io::error::can_not_open_file& e) {
        // This is common on first run or if the file is legitimately missing.
        // Treat as success for application startup, but log it.
        std::cout << "Info: Expense file not found or could not be opened: " << filepath
                  << ". This is normal if running for the first time or if no expenses have been saved." << std::endl;
        return true;
    } catch (const io::error::header_missing& e) {
        std::cerr << "Error: CSV header missing or does not match expected format in file: " << filepath << " - " << e.what() << std::endl;
        return false; // Header mismatch is a more serious structural issue.
    } catch (const std::exception& e) {
        // Catch other CSV parsing errors or general exceptions.
        std::cerr << "Error: Exception while reading CSV file: " << filepath << " - " << e.what() << std::endl;
        return false;
    }
}

bool ExpenseManager::saveExpenses(const std::string& filename) const {
    std::string filepath = "data/" + filename;
    std::ofstream outFile(filepath);

    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file for saving: " << filepath << std::endl;
        return false;
    }

    // Write header
    outFile << "Date,Description,Amount,Category,Type\n"; // CSV header

    for (const auto& exp : expenses_) {
        std::string dateStr = fmt::format("{}-{:02}-{:02}", exp.date.year, exp.date.month, exp.date.day);
        std::string typeStr = (exp.transaction_type == TransactionType::CREDIT) ? "Credit" : "Cash";

        // Format the CSV line using fmt::format.
        // This simple version quotes all string fields.
        // Note: If description or category can contain actual quote characters, they would need to be escaped (e.g., "" for ").
        // This implementation does not handle that level of CSV complexity.
        outFile << fmt::format("\"{}\",\"{}\",{},\"{}\",\"{}\"\n",
                               dateStr,
                               exp.description,
                               exp.amount,
                               exp.category,
                               typeStr);
    }

    outFile.close();
    if (!outFile) { // Check for errors that might have occurred during writes or close.
        std::cerr << "Error: An error occurred while writing to file: " << filepath << std::endl;
        return false;
    }
    return true;
}


} // namespace expense_tracker
