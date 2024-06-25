/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Warlock.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:27:34 by jhogonca          #+#    #+#             */
/*   Updated: 2024/06/25 17:38:33 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WARLOCK_HPP
# define WARLOCK_HPP

# include <iostream>

using namespace std;

class Warlock {
		// attributes
		string _name;
		string _title;

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
};

#endif
