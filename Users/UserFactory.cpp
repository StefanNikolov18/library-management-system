#include "User.h"
#include "Admin.h"
#include "Reader.h"

User* User::UserFactory(std::ifstream& is) {
	std::string type{};
	if (is.peek() == '\n')
		is.ignore();

	if (!std::getline(is, type, '|')) //if no user in file
		return nullptr;

	User* item = nullptr;
	if (type == "Admin") {
		item = new Admin();
		
	}
	else if (type == "Reader") {
		item = new Reader();
		
	}
	else {
		throw std::invalid_argument("Unknown item type in UserFactory!");
	}

	item->read(is);
	return item;
}