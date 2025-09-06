#include "Borrow.h"

Borrow::Borrow(const LibraryItem* item, const Date& b, const Date& r) :
	item(item ? item->clone() : nullptr), borrowing(b), returning(r) {};

Borrow::~Borrow() noexcept { 
	delete item; 
}

Borrow::Borrow(const Borrow& other):
	Borrow(other.item, other.borrowing, other.returning) 
{
}

Borrow& Borrow::operator=(const Borrow& other)
{
	if (this != &other) {
		if (other.item) {
			LibraryItem* temp = other.item->clone();//exception?
			delete item;
			item = temp;

		}
		else {
			delete item;
			item = nullptr;

		}

		borrowing = other.borrowing;
		returning = other.returning;

	}
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Borrow& obj) {
	obj.item->print(os);
	os << '\n';
	obj.borrowing.print(os);
	os << '|';
	obj.returning.print(os);
	return os;
}

std::istream& operator>>(std::istream& is, Borrow& obj) {

	if (is.peek() == '|' || is.peek() == '\n') //for file system
		is.ignore();

		
	LibraryItem* temp = LibraryItem::LibraryFactory(is);		// Type and LibraryItemSpecs

	delete obj.item; // delete prev
	obj.item = temp; //set now

	if (is.peek() == '\n')
		is.ignore();

	obj.borrowing.read(is);				//borrowing
	if (is.peek() == '|')
		is.ignore();
	obj.returning.read(is);				//returning

	return is;
}
