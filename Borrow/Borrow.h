#pragma once
#include "../Date/Date.h"
#include "../Library/LibraryItem.h"
#include <fstream>

/// @class Borrow
/// @brief Represents a borrowing of a library item for a specific period.
class Borrow {
private:
    LibraryItem* item = nullptr; ///< Pointer to the borrowed library item.
    Date borrowing{};            ///< Borrowing date.
    Date returning{};            ///< Returning date.

public:
    /// @brief Default constructor.
    Borrow() = default;

    /// @brief Constructor with parameters.
    /// @param item Pointer to the borrowed item.
    /// @param b Borrowing date.
    /// @param r Returning date.
    Borrow(const LibraryItem* item, const Date& b, const Date& r);

    /// @brief Destructor.
    ~Borrow() noexcept;

    /// @brief Copy constructor.
    /// @param other Object to copy from.
    Borrow(const Borrow& other);

    /// @brief Assignment operator.
    /// @param other Object to assign from.
    /// @return Reference to this object.
    Borrow& operator=(const Borrow& other);

    /// @brief Outputs borrowing information to stream.
    /// @param os Output stream.
    /// @param obj Object to output.
    /// @return Reference to the stream.
    friend std::ostream& operator<<(std::ostream& os, const Borrow& obj);

    /// @brief Inputs borrowing information from stream.
    /// @param is Input stream.
    /// @param obj Object to input into.
    /// @return Reference to the stream.
    friend std::istream& operator>>(std::istream& is, Borrow& obj);

    /// @brief Returns pointer to the borrowed library item.
    /// @return Constant pointer to LibraryItem.
    const LibraryItem* getItem() const { return this->item; }

    /// @brief Returns the returning date of the borrowed item.
    /// @return Constant reference to a Date object.
    const Date& getReturningDate() const { return this->returning; }
};
