#include "alternative.h"
//std
#include <algorithm>
#include <vector>
#include <iterator>
#include <numeric>
#include <utility>
#include <tuple>

namespace alternative
{
	namespace
	{
		using Command = std::pair<char, uint32_t>;

		//represents the input sequence and the damage applied at each index
		using DamageSequence = std::vector<Command>;

		//returns a sequence with the command (S/C) and the damage applied at that index. the minimum damage that the input will give. the current damage being applied by the input
		[[nodiscard]] std::tuple<DamageSequence, uint32_t, uint32_t> calculateDamage(const std::string& input) noexcept
		{
			DamageSequence sequence{};//holds the damange applied at each index, maching the index of the Ss in input
			uint32_t strength{ 1 };
			uint32_t minDamange{ 0 };//the minimum damage is the number of Ss. this is useful to short circuit the algorithm
			uint32_t damage{ 0 };
			std::transform(input.begin(), input.end(), std::back_inserter(sequence), [&strength, &minDamange, &damage](const char c) {
				if (c == 'C')
				{
					strength *= 2;//C's double the
					return std::make_pair('C', 0U);//C's always apply 0 damage
				}//else, always S. not validating the input
				++minDamange;//each S counts 1 for the minDamage
				damage += strength;//apply the current strength to the total damage delt
				return std::make_pair('S', strength);//The current strength is the damage applied by this S
				} );
			return { sequence, minDamange, damage };//return the sequence that was generated containing the same input and the damage applied at each S, while also returning the minDamage and current applicable damage
		}

		inline void hack(DamageSequence::reverse_iterator& match, uint32_t& damage) noexcept
		{
			auto reduction = match->second / 2;
			match->first = 'C';
			match->second = 0;
			++match;
			match->first = 'S';
			match->second = reduction;
			damage -= reduction;
			--match;//just moving the iterator to its previous position
		}

		inline void nextHack(DamageSequence& sequence, DamageSequence::reverse_iterator& it) noexcept
		{
			static const std::vector<char> pattern{ 'C', 'S' };//saving from constant allocations. maybe this could be constexpr and have the same efficiency?
			//it's important to search from right to left because the right-most hack is the best hack and a greedy algorithm, always doing the best hack at the moment, is the best algorithm
			it = std::search(it, sequence.rend(), pattern.rbegin(), pattern.rend(), [](const Command& c, const char p) { return c.first == p; });//TODO why the bool operator==(const Command& c, const char p) didn't work?
		}
	}

	/// <summary>
	/// O(n) solution for the problem
	/// Calculates the damage and builds an array with the damage applied at each S, also calculates the minimum damage, the number of S in the sequence
	/// Short-circuit if it's not possible. If the number of S (minimum damage) is greater than shield, we are doomed
	/// Knowing that there is a solution, start doing consecutive hacks at the most efficient place, the right most CS pair
	/// Continue the search from the last hack (+1) position to save time not going over old values. The +1 is required since the C that was moved to the right (CS -> SC) could have created another CS (ex: CSS -> SCS)
	/// When the damage is finally <= shield, exist reporting the current number of iterations spent doing hacks
	/// </summary>
	int saveEarth(const std::string& input, const uint32_t shield) noexcept
	{
		if (input.size() > 1) [[likely]]//for sizes 1 and 0 there is an optimization bellow. > 1 is kept here for processor pipeline optimization (good case for [[likely]]?)
		{
			if (auto [sequence, minDamage, damage] = calculateDamage(input); minDamage <= shield)
			{
				int iterations{ 0 };
				for (auto it = sequence.rbegin(); damage > shield;)
				{
					nextHack(sequence, it);//nextHack will always return something. in cases where it wouldn't, we should have exited the loop already
					++iterations;
					hack(it, damage);
					if (it != sequence.rbegin())//unless we are at the edge, we should move one more to the right to account for the C that was just moved right and possibly created a CS immediately to the right of the last hack
						--it;
				}
				return iterations;
			}
		}
		else if ((input.size() == 0) ||//no damage means we are always good!
			(input[0] == 'C') ||//if it isn't > 1 nor 0 it has to be 1. if it's C there is no damage so we are good!
			(shield >= 1)) [[unlikely]]//here it's clearly size 1 and it's 'S'. so for shield >= 1 we are good!
		{
			return 0;
		}
		return -1;//it was not possible to save earth :(
	}
}