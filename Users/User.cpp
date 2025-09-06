#include "User.h"


User::User(const std::string& username, const std::string& password, const Date& registrationDate):
	username(username),
	password(password),
	registrationDate(registrationDate),
	lastAccess(registrationDate)
{
}

User::User(const std::string& username, const std::string& password, const Date& registrationDate, const Date& lastAccess):
	username(username),
	password(password),
	registrationDate(registrationDate),
	lastAccess(lastAccess)
{
}

void User::printFiltered() const
{
	std::cout << "Type: " << this->getType() << ", "
		<< "Username: " << this->getUsername() << ", "
		<< "Registration date: ";
	this->getRegistrationDate().print(std::cout);
	std::cout << ", "
		<< "Last Access: ";
	this->getLastAccess().print(std::cout);
}
