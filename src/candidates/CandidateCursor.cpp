#include <sub0hexgrid/candidates/CandidateCursor.hpp>

#include <algorithm>

namespace sub0hexgrid {
CandidateCursor::CandidateCursor(CandidateCells cells) noexcept : m_Cells(cells) {}

std::size_t CandidateCursor::Read(std::span<Axial> output) noexcept {
    const auto count = static_cast<std::size_t>(std::min<std::uint64_t>(output.size(), Remaining()));
    for (auto& cell : output.first(count)) cell = m_Cells.CellAt(m_Consumed++);
    return count;
}
}
