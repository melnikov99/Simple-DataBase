// coursera(final).cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <iomanip>
#include <exception>
#include <sstream>
using namespace std;

class Date {
public: 
	int GetYear() const {
		return year;
	}
	int GetMonth() const {
		return month;
	}
	int GetDay() const {
		return day;
	}
friend Date DateParse(const string& messege);
private:
	int year;
	int month;
	int day;
};

void Print_date(const Date& date) {
	cout << setw(4) << setfill('0') << date.GetYear() << '-'
		<< setw(2) << setfill('0') << date.GetMonth() << '-'
		<< setw(2) << setfill('0') << date.GetDay();
}

class Database {
public:
	set<string> operator[] (const Date& new_date) {
		return events[new_date];
	}

	void add(const Date& new_date, const string& new_event) {
		events[new_date].insert(new_event);
	}

	void delete_one_event(const Date& new_date, const string& new_event) {
		if (events[new_date].count(new_event) != 0) {
			events[new_date].erase(new_event);
			if (events[new_date].count(new_event) == 0)
				cout << "Deleted successfully" << endl;
		}
		else
			cout << "Event not found" << endl;
			
	}

	void delete_some_events(const Date& new_date) {
		cout << "Deleted " << events[new_date].size() << " events" << endl;
		events[new_date].clear();
	
	}

	void Find(const Date& new_date) const {
		if (events.count(new_date) != 0) {
			for (const auto& e : events.at(new_date))
				cout << e << endl;
		}
	}
	
	void Print() const {
		for (const auto& item : events) {
			for (const auto& i : item.second) {
				Print_date(item.first);
				cout << ' ' << i << endl;
			}
		}
	}
private:
	map<Date, set<string>> events;
};

bool operator< (const Date& date1, const Date& date2) {
	if (date1.GetYear() < date2.GetYear())
		return true;
	else 
		if (date1.GetYear() == date2.GetYear()) {
			if (date1.GetMonth() < date2.GetMonth())
				return true;
			else 
				if (date1.GetMonth() == date2.GetMonth()) {
					if (date1.GetDay() < date2.GetDay())
						return true;
				}
		}
	return false;

}

void EnsureNextSymbol(istream& input, const string& error_s) {
	if (input.peek() != '-')
		throw runtime_error(error_s);
}

void GetValueWithMinus(istream& stream, int& value, const string& error_messege) {
	if (isdigit(stream.peek())) {
		stream >> value;
		value *= -1;
	}
	else 
		throw runtime_error(error_messege);
}
Date DateParse(const string& messege) {
	stringstream stream(messege + '\n');
	Date date;

	if (isdigit(stream.peek()))
		stream >> date.year;
	else {
		if (stream.peek() == '-' || stream.peek() == '+') {
			char op;
			stream >> op;
			if (isdigit(stream.peek())) {
				stream.putback(op);
				stream >> date.year;
			}
			else {
				stream.putback(op);
				throw runtime_error("Wrong date format: " + messege);
			}
		}
		else
			throw runtime_error("Wrong date format: " + messege);
	}
	EnsureNextSymbol(stream, "Wrong date format: " + messege);
	stream.ignore(1);

	if (stream.peek() == '-') {
		stream.ignore(1);
		GetValueWithMinus(stream, date.month, "Wrong date format: " + messege);
	}
	else {
		if (isdigit(stream.peek()) || stream.peek() == '+') {
			if (isdigit(stream.peek()))
				stream >> date.month;
			else {
				char op;
				stream >> op;
				if (isdigit(stream.peek())) {
					stream.putback(op);
					stream >> date.month;
				}
				else {
					stream.putback(op);
					throw runtime_error("Wrong date format: " + messege);
				}	
			}
		}
		else
			throw runtime_error("Wrong date format: " + messege);
	}

	EnsureNextSymbol(stream, "Wrong date format: " + messege);
	stream.ignore(1);

	if (stream.peek() == '-') {
		stream.ignore(1);
		GetValueWithMinus(stream, date.day, "Wrong date format: " + messege);
	}
	else {
		if (isdigit(stream.peek()) || stream.peek() == '+') {
			if (isdigit(stream.peek()))
				stream >> date.day;
			else {
				char op;
				stream >> op;
				if (isdigit(stream.peek())) {
					stream.putback(op);
					stream >> date.day;
				}
				else
					throw runtime_error("Wrong date format: " + messege);
			}
		}
		else
			throw runtime_error("Wrong date format: " + messege);
	}
	if (stream.peek() != '\n')
		throw runtime_error("Wrong date format: " + messege);
	if (date.month <= 0 || date.month > 12)
		throw runtime_error("Month value is invalid: " + to_string(date.month));
	if (date.day > 31 || date.day <= 0) {
		throw runtime_error("Day value is invalid: " + to_string(date.day));
	}
	return date;
}
int main()
{
	Database bd;
	Date date;
	string command, messege, date_s;
	map<string, char> commands = { {"Add", 'a'},{ "Del", 'd' },{ "Print", 'p' },{ "Find", 'f' } };
	try {
		while (cin >> command) {
			if (commands.count(command) == 0)
				cout << "Unknown command: " << command << endl;
			else {
				switch (commands[command]) {
				case 'a': {
					cin >> date_s >> messege;
					bd.add(DateParse(date_s), messege);
					break;
				}
				case 'f': {
					cin >> date_s;
					bd.Find(DateParse(date_s));
					break;
				}
				case 'p': {
					bd.Print();
					break;
				}
				case 'd': {
					cin >> date_s;
					if (cin.peek() != '\n') {
						cin >> messege;
						bd.delete_one_event(DateParse(date_s), messege);
					}
					else
						bd.delete_some_events(DateParse(date_s));
				}
				default: 
					break;
				}
			}
		}
	}
	catch (exception& e) { 
		cout << e.what(); 
	}

	return 0;
}

