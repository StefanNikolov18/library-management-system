#pragma once
#include "LibraryItem.h"
#include "../Date/Date.h"
#include <vector>

/// @struct YearAndMonth
/// @brief Represents a year and month with validation.
struct YearAndMonth {
	unsigned year;
	unsigned month;

	/// @brief Constructor with validation for year and month.
	/// @param year Year, default is 2024.
	/// @param month Month, default is 1.
	YearAndMonth(const unsigned year = 2024, const unsigned month = 1) :
		year(year),
		month(month)
	{
		Date today = getTodayDate();
		if (year < 1800 || year > today.getYear())
			throw std::invalid_argument("Invalid year!");

		if (!month || month > 12)
			throw std::invalid_argument("Invalid month!");
	}
};

/// @struct Article
/// @brief Represents an article with title, author, and keywords.
struct Article {
	std::string title{};
	std::string author{};
	std::string keywords{};

	/// @brief Default constructor.
	Article() = default;

	/// @brief Constructor with parameters.
	/// @param title Title.
	/// @param author Author.
	/// @param keywords Keywords.
	Article(const std::string& title, const std::string& author, const std::string& keywords) :
		title(title), author(author), keywords(keywords)
	{}
};

/// @class Periodical
/// @brief Represents a periodical as a library item, containing date, issue number, and collection of articles.
class Periodical : public virtual LibraryItem
{
public:
	/// @brief Default constructor.
	Periodical() = default;

	/// @brief Constructor with parameters.
	/// @param genre Genre.
	/// @param title Title.
	/// @param publisher Publisher.
	/// @param rating Rating.
	/// @param description Description.
	/// @param copiesCount Number of copies.
	/// @param date Date (year and month).
	/// @param issueNum Issue number.
	/// @param articles Vector of articles.
	/// @param ISSN ISSN number.
	Periodical(const GENRE& genre, const std::string& title,
		const std::string& publisher, double rating,
		const std::string& description, const size_t copiesCount,
		const YearAndMonth& date, const size_t issueNum,
		const std::vector<Article>& articles,
		const std::string& ISSN);

	/// @brief Prints information about the periodical.
	/// @brief Format: Periodical|Genre|Title|publisher|rating|description|copies|date|issueNum|cntArticles|articles...|ISSN or N/A
	/// @param os Output stream.
	void print(std::ostream& os) const override;

	/// @brief Reads information about the periodical from input stream.
	/// @param is Input stream.
	void read(std::istream& is) override;

	/// @brief Clones the current object.
	/// @return Pointer to a copy of the object.
	LibraryItem* clone() const override {
		return new Periodical(*this);
	}

	/// @brief Returns author (for periodicals returns empty string).
	/// @return Empty string.
	const std::string& getAuthor() const override { return ""; }

	/// @brief Returns ISSN number.
	/// @return ISSN as string.
	const std::string& getIsbnIssn() const override { return ISSN; };

	/// @brief Prints filtered information.
	/// @param os Output stream.
	void printFiltered(std::ostream& os = std::cout) const override;

	/// @brief Returns the type of the library item.
	/// @return String "Periodical".
	std::string getType() const override { return "Periodical"; }

	// getters
	const YearAndMonth& getDate() const { return this->date; }
	const size_t getIssueNum() const { return issueNum; }
	const std::vector<Article>& getArticles() const { return this->articles; }
	const std::string& getISSN() const { return this->ISSN; }

	/// @brief Returns the year of publication.
	/// @return Year.
	const unsigned getYear() const override { return this->date.year; }

	// setters
	void setYearAndMonth(const YearAndMonth& newDate) { this->date = newDate; }
	void setIssueNum(const size_t newIssueNum) { this->issueNum = newIssueNum; }
	void setArticles(const std::vector<Article>& newArticles) { this->articles = newArticles; }
	void setISSN(const std::string& newISSN = "") { this->ISSN = newISSN; }

	/// @brief Checks if there is an ISSN.
	/// @return true if ISSN exists, false otherwise.
	bool hasISSN() const { return this->ISSN != ""; }

protected:

	YearAndMonth date{};               ///< Date of publication.
	size_t issueNum = 0;               ///< Issue number.
	std::vector<Article> articles{};  ///< List of articles.
	std::string ISSN{};                ///< ISSN number.
};
