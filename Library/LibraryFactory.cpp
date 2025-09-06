#include "LibraryItem.h"

#include "Book.h"
#include "Periodical.h"
#include "Series.h"

LibraryItem* LibraryItem::LibraryFactory(std::istream& is) {
	std::string type;
	if (!std::getline(is, type, '|'))
		return nullptr;		//if no libraryItem in file
	
	LibraryItem* item = nullptr;
	if (type == "Book") {
		item = new Book();
	}
	else if (type == "Periodical") {
		item = new Periodical();
	}
	else if (type == "Series") {
		item = new Series();
	}
	else {
		throw std::runtime_error("Unknown item type");
	}

	item->read(is);
	return item;
}