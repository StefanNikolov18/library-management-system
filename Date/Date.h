#pragma once
#include <fstream>
#include <iostream>


/// @class Date
/// @brief Represents a simple date with day, month, and year.
class Date
{
public:
    /// @brief Constructor with parameters (default 01.01.2020).
    /// @param d Day.
    /// @param m Month.
    /// @param y Year.
    Date(int d = 1, int m = 1, int y = 2020);

    /// @brief Checks if the date is valid.
    /// @return true if valid, false otherwise.
    bool isValidDate() const;

    /// @brief Adds days to the current date.
    /// @param days Number of days to add.
    /// @return New date with added days.
    Date addDays(int days) const;

    const int getDay() const { return this->day; }
    const int getMonth() const { return this->month; }
    const int getYear() const { return this->year; }

    /// @brief Prints the date in DD.MM.YYYY format.
    /// @param os Output stream.
    void print(std::ostream& os = std::cout) const;

    /// @brief Reads a date from input stream.
    /// @param is Input stream.
    void read(std::istream& is);

private:
    unsigned day;   ///< Day of the month (1-31 depending on the month).
    unsigned month; ///< Month of the year (1-12).
    unsigned year;  ///< Year (e.g. 2025).
};


/// @brief Gets todays date. 
Date getTodayDate();

/// @brief Compares if lhs < rhs.
/// @return true if lhs is earlier than rhs.
bool operator<(const Date& lhs, const Date& rhs);

/// @brief Compares if lhs > rhs.
/// @return true if lhs is later than rhs.
bool operator>(const Date& lhs, const Date& rhs);

/// @brief Compares if lhs <= rhs.
/// @return true if lhs is earlier than or equal to rhs.
bool operator<=(const Date& lhs, const Date& rhs);

/// @brief Compares if lhs >= rhs.
/// @return true if lhs is later than or equal to rhs.
bool operator>=(const Date& lhs, const Date& rhs);