#define _CRT_SECURE_NO_WARNINGS
#include "Date.h"
#include <iostream>
#include <ctime>

Date::Date(int d, int m, int y):
	day(d),
	month(m),
	year(y)
{
	if (!this->isValidDate())
		throw std::invalid_argument("Invalid Date!");
}

bool Date::isValidDate() const
{
	
	if(this->year < 1900 || this->year > 2025)
		return false;

	if (this->month < 1 || this->month > 12)
		return false;

	unsigned MonthsInDays[12] =
		{ 31,28,31,30,31,30,31,31,30,31,30,31 };

	if (this->year % 4 == 0 && (this->year % 100 != 0 || this->year % 400 == 0))
		MonthsInDays[1] += 1;

	return this->day >= 1 && (this->day <= MonthsInDays[this->month - 1]);
}

void Date::print(std::ostream& os) const //day.month.year
{
	os << day << '.' << month << '.' << year;
}

void Date::read(std::istream& is) 
{
	is >> this->day;
	is.ignore();
	is >> this->month;
	is.ignore();
	is >> this->year;
}

Date Date::addDays(int days) const {
	
	static const int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	int d = this->day;
	int m = this->month;
	int y = this->year;

	auto isLeap = [](int year) {
		return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
	};

	d += days;

	while (true) {
		int dim = daysInMonth[m - 1];
		
		if (m == 2 && isLeap(y)) {
			dim = 29;
		}

		if (d <= dim) break;  

		d -= dim;
		m++;
		if (m > 12) {
			m = 1;
			y++;
		}
	}

	return Date(d, m, y);
}


Date getTodayDate()
{
	time_t now = time(0);
	tm* t = localtime(&now);
	return Date(t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
}

bool operator<(const Date& lhs, const Date& rhs)
{
	if (lhs.getYear() != rhs.getYear())
		return lhs.getYear() < rhs.getYear();
	if (lhs.getMonth() != rhs.getMonth())
		return lhs.getMonth() < rhs.getMonth();
	return lhs.getDay() != rhs.getDay();
}

bool operator>(const Date& lhs, const Date& rhs)
{
	return rhs < lhs;
}

bool operator<=(const Date& lhs, const Date& rhs)
{
	return !(lhs > rhs);
}

bool operator>=(const Date& lhs, const Date& rhs)
{
	return !(lhs < rhs);
}
