#ifndef GAMES_TICTACTOE_CLI_H
#define GAMES_TICTACTOE_CLI_H

#include <cstdint>
#include <cstring>
#include <iostream>
#include <optional>
#include <string>

#include "game.h"

namespace games::tictactoe
{

struct cli
{
	static void display(std::ostream &os, game::state st);

	static std::optional<game::state> parse(std::istream &is, game::state st);
};

void cli::display(std::ostream &os, game::state st)
{
	static char const STR[] =
		"1│2│3\n"
		"─┼─┼─\n"
		"4│5│6\n"
		"─┼─┼─\n"
		"7│8│9\n";
	static char buf[std::size(STR)];
	std::strcpy(buf, STR);

	char *ptr = buf;
	game::state st2 = st >> 16;
	for (int i = 0; i < 9; i++)
	{
		game::state bit = 1u << i;
		while (*ptr != i + '1')
		{
			ptr++;
		}
		if (st & bit)
		{
			*ptr = 'X';
		}
		else if (st2 & bit)
		{
			*ptr = 'O';
		}
	}
	os << buf;
}

std::optional<game::state> cli::parse(std::istream &is, game::state st)
{
	static std::string str;
	static std::pair<std::string, game::state> options[game::MAX_BRANCH];

	is >> str;
	int num = game::named_moves(st, options);
	for (int i = 0; i < num; i++)
	{
		if (options[i].first == str)
		{
			return options[i].second;
		}
	}
	return std::nullopt;
}

}

#endif
