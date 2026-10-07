#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

#include "sub0hexgrid/candidates/candidate_cells.hpp"

namespace sub0hexgrid
{
/** Resumable candidate traversal with caller-owned, workload-bounded output.
 * Copying snapshots progress; independent copies can run concurrently.
 * Mutation of one cursor requires exclusive access; no clock or worker is owned.
 */
class CandidateCursor
{
  public:
    /** Starts at the beginning of an owned range or slice.
     * @param cells Validated candidate metadata; no source object is retained.
     */
    explicit CandidateCursor(CandidateCells cells) noexcept;

    /** Writes the next prefix with constant work per cell and no allocation.
     * @param output Caller-owned cells to fill; an empty span makes no progress.
     * @return Number written; only that prefix is modified, and isDone reports completion.
     */
    [[nodiscard]] std::size_t read(std::span<Axial> output) noexcept;

    /** Returns the number of candidates not yet written.
     * @return The number of remaining cells in the owned range.
     */
    [[nodiscard]] constexpr std::uint64_t remaining() const noexcept
    {
        return cells_.getCellCount() - consumed_;
    }

    /** Reports whether traversal has consumed the complete range.
     * @return True when no candidate remains.
     */
    [[nodiscard]] constexpr bool isDone() const noexcept { return remaining() == 0; }

  private:
    CandidateCells cells_;
    std::uint64_t consumed_{};
};
} // namespace sub0hexgrid
