#ifndef EXPENSE_H
#define EXPENSE_H

#include <string>
#include <chrono> // For date handling, will integrate with the 'date' library later

// Forward declaration for the date library type if needed, or include directly
// For now, using std::chrono::year_month_day as a placeholder for compilation
// We will replace this with the actual type from the 'date' library (date::year_month_day)
// once CMake and Conan are set up to link it.

namespace expense_tracker {

enum class TransactionType {
    CASH,
    CREDIT
};

struct Date {
    int year;
    int month;
    int day;

    // Basic constructor
    Date(int y = 0, int m = 0, int d = 0) : year(y), month(m), day(d) {}

    // Friend function for easy output, useful for debugging
    friend std::ostream& operator<<(std::ostream& os, const Date& date);
};

struct Expense {
    std::string description;
    double amount;
    Date date;
    std::string category;
    TransactionType transaction_type;

    Expense(std::string desc, double amt, Date dt, std::string cat, TransactionType type)
        : description(std::move(desc)), amount(amt), date(dt), category(std::move(cat)), transaction_type(type) {}
};

} // namespace expense_tracker

#endif // EXPENSE_H
