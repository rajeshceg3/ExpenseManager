#include "Expense.h" // Assuming Expense.h is in the include directory, adjust path if necessary
#include <ostream>
#include <iomanip> // For std::setfill and std::setw

namespace expense_tracker {

// Implementation for the Date ostream operator
std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << date.year << "-"
       << std::setfill('0') << std::setw(2) << date.month << "-"
       << std::setfill('0') << std::setw(2) << date.day;
    return os;
}

// Implementations for Expense struct methods would go here if there were any
// For now, it mostly has a constructor defined in the header.

} // namespace expense_tracker
