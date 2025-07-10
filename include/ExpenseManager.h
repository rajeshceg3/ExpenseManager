#ifndef EXPENSE_MANAGER_H
#define EXPENSE_MANAGER_H

#include "Expense.h"
#include <vector>
#include <string>
#include <functional> // For std::function in more advanced filtering later if needed

namespace expense_tracker {

class ExpenseManager {
public:
    ExpenseManager();

    // Core operations
    void addExpense(const Expense& expense);
    bool deleteExpense(size_t index); // Returns true if successful, false if index out of bounds
    const std::vector<Expense>& getAllExpenses() const;

    // Data persistence
    bool loadExpenses(const std::string& filename);
    bool saveExpenses(const std::string& filename) const;

    // Summarization (declarations for now, implementation will follow)
    // These might return a new vector of expenses or directly print them
    // For now, let's assume they print and return void for simplicity in this step
    void viewExpensesSummary() const; // Simple view of all expenses
    std::vector<Expense> getExpensesByDay(const Date& date) const;
    std::vector<Expense> getExpensesByMonth(int month, int year) const;
    std::vector<Expense> getExpensesByYear(int year) const;
    std::vector<Expense> getExpensesByDateRange(const Date& startDate, const Date& endDate) const;


private:
    std::vector<Expense> expenses_;
    std::string data_filename_; // To store the default filename

    // Helper for parsing date strings if needed, or can be part of loadExpenses
    Date parseDateString(const std::string& dateStr) const;
};

} // namespace expense_tracker

#endif // EXPENSE_MANAGER_H
