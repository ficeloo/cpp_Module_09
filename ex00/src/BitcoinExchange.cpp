/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcros <tcros@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 10:28:50 by tcros             #+#    #+#             */
/*   Updated: 2026/03/10 12:32:08 by tcros            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

static void	parse_date(std::string& date, int line, bool input)
{
	if (date.length() != 10)
		throw BadInput(date, line, !input);

	if (date[4] != '-' || date[7] != '-')
		throw BadInput(date, line, !input);

	std::stringstream	ssdate(date);

	char	dash;
	int	year;
	int	month;
	int	day;

	if (!(ssdate >> year >> dash >> month >> dash >> day))
		throw BadInput(date, line, !input);

	bool	isBis = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	bool	isYear = (year < 2009) && !input;
	bool	isMonth = (month < 1) || (month > 12);
	
	if (isYear || isMonth)
		throw BadInput(date, line, !input);

	int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (isBis)
		daysInMonth[1] = 29;

	bool	isDay = (day < 1 || day > daysInMonth[month - 1]);

	if (isDay)
		throw BadInput(date, line, !input);
}

static float	parse_value(std::string& value, int line, float val_max, bool isData)
{
	if (value.empty())
		throw NotPositiveNumber();
	
	std::stringstream	ssval(value);

	float	val;

	if (!(ssval >> val))
		throw BadInput(value, line, isData);

	if (val < 0)
		throw NotPositiveNumber();

	if (val > val_max)
		throw TooLargeNumber();

	return (val);
}

static void	search_and_print(std::map<std::string, float>& data_map, std::string s_date, float val)
{
	std::map<std::string, float>::iterator	it;

	it = data_map.lower_bound(s_date);

	if (it->first != s_date)
	{
		if (it != data_map.begin())
			--it;
	}

	std::cout <<  s_date << " => " << val << " = "
		<< (val * it->second)
		<< std::endl;
}

static void	parse_file(std::ifstream& infile, std::map<std::string, float>* data_map, const std::string& sep, bool input)
{
	std::string	currLine;
	int	i = 2;

	while (std::getline(infile, currLine))
	{
		try
		{
			size_t	pos = currLine.find(sep);

			if (pos == std::string::npos)
				throw BadInput(currLine, i, !input);
			
			std::string	s_date = currLine.substr(0, pos);
			std::string	s_val = currLine.substr(pos + sep.length());
			
			float	max_value = (input) ? 1000 : INT_MAX;
			parse_date(s_date, i, input);
			float	val = parse_value(s_val, i, max_value, !input);

			if (!input)
				(*data_map)[s_date] = val;
			else
				search_and_print(*data_map, s_date, val);
			i++;
		}
		catch (std::exception& e)
		{
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
}

static void	parsing(std::map<std::string, float>& data_map)
{
	std::ifstream	infile;

	size_t		pos;
	std::string	db_name = DATA_BASE;

	pos = db_name.rfind(".csv");
	if (pos == std::string::npos || pos != db_name.size() - 4)
		throw NoDataFileFound();

	infile.open(DATA_BASE);
	if (!infile)
		throw NoDataFileFound();

	std::string	firstLine;
	std::string	firstline_name = FIRST_LINE_DB;

	std::getline(infile, firstLine);
	if (firstLine != firstline_name)
		throw BadInput(firstLine, 1, true);

	parse_file(infile, &data_map, ",", false);
	infile.close();
}

static void	check_input_file(char *av, std::map<std::string, float>& data_map)
{
	std::ifstream	input_file;

	input_file.open(av);
	if (!input_file)
		throw CouldntOpenFile();

	std::string	firstLine;

	std::getline(input_file, firstLine);
	if (firstLine != "date | value")
		throw BadInput(firstLine, 1, false);

	parse_file(input_file, &data_map, " | ", true);
	input_file.close();
}

void	bitcoinexchange(int ac, char *av[])
{
	try
	{
		if (ac < 2)
			throw CouldntOpenFile();

		std::map<std::string, float>	data_map;

		parsing(data_map);
		
		check_input_file(av[1], data_map);
	}
	catch (const std::exception& e)
	{
		magnifText('e', e.what());
	}
}
