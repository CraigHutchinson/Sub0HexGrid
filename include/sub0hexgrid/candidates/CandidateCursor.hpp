#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

#include "sub0hexgrid/candidates/CandidateCells.hpp"

namespace sub0hexgrid {
/** Resumable candidate traversal with caller-owned, workload-bounded output.
 * Copying snapshots progress; independent copies can run concurrently.
 * Mutation of one cursor requires exclusive access; no clock or worker is owned.
 */
class CandidateCursor {
public:
    /** Starts at the beginning of an owned range or slice.
     * @param cells Validated candidate metadata; no source object is retained.
     */
    explicit CandidateCursor(CandidateCells cells) noexcept;

    /** Writes the next prefix with constant work per cell and no allocation.
     * @param output Caller-owned cells to fill; an empty span makes no progress.
     * @return Number written; only that prefix is modified, and IsDone reports completion.
     */
    [[nodiscard]] std::size_t Read(std::span<Axial> output) noexcept;

    [[nodiscard]] constexpr std::uint64_t Remaining() const noexcept { return m_Cells.GetCellCount() - m_Consumed; }
    [[nodiscard]] constexpr bool IsDone() const noexcept { return Remaining() == 0; }

private:
    CandidateCells m_Cells;
    std::uint64_t m_Consumed{};
};
}
