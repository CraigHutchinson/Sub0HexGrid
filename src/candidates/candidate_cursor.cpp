#include "sub0hexgrid/candidates/candidate_cursor.hpp"

#include <algorithm>

namespace sub0hexgrid
{
CandidateCursor::CandidateCursor(CandidateCells cells) noexcept : cells_(cells) {}

std::size_t CandidateCursor::read(std::span<Axial> output) noexcept
{
    const auto count =
        static_cast<std::size_t>(std::min<std::uint64_t>(output.size(), remaining()));
    for (auto& cell : output.first(count))
        cell = cells_.cellAt(consumed_++);
    return count;
}
} // namespace sub0hexgrid
