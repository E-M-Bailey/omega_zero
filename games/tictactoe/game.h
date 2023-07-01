#ifndef GAMES_TICTACTOE_GAME_H
#define GAMES_TICTACTOE_GAME_H

#include <concepts>
#include <cstdint>
#include <span>
#include <string>
#include <tuple>
#include <utility>

namespace tictactoe
{

struct game
{
	// 0 1 2
	// 3 4 5
	// 6 7 8
	// bits [0, 9): player 1, [16, 25): player 2
	typedef std::uint32_t state;

	static constexpr std::size_t MAX_BRANCH = 9;
	static constexpr std::size_t DIMENSIONS = 18;

	[[nodiscard]] static constexpr std::size_t moves(state st, std::span<state, MAX_BRANCH> dest) noexcept;

	[[nodiscard]] static constexpr std::size_t named_moves(state st, std::span<std::pair<std::string, state>, MAX_BRANCH> dest) noexcept;

	[[nodiscard]] static constexpr void flip(state &st) noexcept;

	[[nodiscard]] static constexpr int winner(state st) noexcept;

	template<std::floating_point F>
	[[nodiscard]] static constexpr void encode(state st, std::span<F, DIMENSIONS> dest) noexcept;

private:
	static constexpr std::uint32_t WINS[]
	{
		0b000'000'111u,
		0b000'111'000u,
		0b111'000'000u,
		0b001'001'001u,
		0b010'010'010u,
		0b100'100'100u,
		0b001'010'100u,
		0b100'010'001u
	};
};

[[nodiscard]] constexpr std::size_t game::moves(game::state st, std::span<game::state, game::MAX_BRANCH> dest) noexcept
{
	std::size_t pos = 0;
	for (std::size_t i = 0; i < 9; i++)
	{
		state bits = 0x10001u << i;
		if ((st & bits) == 0)
		{
			dest[pos] = st | (bits & 0xffffu);
			pos++;
		}
	}
	return pos;
}


[[nodiscard]] constexpr std::size_t game::named_moves(game::state st, std::span<std::pair<std::string, game::state>, MAX_BRANCH> dest) noexcept
{
	std::size_t pos = 0;
	for (std::size_t i = 0; i < 9; i++)
	{
		state bits = 0x10001u << i;
		if ((st & bits) == 0)
		{
			dest[pos].first = std::to_string(i + 1);
			dest[pos].second = st | (bits & 0xffffu);
			pos++;
		}
	}
	return pos;
}

[[nodiscard]] constexpr void game::flip(game::state &st)
{
	st = (st >> 16) | (st << 16);
}

[[nodiscard]] constexpr int game::winner(game::state st)
{
	state st2 = st >> 16;
	for (std::uint32_t mask : WINS)
	{
		if ((st & mask) == mask)
		{
			return 1;
		}
		if ((st2 & mask) == mask)
		{
			return -1;
		}
	}
	return 0;
}

template<std::floating_point F>
[[nodiscard]] constexpr void game::encode(game::state st, std::span<F, game::DIMENSIONS> dest) noexcept
{
	for (std::size_t i = 0; i < 9; i++)
	{
		dest[i] = (st >> i) & 1;
	}
	for (std::size_t i = 16; i < 25; i++)
	{
		dest[i - 5] = (st >> i) & 1;
	}
}

}

#endif
