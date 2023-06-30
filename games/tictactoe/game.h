#ifndef GAMES_TICTACTOE_GAME_H
#define GAMES_TICTACTOE_GAME_H

#include <concepts>
#include <cstdint>
#include <tuple>
#include <utility>

namespace tictactoe
{

struct game
{
	// 0 1 2
	// 3 4 5
	// 6 7 8
	typedef std::pair<std::uint16_t, std::uint16_t> state;

	static constexpr int MAX_BRANCH = 9;
	static constexpr int DIMENSIONS = 18;

	[[nodiscard]] static constexpr state *moves(state const &st, state *dest) noexcept;

	[[nodiscard]] static constexpr void flip(state &st) noexcept;

	[[nodiscard]] static constexpr int winner(state const &st) noexcept;

	template<std::floating_point F>
	[[nodiscard]] static constexpr void encode(state const &st, F *dest) noexcept;

private:
	static constexpr std::uint16_t WINS[]
	{
		0b000'000'111,
		0b000'111'000,
		0b111'000'111,
		0b001'001'001,
		0b010'010'010,
		0b100'100'100,
		0b001'010'100,
		0b100'010'001
	};
};

[[nodiscard]] constexpr game::state *game::moves(game::state const &st, game::state *dest) noexcept
{
	for (int i = 0; i < 9; i++)
	{
		std::uint16_t bit = 1 << i;
		if (((st.first | st.second) & bit) == 0)
		{
			dest->first = st.first | bit;
			dest->second = st.second;
			dest++;
		}
	}
	return dest;
}

[[nodiscard]] constexpr void game::flip(game::state &st)
{
	std::swap(st.first, st.second);
}

[[nodiscard]] constexpr int game::winner(game::state const &st)
{
	for (std::uint16_t mask : WINS)
	{
		if ((st.first & mask) == mask)
		{
			return 1;
		}
		if ((st.second & mask) == mask)
		{
			return -1;
		}
	}
	return 0;
}

template<std::floating_point F>
[[nodiscard]] constexpr void game::encode(game::state const &st, F *dest) noexcept
{
	for (int i = 0; i < 9; i++)
	{
		*dest++ = (st.first >> i) & 1;
	}
	for (int i = 0; i < 9; i++)
	{
		*dest++ = (st.second >> i) & 1;
	}
}

}

#endif
