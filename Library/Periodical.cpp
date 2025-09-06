#include "Periodical.h"

Periodical::Periodical(const GENRE& genre, const std::string& title,
	const std::string& publisher, double rating, const std::string& description,
	const size_t copiesCount, const YearAndMonth& date, const size_t issueNum,
	const std::vector<Article>& articles, const std::string& ISSN):
	LibraryItem(genre,title,publisher,rating,description,copiesCount),
	date(date),
	issueNum(issueNum),
	articles(articles),
	ISSN(ISSN)
{
}

void Periodical::print(std::ostream& os) const
{
	printLibraryItem(os);					//LibraryItem

	os << getDate().year << '|'				//year
		<< getDate().month << '|'			//month
		<< getIssueNum() << '|'				//issueNum
		<< this->articles.size() << '|'; //size

	for (size_t i = 0; i < articles.size();++i) { // Articles
		os << articles.at(i).title << '|'
			<< articles.at(i).author << '|'
			<< articles.at(i).keywords << '|';
	}


	if (hasISSN())				//ISSN
		os << getISSN();
	else
		os << "N/A";
}

void Periodical::read(std::istream& is)
{
	readLibraryItem(is);			//LibraryItem

	unsigned year, month;

	if (is.peek() == '|')
		is.ignore();

	is >> year;
	is.ignore(); //|
	is >> month;

	YearAndMonth d(year, month);		//YearAndMonth
	this->setYearAndMonth(d);

	if (is.peek() == '|')
		is.ignore();
	size_t num;
	is >> num;
	this->setIssueNum(num);			//IssueNumber
	is.ignore();

	is >> num;
	is.ignore();
	
	std::vector<Article> Articles(num);			//Articles
	for (size_t i = 0; i < Articles.size();++i) {
		std::getline(is, Articles[i].title, '|');
		std::getline(is, Articles[i].author, '|');
		std::getline(is, Articles[i].keywords, '|');
	}

	this->articles = Articles;

	

	std::string temp{};			//ISSN
	std::getline(is, temp);
	if (temp != "N/A")
		setISSN(temp);
	else
		setISSN();
		
}

void Periodical::printFiltered(std::ostream& os) const
{
	os << "[" << this->getUniqueID() << "] "
		<< "Type: " << this->getType() << ", "
		<< "\"" << this->getTitle() << "\""
		<< " (" << this->date.year << "/" << this->date.month << "), "
		<< "Issue: " << this->issueNum << ", "
		<< "Genre: " << genreToString(this->getGenre())
		<< ", Copies: " << this->getCopiesCNT()
		<< ", Rating: " << this->getRating() << "/10\n";
	os << "Description: " << this->getDescription() << '\n';
}

