#ifndef GAMES_GAME_H
#define GAMES_GAME_H

#include <concepts>
#include <iostream>
#include <optional>
#include <span>
#include <string>

namespace games
{

template<typename G>
concept game = requires
{
	typename G::state;

	{G::MAX_BRANCH} noexcept -> std::convertible_to<std::size_t>;

		requires requires
	(
		typename G::state &st,
		typename G::state const &cst,
		std::span<typename G::state, G::MAX_BRANCH> moves_dest,
		std::span<std::pair<std::string, typename G::state>, G::MAX_BRANCH> named_moves_dest
		)
	{
		{G::moves(cst, moves_dest)} noexcept -> std::convertible_to<std::size_t>;
		{G::named_moves(cst, named_moves_dest)} noexcept -> std::convertible_to<std::size_t>;
		{G::flip(st)} noexcept;
		{G::winner(cst)} noexcept -> std::convertible_to<int>; // TODO make an enum for this
		{G::hash(cst)} noexcept -> std::convertible_to<std::size_t>;
	};
};

template<typename G, typename F>
concept encodable_game = requires
{
	game<G>;
	std::floating_point<F>;

	{ G::DIMENSIONS } noexcept -> std::convertible_to<std::size_t>;

		requires requires
	(
		typename G::state const &cst,
		std::span<F, G::DIMENSIONS> encode_dest
		)
	{
		{ G::encode(cst, encode_dest) } noexcept;
	};
};

template<typename C, typename G>
concept cli_for = requires
{
	game<G>;

		requires requires
	(
		typename G::state const &cst,
		std::istream &is,
		std::ostream &os
		)
	{
		{C::parse(is, cst)} -> std::convertible_to<std::optional<typename G::state>>;
		{C::display(os, cst)};
	};
};

}

#endif
