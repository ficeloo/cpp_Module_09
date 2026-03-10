/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcros <tcros@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 14:36:08 by tcros             #+#    #+#             */
/*   Updated: 2026/03/10 16:05:01 by tcros            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

static bool	isoperator(char c)
{
	return (c == '+' || c == '-' || c == '*' || c == '/');
}

static void	parse(std::string& str)
{
	if (str.length() != 1)
		throw BadInput();

	if (!std::isdigit(str[0]) && !isoperator(str[0]))
		throw UnauthorizedChar();
}

static int	add(int x, int y)
{
	return (x + y);
}

static int	sub(int x, int y)
{
	return (x - y);
}

static int	mul(int x, int y)
{
	return (x * y);
}

static int	divi(int x, int y)
{
	if (y == 0)
		throw DivisionByZero();
	return (x / y);
}

static void	operate(std::stack<int>& mystack, std::string& oper)
{
	if (mystack.size() < 2)
		throw BadOperationOrder();

	std::string	ops[] = {"+", "-", "*", "/"};
	int	(*funcs[])(int, int) = {add, sub, mul, divi};

	int	nb1 = mystack.top();
	mystack.pop();
	int	nb2 = mystack.top();
	mystack.pop();

	for (size_t i = 0; i < 4; ++i)
	{
		if (oper == ops[i])
		{
			mystack.push(funcs[i](nb2, nb1));
			break;
		}
	}
}

static int	calculate(char *av)
{
	std::stringstream	ss(av);
	std::string			split;
	const char			del = ' ';
	
	std::stack<int>	mystack;

	while (std::getline(ss, split, del))
	{
		if (split.empty())
			continue;

		parse(split);

		if (std::isdigit(split[0]))
			mystack.push(std::atoi(split.c_str()));
		else
			operate(mystack, split);
	}
	if (mystack.size() != 1)
		throw BadInput();
	return (mystack.top());
}

void	RPN(char *av)
{
	try
	{
		int	result = calculate(av);

		std::cout << result << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
	}
}
