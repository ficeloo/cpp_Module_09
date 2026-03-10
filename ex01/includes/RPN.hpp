/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcros <tcros@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 14:34:48 by tcros             #+#    #+#             */
/*   Updated: 2026/03/10 15:54:42 by tcros            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
# define RPN_HPP

# include <stack>
# include <sstream>
# include <exception>
# include <cctype>
# include <cstdlib>

# include "coloring.hpp"

class BadInput : public std::exception
{
	virtual const char*	what() const throw()
	{
		return ("bad input.");
	}
};

class DivisionByZero: public std::exception
{
	virtual const char*	what() const throw()
	{
		return ("cannot divide by zero.");
	}
};

class BadOperationOrder : public std::exception
{
	virtual const char*	what() const throw()
	{
		return ("bad operation order.");
	}
};

class UnauthorizedChar : public std::exception
{
	virtual const char*	what() const throw()
	{
		return ("unknown char.");
	}
};

void	RPN(char *av);

#endif
