#include "Command.h"
#include "../System/System.h"
#include <iostream>

#include "../Library/Book.h"
#include "../Library/Periodical.h"
#include "../Library/Series.h"

Command::Command(System& system, const std::string& inputCommand):
	system(system),
	command(inputCommand)
{}

//Execute cammands
void Command::execute()
{

	//prefixes
	const std::string listInfoPrefix = "list info "; //for issn or isbn

	//find by option
	const std::string booksFindPrefix = "books find ";
	const std::string newslettersFindPrefix = "newsletters find ";
	const std::string seriesFindPrefix = "series find ";
	const std::string listFindPrefix = "list find ";

	//user finf
	const std::string userFindPrefix = "user find ";

	//list remove and change
	const std::string listRemovePrefix = "list remove ";
	const std::string listChangePrefix = "list change ";

	//user settings
	const std::string addUserPrefix = "user add ";
	const std::string removeUserPrefix = "user remove ";
	const std::string userChangePrefix = "user change ";

	//borrowing commands
	const std::string takeIDPrefix = "take ";
	const std::string returnIDPrefix = "return ";

	//help
	if (command == "help") { help(); }

	//Login and Logout
	else if (command == "login") { login(); }
	else if (command == "logout") { logout(); }

	//Show LibraryItem
	else if (command == "books all") {	booksAll(); }
	else if (command == "newsletters all") {newslettersAll();}
	else if (command == "series all") { seriesAll(); }
	else if (command == "list all") { ListAll(); }

	//Info for ISBN/ISSN
	else if (startWith(command, listInfoPrefix)) { listInfo(listInfoPrefix); }
		
	
	//List find by option 
	else if (startWith(command, booksFindPrefix)) { findItem(booksFindPrefix,"Book"); }
	else if(startWith(command, newslettersFindPrefix)){ findItem(newslettersFindPrefix,"Periodical"); }
	else if(startWith(command, seriesFindPrefix)){ findItem(seriesFindPrefix,"Series"); }
	else if (startWith(command, listFindPrefix)) { findItem(listFindPrefix); }

	//User find by option
	else if (startWith(command, userFindPrefix)) { findUser(userFindPrefix); }


	//Library Item add
	else if (command == "books add") { BooksAdd(); }
	else if (command == "newsletters add") { NewsletterAdd(); }
	else if (command == "series add") { SeriesAdd(); }

	//List remove and change
	else if (startWith(command, listRemovePrefix)) { ListRemove(listRemovePrefix); }
	else if (startWith(command, listChangePrefix)) { ListChange(listChangePrefix); }

	//add and remove user
	else if (startWith(command, addUserPrefix)) { AddUser(addUserPrefix);}
	else if (startWith(command, removeUserPrefix)) { RemoveUser(removeUserPrefix); }
	
	//change Password command user change pass [who]
	else if (startWith(command, userChangePrefix)) { ChangePassword(userChangePrefix); }

	else if (startWith(command, takeIDPrefix)) { TakeByID(takeIDPrefix); }
	else if (startWith(command, returnIDPrefix)) { ReturnID(returnIDPrefix); }


	//Invalid Command was typed
	else { std::cerr << "Invalid command!\n"; }
}

void Command::help() const
{
	std::cout << "login - User login\n"
		<< "logout - User logout\n"
		<< "books all - show all books in library\n"
		<< "newsletters all - show all newsletters in library\n"
		<< "series all - show all series in library\n"
		<< "list all - show all lobrary items\n"
		<< "list info <ISBN/ISSN> - show information about this ISBN/ISSN value\n"
		<< "type find <option> <option_string> [sort <key> [asc|desc] ] [top <val>] \n"
		<< "        - find item by specs option = {title,author,tag}, sort by ascending or descending and top <number>\n"
		<< "user find <option> <optional_string> - find user by option = {state, ID, name} , state = {overdue,reader,inactive}\n"
		<< "type add - add new item in library\n"
		<< "list remove <id> - remove item from library by ID\n"
		<< "list change <id> - change something about item in library\n"
		<< "user add <user> <password> [Admin] - add user\n"
		<< "user remove <username> - remove user by username\n"
		<< "user change <password> [<name>] - change password\n"
		<< "take <ID> - reader borrow from library\n"
		<< "return <ID> - reader return his borrowing item\n"
		<< "exit - exit from the program\n";

}

//Login and loggout functions
void Command::login()
{
	if (system.username.empty()) {
		std::string tempUser{};
		std::cout << "Enter username: ";
		std::getline(std::cin, tempUser);

		std::string tempPassword{};
		std::cout << "Enter password: ";
		std::getline(std::cin, tempPassword);

		if (system.isInUserAndThisPassword(tempUser, tempPassword)) {
			
			system.loginUser(tempUser);
			std::cout << "Welcome, " << system.username << "!\n";

		}
		else {
			std::cerr << "Invalid username or password!\n";
		}

		checkPointUsers();
	}
	else {
		std::cerr << "A user is already logged in!\n";
	}

	
}
void Command::logout()
{
	if (!system.username.empty()) {
		system.username.clear(); //clear username
		std::cout << "You have been logged off." << std::endl;
	}
	else {
		std::cerr << "No user is currently logged in!\n";
	}
	
}

//List functions
void Command::booksAll() const
{
	system.ListLibraryItems("Book");
}
void Command::newslettersAll() const
{
	system.ListLibraryItems("Periodical");
}
void Command::seriesAll() const
{
	system.ListLibraryItems("Series");
}
void Command::ListAll() const
{
	system.ListAllLibraryItems();
}

//List Info for ISSN or ISBN
void Command::listInfo(const std::string& listInfoPref) const
{

	if (!system.username.empty()) {
		std::string isbn = command.substr(listInfoPref.size());
		if (isbn != "") {
			system.ListLibraryItemInfo(isbn);
		}
		else {
			std::cerr << "ISBN value missing after 'list info' command.\n";
		}
	}
	else {
		std::cerr << "You have to be logged in for info!\n";
	}

}


//Find Item in System Function
void Command::findItem(const std::string& Prefix, const std::string& Type) const
{
	if (system.username.empty()) {
		std::cerr << "You are not logged in to search!\n";
		return;
	}
	
	
	std::string args = command.substr(Prefix.size()); //remove prefix

	// Find <option> (title, author, tag)
	size_t spacePos = args.find(' ');
	if (spacePos == std::string::npos) {
		std::cerr << "Missing search value!\n";
		return;
	}

	std::string option = args.substr(0, spacePos); // option {title,author,tag}
	if (option != "title" && option != "author" && option != "tag") {
		std::cerr << "Invalid option!\n";
		return;
	}

	std::string rest = args.substr(spacePos + 1);         //rest

	// finding "sort"
	size_t sortPos = rest.find("sort");

	std::string option_string;
	if (sortPos != std::string::npos) { //is sort
		option_string = rest.substr(0, sortPos);
		rest = rest.substr(sortPos); 
	}
	else {
		option_string = rest;
		rest = "";
	}

	// remove spaces
	while (!option_string.empty() && option_string.front() == ' ')
		option_string.erase(option_string.begin());
	while (!option_string.empty() && option_string.back() == ' ')
		option_string.pop_back();

	// 
	bool hasSort = false;
	std::string sortKey;
	bool asc = true;
	int topCount = -1;

	if (rest.find("sort") == 0) { //helped
		hasSort = true;

		size_t keyStart = 5; // after "sort "
		size_t keyEnd = rest.find(' ', keyStart);
		if (keyEnd != std::string::npos)
			sortKey = rest.substr(keyStart, keyEnd - keyStart);
		else
			sortKey = rest.substr(keyStart);

		// try to find "asc" or "desc"
		size_t orderPos = rest.find("asc", keyEnd);
		if (orderPos == std::string::npos)
			orderPos = rest.find("desc", keyEnd);

		if (orderPos != std::string::npos) {
			std::string order = rest.substr(orderPos, 4);
			asc = (order == "asc");
		}

		// find for "top N"
		size_t topPos = rest.find("top");
		if (topPos != std::string::npos) {
			size_t numStart = topPos + 4;
			std::string number = rest.substr(numStart);
			topCount = atoi(number.c_str());
		}
	}

	//system.function
	system.findItemByOption(
		Type,             
		option,           
		option_string,    
		hasSort,          
		sortKey,          
		asc,              
		topCount          
	);

	// DEBUG 
	/*std::cout << "Option: " << option << '\n';
	std::cout << "Value: " << option_string << '\n';
	if (hasSort) {
		std::cout << "Sort by: " << sortKey << '\n';
		std::cout << "Order: " << (asc ? "Ascending" : "Descending") << '\n';
	}
	if (topCount != -1)
		std::cout << "Top count: " << topCount << '\n'; }*/
}

// Find User in System Function
void Command::findUser(const std::string& userFindPrefix) const
{
	//is logged user
	if (system.username.empty()) {
		std::cerr << "You are not logged in to search!\n";
		return;
	}

	// has to be admin
	if (!system.isAdmin()) {
		std::cerr << "You don't have permission, because you are not admin\n";
		return;
	}

	std::string args = command.substr(userFindPrefix.size());

	// Find <option> (name, ID, state)
	size_t spacePos = args.find(' ');
	if (spacePos == std::string::npos) {
		std::cerr << "Missing search value!\n";
		return;
	}

	// option {name, ID, state}
	std::string option = args.substr(0, spacePos);
	std::string option_string = args.substr(spacePos + 1);

	if (option != "name" && option != "ID" && option != "state") {
		std::cerr << "Invalid option!\n";
		return;
	}

	// state {overdue, reader, inactive}
	if (option == "state") {
		std::string stateString = option_string;

		if (stateString != "overdue" && stateString != "reader" && stateString != "inactive") {
			std::cerr << "Invalid state Option!\n";
			return;
		}

		system.findUserByState(stateString);
	}
	else if (option == "name") {
		system.findUserByName(option_string);
	}
	else { // option == "ID"
		system.findUsersByID(option_string);
	}
}


//Add Items for stdin and push_back in system
void Command::BooksAdd() const
{
	if (system.username.empty()) {
		std::cerr << "You are not logged in!\n";
	}
	else if (!system.isAdmin()) {
		std::cerr << "You don't have permission, because you are not admin\n";
	}
	else {
		system.LoadAllLibraryItemsInBooks();
		const LibraryItem* book = ReadDataForBook();
		if (book != nullptr) {
			system.AddInBooks(book); //add book on books
			delete book; //AddInBook takes copy clone
			checkPointBooks(); //store all and reset
			std::cout << "Book was stored successfully!" << std::endl;
		}
		else { //book = nullptr is canceled
			std::cout << "The process was canceled!" << std::endl;
		}

	}
}
void Command::NewsletterAdd() const
{
	if (system.username.empty()) {
		std::cerr << "You are not logged in!\n";
	}
	else if (!system.isAdmin()) {
		std::cerr << "You don't have permission, because you are not admin\n";
	}
	else {
		system.LoadAllLibraryItemsInBooks();
		const LibraryItem* periodical = ReadDataForPeriodical();
		if (periodical != nullptr) {
			system.AddInBooks(periodical); //add periodical on books
			delete periodical;
			checkPointBooks(); //store all and reset
			std::cout << "Periodical was stored successfully!" << std::endl;
		}
		else { //newsletter = nullptr is canceled
			std::cout << "The process was canceled!" << std::endl;
		}
	}
}
void Command::SeriesAdd() const
{
	if (system.username.empty()) {
		std::cerr << "You are not logged in!\n";
	}
	else if (!system.isAdmin()) {
		std::cerr << "You don't have permission, because you are not admin\n";
	}
	else {
		system.LoadAllLibraryItemsInBooks();
		const LibraryItem* series = ReadDataForSeries();
		if (series != nullptr) {
			system.AddInBooks(series); //add book on books
			delete series;
			checkPointBooks(); //store all and reset
			std::cout << "Series was stored successfully!" << std::endl;
		}
		else { //series = nullptr is canceled
			std::cout << "The process was canceled!" << std::endl;
		}

	}
}

//Add User function
static bool isValidCommandAddUser(const std::string& command)
{
	const char* ptr = command.c_str();
	int cnt = 0;
	while (*ptr) {
		if (*ptr == ' ')
			++cnt;
		++ptr;
	}

	return (cnt == 3 || cnt == 4);
}

void Command::AddUser(const std::string& addUserPrefix) const
{
	if (system.username.empty()) { //not logged user in system
		std::cerr << "You are not logged in to add user!\n";
	}
	else if (!system.isAdmin()) { //Is Not admin
		std::cerr << "You don't have permission, because you are not admin.\n";
	}
	else {
		if (!isValidCommandAddUser(command)) { //3 or 4 spaces
			std::cerr << "Not invalid Command!\n";
			return;
		}

		std::string specsUser = command.substr(addUserPrefix.size()); //takes add user prefix
		system.addUser(specsUser); //adding
		checkPointUsers();
		
	}

	checkPointUsers();
}

//Remove User
void Command::RemoveUser(const std::string& removeUserPrefix) const
{
	if (system.username.empty()) { //not logged user in system
		std::cerr << "You are not logged in to add user!\n";
	}
	else if (!system.isAdmin()) { //Is Not admin
		std::cerr << "You don't have permission, because you are not admin.\n";
	}
	else {
		std::string specsUser = command.substr(removeUserPrefix.size()); //specsUser is username of the user
		system.removeUser(specsUser);
		checkPointUsers();
	}
}

//Change Password Help Function
static bool isValidPassUser(const std::string& specsPass){
	const char* ptr = specsPass.c_str();
	int cnt = 0;
	while (*ptr) {
		if (*ptr == ' ')
			++cnt;

		++ptr;
	}

	return cnt == 0;
}
static bool isValidPassAdmin(const std::string& specsPass) {
	const char* ptr = specsPass.c_str();
	int cnt = 0;
	while (*ptr) {
		if (*ptr == ' ')
			++cnt;

		++ptr;
	}

	return cnt <= 1;
}
//Change Password function
void Command::ChangePassword(const std::string& userChangePrefix) const
{
	std::string specsPass = command.substr(userChangePrefix.size()); //specsUser is username of the user
	if (system.username.empty()) { //not logged user in system
		std::cerr << "You are not logged in to change password!\n";
	}
	else if (!system.isAdmin()) { //Is User only his password
		
		if (!isValidPassUser(specsPass)) { //zero spaces
			std::cerr << "Cannot change others password!\n";
			return;
		}
		//is Valid
		system.changePassword(system.username, specsPass);
		checkPointUsers();
	}
	else { //can change his and others 
		
		if (!isValidPassAdmin(specsPass)) { //Name and paswword
			std::cerr << "Invalid command!\n";
			return;
		}

		//words
		std::vector<std::string> tokens;
		System::splitsWords(tokens, specsPass);
		
		//0 spaces - change it on himself
		if (tokens.size() == 1)
			system.changePassword(system.username, tokens[0]);
		else { //1 space - on User
			system.changePassword(tokens[1], tokens[0]);
			
		}

		checkPointUsers();
	}
}

void Command::TakeByID(const std::string& takeIDPrefix) const
{
	std::string ID = command.substr(takeIDPrefix.size());
	if (system.username.empty()) { //not logged user in system
		std::cerr << "You are not logged in to take library Item!\n";
	}
	else if (!system.isAdmin()) { 

		system.borrowID(ID);
		checkPointUsers();
	}
	else {
		std::cout << "You are admin;)" << std::endl;
	}

}

void Command::ReturnID(const std::string& returnIDPrefix) const
{
	std::string ID = command.substr(returnIDPrefix.size());
	if (system.username.empty()) { //not logged user in system
		std::cerr << "You are not logged in to return library Item!\n";
	}
	else if (!system.isAdmin()) {

		system.returnID(ID);
		checkPointUsers();
	}
	else {
		std::cout << "You are admin;)" << std::endl;
	}
}

void Command::ListRemove(const std::string& listRemovePrefix) const
{
	if (system.username.empty()) { //not logged user in system
		std::cerr << "You are not logged in to remove from List!\n";
	}
	else if (!system.isAdmin()) { //Is Not admin
		std::cerr << "You don't have permission, because you are not admin.\n";
	}
	else {
		std::string ID = command.substr(listRemovePrefix.size()); //specsUser is username of the user
		system.removeListByID(ID);

	}
}

void Command::ListChange(const std::string& listChangePrefix) const
{
	if (system.username.empty()) { //not logged user in system
		std::cerr << "You are not logged in to change List!\n";
	}
	else if (!system.isAdmin()) { //Is Not admin
		std::cerr << "You don't have permission, because you are not admin.\n";
	}
	else {
		std::string ID = command.substr(listChangePrefix.size()); //specsUser is username of the user
		system.changeListByID(ID);
	}
}


//Read LibraryItems from stdin details function
const LibraryItem* Command::ReadDataForBook() const
{
	std::string input;
	std::string title, publisher, description, author, keywords, isbn;
	double rating = -1;
	int copiesCount = 0;
	unsigned year = 0;
	int genreChoice = -1;
	GENRE genre = GENRE::Unknown;

	std::cout << "Enter book details (type 'cancel' to abort):\n";

	std::cout << "Title: ";						//Title
	std::getline(std::cin, title);
	if (title == "cancel")
		return nullptr;

	std::cout << "Publisher: ";					//Publisher
	std::getline(std::cin, publisher);
	if (publisher == "cancel") return nullptr;

	while (true) {								//Rating
		std::cout << "Rating (0.0 - 9.99): ";
		std::getline(std::cin, input);
		if (input == "cancel") 
			return nullptr;

		rating = atof(input.c_str());//return 0.0
		if (rating < 0.0 || rating >= 10.0)
			std::cout << "Invalid rating. Try again.\n";
		else
			break;
	}

	std::cout << "Description: ";				//Description
	std::getline(std::cin, description);
	if (description == "cancel") 
		return nullptr;

	while (true) {
		std::cout << "Number of copies: ";		//copies
		std::getline(std::cin, input);
		if (input == "cancel") 
			return nullptr;

		
		copiesCount = atoi(input.c_str());
		if (copiesCount < 0)
			std::cout << "Invalid number. Try again.\n";
		else
			break;
		
	}

	std::cout << "Author: ";				//Author
	std::getline(std::cin, author);
	if (author == "cancel") return nullptr;

	while (true) {
		std::cout << "Year of publishing: ";
		std::getline(std::cin, input);
		if (input == "cancel") 
			return nullptr;

		year = atoi(input.c_str());
		if (year < 1900 || year > 2025)
			std::cout << "Invalid year. Try again.\n";
		else
			break;
	}

	std::cout << "Keywords: ";					//keywords
	std::getline(std::cin, keywords);
	if (keywords == "cancel") 
		return nullptr;

	std::cout << "ISBN (optional, can leave empty -> press enter): ";	//ISBN
	std::getline(std::cin, isbn);
	if (isbn == "cancel") 
		return nullptr;

	std::cout << "Select genre:\n";
	const char* genreNames[] = { "Fantasy", "SciFi", "Mystery", "Romance", "Thriller", "Horror", "Biography" };
	for (int i = 0; i < 7; ++i) {
		std::cout << i << ". " << genreNames[i] << '\n';
	}

	
	std::cout << "Genre number: ";
	std::getline(std::cin, input);
	if (input == "cancel") return nullptr;

	genreChoice = atoi(input.c_str());
	if (genreChoice < -1 || genreChoice > 6) {
		std::cout << "Invalid genre.Set to Unknown.\n";
	}
	else {
		genre = static_cast<GENRE>(genreChoice);
	}
	
	
	return new Book(genre, title, publisher, rating, description, copiesCount, author, year, keywords, isbn);
}
const LibraryItem* Command::ReadDataForPeriodical() const
{
	std::string input;
	std::string title, publisher, description, keywords, issn;
	double rating = -1;
	int copiesCount = 0;
	int issueNumber = 0;
	int genreChoice = -1;
	GENRE genre = GENRE::Unknown;

	unsigned year = 0, month = 0;
	

	std::cout << "Enter periodical details (type 'cancel' to abort):\n";

	std::cout << "Title: ";
	std::getline(std::cin, title);
	if (title == "cancel") return nullptr;

	std::cout << "Publisher: ";
	std::getline(std::cin, publisher);
	if (publisher == "cancel") return nullptr;

	while (true) {
		std::cout << "Rating (0.0 - 9.99): ";
		std::getline(std::cin, input);
		if (input == "cancel") return nullptr;

		rating = atof(input.c_str());
		if (rating < 0.0 || rating >= 10.0)
			std::cout << "Invalid rating. Try again.\n";
		else break;
	}

	std::cout << "Description: ";
	std::getline(std::cin, description);
	if (description == "cancel") return nullptr;

	while (true) {
		std::cout << "Number of copies: ";
		std::getline(std::cin, input);
		if (input == "cancel") return nullptr;

		copiesCount = atoi(input.c_str());
		if (copiesCount < 0)
			std::cout << "Invalid number. Try again.\n";
		else break;
	}

	while (true) {
		std::cout << "Publishing year: ";
		std::getline(std::cin, input);
		if (input == "cancel") return nullptr;

		year = atoi(input.c_str());
		if (year < 1900 || year > 2025)
			std::cout << "Invalid year. Try again.\n";
		else break;
	}

	while (true) {
		std::cout << "Publishing month (1-12): ";
		std::getline(std::cin, input);
		if (input == "cancel") return nullptr;

		month = atoi(input.c_str());
		if (month < 1 || month > 12)
			std::cout << "Invalid month. Try again.\n";
		else break;
	}

	YearAndMonth date(year, month);

	while (true) {
		std::cout << "Issue number: ";
		std::getline(std::cin, input);
		if (input == "cancel") return nullptr;

		issueNumber = atoi(input.c_str());
		if (issueNumber < 0)
			std::cout << "Invalid number. Try again.\n";
		else break;
	}

	std::cout << "Keywords: ";
	std::getline(std::cin, keywords);
	if (keywords == "cancel") return nullptr;

	std::cout << "ISSN (optional, press enter to skip): ";
	std::getline(std::cin, issn);
	if (issn == "cancel") return nullptr;

	std::cout << "Select genre:\n";
	const char* genreNames[] = { "Fantasy", "SciFi", "Mystery", "Romance", "Thriller", "Horror", "Biography" };
	for (int i = 0; i < 7; ++i) {
		std::cout << i << ". " << genreNames[i] << '\n';
	}

	std::cout << "Genre number: ";
	std::getline(std::cin, input);
	if (input == "cancel") return nullptr;

	genreChoice = atoi(input.c_str());
	if (genreChoice < -1 || genreChoice > 6) {
		std::cout << "Invalid genre. Set to Unknown.\n";
	}
	else {
		genre = static_cast<GENRE>(genreChoice);
	}

	std::vector<Article> articles;
	size_t cntArticles;

	while (true) {
		std::cout << "How many articles(max 10): ";
		std::getline(std::cin, input);
		if (input == "cancel") return nullptr;

		cntArticles =static_cast<size_t>(atoi(input.c_str()));
		if (cntArticles > 10) {
			std::cout << "Invalid number of articles.Type again.\n";
		}
		else {
			break;
		}
	}
	
	
	std::string artTitle;
	std::string artAuthor;
	std::string artKeyW;
	for (size_t i = 0; i < cntArticles;++i) {
		std::cout << "Enter title: ";
		std::getline(std::cin, artTitle);
		if (artTitle == "cancel") return nullptr;

		std::cout << "Enter author: ";
		std::getline(std::cin, artAuthor);
		if (artAuthor == "cancel") return nullptr;

		std::cout << "Enter keywords: ";
		std::getline(std::cin, artKeyW);
		if (artKeyW == "cancel") return nullptr;


		Article art(artTitle, artAuthor, artKeyW);
		articles.push_back(art);
	}

	return new Periodical(genre, title, publisher, rating, description,
		copiesCount, date, issueNumber, articles, issn);
}
const LibraryItem* Command::ReadDataForSeries() const
{
	std::string input;
	std::string title, publisher, author, description, keywords, ISBN, ISSN;
	double rating = -1;
	size_t copiesCount = 0;
	size_t issueNum = 0;
	int genreChoice = -1;
	GENRE genre = GENRE::Unknown;
	unsigned year = 0, month = 0;

	std::cout << "Enter series details (type 'cancel' to abort):\n";

	std::cout << "Title: ";
	std::getline(std::cin, title);
	if (title == "cancel") return nullptr;

	std::cout << "Publisher: ";
	std::getline(std::cin, publisher);
	if (publisher == "cancel") return nullptr;

	std::cout << "Author: ";
	std::getline(std::cin, author);
	if (author == "cancel") return nullptr;

	while (true) {
		std::cout << "Rating (0.0 - 9.99): ";
		std::getline(std::cin, input);
		if (input == "cancel") return nullptr;

		rating = atof(input.c_str());
		if (rating < 0.0 || rating >= 10.0)
			std::cout << "Invalid rating. Try again.\n";
		else 
			break;
	}

	std::cout << "Description: ";
	std::getline(std::cin, description);
	if (description == "cancel") return nullptr;

	while (true) {
		std::cout << "Number of copies: ";
		std::getline(std::cin, input);
		if (input == "cancel") return nullptr;

		copiesCount = static_cast<size_t>(atoi(input.c_str()));
		if (copiesCount < 0)
			std::cout << "Invalid number. Try again.\n";
		else break;
	}

	while (true) {
		std::cout << "Publishing year: ";
		std::getline(std::cin, input);
		if (input == "cancel") return nullptr;

		year = static_cast<unsigned>(atoi(input.c_str()));
		if (year < 1900 || year > 2025)
			std::cout << "Invalid year. Try again.\n";
		else break;
	}

	while (true) {
		std::cout << "Publishing month (1-12): ";
		std::getline(std::cin, input);
		if (input == "cancel") return nullptr;

		month = static_cast<unsigned>(atoi(input.c_str()));
		if (month < 1 || month > 12)
			std::cout << "Invalid month. Try again.\n";
		else break;
	}

	YearAndMonth date(year, month);

	while (true) {
		std::cout << "Issue number: ";
		std::getline(std::cin, input);
		if (input == "cancel") return nullptr;

		issueNum = static_cast<size_t>(atoi(input.c_str()));
		if (issueNum < 0)
			std::cout << "Invalid number. Try again.\n";
		else break;
	}

	std::cout << "Keywords: ";
	std::getline(std::cin, keywords);
	if (keywords == "cancel") return nullptr;

	std::cout << "ISBN (optional, press enter to skip): ";
	std::getline(std::cin, ISBN);
	if (ISBN == "cancel") return nullptr;

	std::cout << "ISSN (optional, press enter to skip): ";
	std::getline(std::cin, ISSN);
	if (ISSN == "cancel") return nullptr;

	std::cout << "Select genre:\n";
	const char* genreNames[] = { "Fantasy", "SciFi", "Mystery", "Romance", "Thriller", "Horror", "Biography" };
	for (int i = 0; i < 7; ++i) {
		std::cout << i << ". " << genreNames[i] << '\n';
	}

	std::cout << "Genre number: ";
	std::getline(std::cin, input);
	if (input == "cancel") return nullptr;

	genreChoice = atoi(input.c_str());
	if (genreChoice < -1 || genreChoice > 6) {
		std::cout << "Invalid genre. Set to Unknown.\n";
	}
	else {
		genre = static_cast<GENRE>(genreChoice);
	}

	std::vector<Article> articles;
	size_t cntArticles;

	while (true) {
		std::cout << "How many articles(max 10): ";
		std::getline(std::cin, input);
		if (input == "cancel") return nullptr;

		cntArticles = static_cast<size_t>(atoi(input.c_str()));
		if (cntArticles > 10) {
			std::cout << "Invalid number of articles.Type again.\n";
		}
		else {
			break;
		}
	}


	std::string artTitle;
	std::string artAuthor;
	std::string artKeyW;
	for (size_t i = 0; i < cntArticles;++i) {
		std::cout << "Enter title: ";
		std::getline(std::cin, artTitle);
		if (artTitle == "cancel") return nullptr;

		std::cout << "Enter author: ";
		std::getline(std::cin, artAuthor);
		if (artAuthor == "cancel") return nullptr;

		std::cout << "Enter keywords: ";
		std::getline(std::cin, artKeyW);
		if (artKeyW == "cancel") return nullptr;


		Article art(artTitle, artAuthor, artKeyW);
		articles.push_back(art);
	}

	return new Series(genre, title, publisher, rating, description, copiesCount,
		author, year, keywords, ISBN, date, issueNum, articles, ISSN);
}



void Command::checkPointBooks() const
{
	system.saveToFileBooks(); //Before that everything is load 
	system.resetBooks(); //first maxstoring
}

void Command::checkPointUsers() const
{
	system.saveToFileUsers();
}