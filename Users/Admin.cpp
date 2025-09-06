#include "Admin.h"

#include <iostream>

Admin::Admin(const std::string& username, const std::string& password, const Date& registrationDate,
			 const std::string& email):
	User(username,password,registrationDate)
{
	
	if (!isValidEmail(email)) {
		throw std::invalid_argument("Not correct email!");
	}
	this->email = email;
}

void Admin::print(std::ostream& os) const
{
	os << this->getType() << '|'
		<< this->getUsername() << '|'		//User
		<< this->getPassword() << '|';		//.
	//.
	this->getRegistrationDate().print(os);	//.
	os << '|';								//.
	this->getLastAccess().print(os);		//.user.end()

	os << '|';
	os << this->email;					//email

}

void Admin::read(std::istream& is)
{
	std::string tempStr; 
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

	std::getline(is, tempStr);				//email
	this->email = tempStr;

}

bool Admin::isValidEmail(const std::string& email) // |@| == 1 && |.| >= 1 && no spaces
{

	const char* ptr = email.c_str();
	
	int cnt1 = 0; // @
	int cnt2 = 0;
	while (*ptr != '\0') {
		if (*ptr == '@')
			++cnt1;
		else if (*ptr == '.')
			++cnt2;
		if (*ptr == ' ')
			return false;
		++ptr;
	}
	
	return (cnt1 == 1) && (cnt2 >= 1);
}
