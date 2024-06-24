#ifndef WARLOCK_HPP
#define  WARLOCK_HPP

#include <string>
#include <iostream>


using std::string;
using std::cout;
using std::endl;

class Warlock {
		// attributes
		string _name;
		string _title;

		// Constructors
		Warlock();
		Warlock(const Warlock &otr);
		
		// Assigment Operator
		Warlock & operator=(const Warlock &otr);

	public:
		// Constructor & Destructor
		Warlock(const string &name, const string &title);
		~Warlock();
		
		// Getters
		const string &getName() const;
		const string &getTitle() const;

		// Setter
		void setTitle(const string & title);

		// Method
		void	introduce() const;
};

#endif
