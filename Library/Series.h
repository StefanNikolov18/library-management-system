#pragma once
#include "LibraryItem.h"
#include "Book.h"
#include "Periodical.h"

/// @class Series
/// @brief Represents a series as a derived class from both Book and Periodical.
class Series : public Book, public Periodical
{
public:
    /// @brief Default constructor.
    Series() = default;

    /// @brief Constructor with parameters to initialize all members.
    /// @param genre Genre.
    /// @param title Title.
    /// @param publisher Publisher.
    /// @param rating Rating.
    /// @param description Description.
    /// @param copiesCount Number of copies.
    /// @param author Author.
    /// @param year Year.
    /// @param keywords Keywords.
    /// @param ISBN ISBN number.
    /// @param date Date (year and month).
    /// @param issueNum Issue number.
    /// @param articles Vector of articles.
    /// @param ISSN ISSN number.
    Series(const GENRE& genre, const std::string& title,
        const std::string& publisher, double rating,
        const std::string& description,
        const size_t copiesCount,
        const std::string& author,
        const unsigned year,
        const std::string& keywords,
        const std::string& ISBN,
        const YearAndMonth& date, const size_t issueNum,
        const std::vector<Article>& articles,
        const std::string& ISSN);

    /// @brief Prints information about the series.
    /// @brief Format: Series|Genre|Title|publisher|rating|description|copies|author|year|keywords|ISBN or N/A
    /// @brief |year|month|issueNum|cntArticles|articles...|ISSN or N/A
    /// @param os Output stream.
    void print(std::ostream& os) const override;

    /// @brief Reads information about the series from input stream.
    /// @param is Input stream.
    void read(std::istream& is) override;

    /// @brief Clones the current object.
    /// @return Pointer to a copy of the object.
    LibraryItem* clone() const override {
        return new Series(*this);
    }

    /// @brief Returns the year (uses Book::getYear()).
    /// @return Year.
    const unsigned getYear() const override { return Book::getYear(); }

    /// @brief Returns the author (uses Book::getAuthor()).
    /// @return Author.
    const std::string& getAuthor() const override { return Book::getAuthor(); }

    /// @brief Returns the ISBN number (uses Book::getISBN()).
    /// @return ISBN.
    const std::string& getIsbnIssn() const override { return Book::getISBN(); }

    /// @brief Prints filtered information.
    /// @param os Output stream.
    void printFiltered(std::ostream& os = std::cout) const override;

    /// @brief Returns the type of the library item.
    /// @return String "Series".
    std::string getType() const override { return "Series"; }
};

