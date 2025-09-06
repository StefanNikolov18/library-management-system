#include "Reader.h"


Reader::Reader(const std::string& username, const std::string& password,
	const Date& registrationDate, const std::vector<Borrow>& borrowingItems):
	User(username,password,registrationDate),
	borrowingItems(borrowingItems)
{
}

//Type = {Book,Periodical,Series}
void Reader::print(std::ostream& os) const	// Format: username | password | regDate | lastAccess | sizeBorrowinItems \n
											//         Type |LibraryItemSpecs|borrowDate1|returnDate1\n
											//         ...
{
	os << getType() << '|'
		<< this->getUsername() << '|'
		<< this->getPassword() << '|';
	this->getRegistrationDate().print(os);
	os << '|';
	this->getLastAccess().print(os);
	os << '|';

	os << borrowingItems.size();

	if (!borrowingItems.empty()) {
		for (const Borrow& br : borrowingItems) {
			os << '|'; 
			os << br;  
		}
	}
}

void Reader::read(std::istream& is)
{
	std::string tempStr; //Temp stoinosti
	Date tempDate;

	std::getline(is, tempStr, '|');			//User
	this->setUsername(tempStr);				//..
											//..
	std::getline(is, tempStr, '|');
	this->setPassword(tempStr);

	tempDate.read(is);
	this->setRegistrationDate(tempDate);

	if (is.peek() == '|')
		is.ignore();

	tempDate.read(is);
	this->setLastAccessDate(tempDate);		//..User.end()

	if (is.peek() == '|')
		is.ignore();

	size_t n = 0;			//size Borrow
	is >> n;

	if (is.peek() == '\n')
		is.ignore();

	std::vector<Borrow> temp(n);

	for (Borrow& br : temp)
		is >> br;

	
	
	this->borrowingItems = temp;
	

	
}



bool Reader::hasThisBorrowingList(const std::string& title) const
{
	for (size_t i = 0; i < borrowingItems.size();++i) {
		if (borrowingItems[i].getItem()->getTitle() == title) {
			return true;
		}
	}

	return false;
}

void Reader::removeBorrow(const std::string& title)
{
	for (size_t i = 0; i < borrowingItems.size();++i) {
		if (borrowingItems[i].getItem()->getTitle() == title) {
			borrowingItems.erase(borrowingItems.begin() + i);
			std::cout << "Borrowing Item was succesfully removed!\n";
			return;
		}
		
	}
}

bool Reader::hasThisInBorrowingListID(const unsigned ID) const
{
	for (size_t i = 0;i < borrowingItems.size();++i) {
		if (borrowingItems[i].getItem()->getUniqueID() == ID) {
			return true;
		}
	}
	return false;
}

bool Reader::hasBorrowingOverdue(const Date& today) const
{
	for (size_t i = 0; i < borrowingItems.size();++i) {
		if (borrowingItems[i].getReturningDate() > today) {
			return true;
		}
	}
	return false;
}

bool Reader::InactiveUserBorrowing(const Date& today) const
{
	Date threeMonthsAgo = today;
	int newMonth = today.getMonth() - 3;
	int newYear = today.getYear();

	if (newMonth <= 0) {
		newMonth += 12;
		--newYear;
	}

	threeMonthsAgo = Date(today.getDay(), newMonth, newYear);


	for (size_t i = 0; i < borrowingItems.size();++i) {
		if(this->borrowingItems[i].getReturningDate() >= threeMonthsAgo)
			return false;
	}

	return true;
}


