/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PMergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcros <tcros@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 14:31:19 by tcros             #+#    #+#             */
/*   Updated: 2026/03/18 17:51:50 by tcros            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <deque>
# include <vector>
# include <ctime>
# include <cstdlib>
# include <sstream>
# include <climits>
# include <cmath>
# include <algorithm>

# include "coloring.hpp"

class TooLargeNumber : public std::exception
{
	virtual const char*	what() const throw()
	{
		return ("larger than an integer.");
	}
};

class NotADigit: public std::exception
{
	virtual const char*	what() const throw()
	{
		return ("not a digit.");
	}
};

class BadInput : public std::exception
{
	virtual const char*	what() const throw()
	{
		return ("bad input.");
	}
};

void	PMergeMe(int ac, char *av[]);

#endif
