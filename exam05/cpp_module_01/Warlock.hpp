/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Warlock.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:27:34 by jhogonca          #+#    #+#             */
/*   Updated: 2024/06/25 17:27:34 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WARLOCK_HPP
# define WARLOCK_HPP

# include <map>
# include <iostream>
# include "ASpell.hpp"
# include "ATarget.hpp"

using namespace std;

class Warlock {
		// attributes
		string _name;
		string _title;
		map<string, ASpell*> _slots;

		// Constructor
		Warlock() {}

		// Copy Constructor
		Warlock(const Warlock &other);
		
		// Assigment Operator
		Warlock & operator=(const Warlock &other);
	public:
		// Constructor & Destructor
		Warlock(const string &name, const string &title);
		~Warlock();
		
		// Getters and Setter
		const string &getName() const;
		const string &getTitle() const;
		void setTitle(const string & title);

		// Methods
		void	introduce() const;
		void	learnSpell(ASpell* spell);
		void	forgetSpell(string spell);
		void	launchSpell(string spell, const ATarget& target);
};

#endif
