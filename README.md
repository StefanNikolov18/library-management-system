# 📚 Library Management System

## Overview
This project implements an **interactive library information system** that manages books, periodicals(newslettters), and series, as well as readers and administrators.  
The system stores data in files, supports authentication, and provides commands for searching, adding, editing, and borrowing items.  

It is designed to handle **large amounts of data** efficiently by not loading all records into memory at once.

---

## ✨ Features
- **Library items**  
  - Books (with author, title, genre, description, publisher, year, ISBN, etc.)  
  - Periodicals (with issue number, articles, ISSN, etc.)  
  - Series (combines book + periodical features)  
  - Tracks available copies of each item  
- **Users**  
  - Readers (with borrowed items history, deadlines, overdue checks)  
  - Administrators (with email and management permissions)  
- **Authentication**  
  - `login`, `logout`, user roles (reader / admin)  
- **Commands**  
  - Display all items with pagination (`books all`, `series all`, `list all`)  
  - Search with filtering, sorting, and limiting (`find`, `list info <isbn>`)  
  - Manage users (`user add`, `user remove`, `user change`, `user find`)  
  - Manage items (`add`, `remove`, `change`)  
  - Borrowing system (`take <ID>`, `return <ID>`)  
  - `help` and `exit` commands  

---

## 🗂 Data Storage
- Data is stored in **files** (custom text format).  
- At program startup, data is **loaded and validated**.  
- If no data is found, the system initializes with a default admin account:  
  - **Username:** `admin`  
  - **Password:** `i<3c++`  

---

## 🛠 Commands
### 🔑 Authentication
- `login` → enter username + password  
- `logout` → leave the system  
- `exit` → close the program  

### 📖 Library Items
- `books all` / `series all` / `newsletters all` / `list all`  
- `list info <isbn_value>` → detailed info  
- `books find <option> <value> [sort <key> [asc|desc]] [top <val>]`  
- `series find ...`, `newsletters find ...`, `list find ...`  

### 👤 Users
- `user add <user> <password> [admin]`  
- `user remove <user>`  
- `user change [<name>]`  
- `user find <option> <value>`  

### 📦 Borrow & Return
- `take <ID>` → borrow item  
- `return <ID>` → return borrowed item  

---

## 🚀 Usage
1. Clone the repository:
   ```bash
   git clone https://github.com/StefanNikolov18/library-management-system.git
   cd library-management-system
