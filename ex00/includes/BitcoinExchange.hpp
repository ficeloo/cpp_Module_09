/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcros <tcros@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 10:16:28 by tcros             #+#    #+#             */
/*   Updated: 2026/03/09 16:18:31 by tcros            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <map>
# include <fstream>
# include <exception>
# include <sstream>
# include <climits>
# include <iomanip>

# include "coloring.hpp"

# define DATA_BASE "data.csv"
# define FIRST_LINE_DB "date,exchange_rate"

class NotPositiveNumber : public std::exception
{
	virtual const char*     what() const throw()
	{
		return ("not a positive number.");
	}
};

class NoDataFileFound : public std::exception
{
	virtual const char*     what() const throw()
	{
		return ("no database found.");
	}
};

class BadInput : public std::exception
{
	private :
		std::string	_msg;
	
	public :
	virtual const char*     what() const throw()
	{
		return (_msg.c_str());
	}

	BadInput( std::string input, int lineNbr )
	{
		std::ostringstream	oss;

		oss << "bad input => " << input << " at line " << lineNbr;
		_msg = oss.str();
	}
	virtual ~BadInput() throw() {}
};

class CouldntOpenFile : public std::exception
{
	virtual const char*     what() const throw()
	{
		return ("could not open file.");
	}
};

class TooLargeNumber : public std::exception
{
	virtual const char*     what() const throw()
	{
		return ("too large a number.");
	}
};

void	bitcoinexchange(int ac, char *av[]);

#endif
