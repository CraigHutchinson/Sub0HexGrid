#include "sub0hexgrid/candidates/candidate_cells.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numbers>

namespace sub0hexgrid
{
namespace
{
static_assert(std::numeric_limits<double>::is_iec559 && std::numeric_limits<double>::digits == 53 &&
              std::numeric_limits<double>::max_exponent == 1024);

struct Interval
{
    double lower;
    double upper;
    [[nodiscard]] bool isFinite() const noexcept
    {
        return std::isfinite(lower) && std::isfinite(upper);
    }
};

[[nodiscard]] double down(double value) noexcept
{
    return std::nextafter(value, -std::numeric_limits<double>::infinity());
}

[[nodiscard]] double up(double value) noexcept
{
    return std::nextafter(value, std::numeric_limits<double>::infinity());
}

[[nodiscard]] Interval subtract(Interval from, Interval amount) noexcept
{
    return {down(from.lower - amount.upper), up(from.upper - amount.lower)};
}

[[nodiscard]] Interval divide(Interval interval, double positiveDivisor) noexcept
{
    return {down(interval.lower / positiveDivisor), up(interval.upper / positiveDivisor)};
}

[[nodiscard]] Interval multiply(Interval interval, double positiveFactor) noexcept
{
    return {down(interval.lower * positiveFactor), up(interval.upper * positiveFactor)};
}
} // namespace

CandidateCells::CandidateCells(Axial minimum, std::uint64_t width, std::uint64_t first,
                               std::uint64_t count) noexcept
    : minimum_(minimum), width_(width), first_(first), count_(count)
{
}

std::optional<CandidateCells> CandidateCells::tryCreate(PointyLayout layout, AxialRegion region,
                                                        Point center, double radius) noexcept
{
    if (!std::isfinite(center.x) || !std::isfinite(center.y) || !std::isfinite(radius) ||
        radius < 0)
        return std::nullopt;

    const auto origin = layout.getOrigin();
    const Interval worldX{down(center.x - radius), up(center.x + radius)};
    const Interval worldY{down(center.y - radius), up(center.y + radius)};
    if (!worldX.isFinite() || !worldY.isFinite())
        return std::nullopt;

    const auto translatedX = subtract(worldX, {origin.x, origin.x});
    const auto translatedY = subtract(worldY, {origin.y, origin.y});
    if (!translatedX.isFinite() || !translatedY.isFinite())
        return std::nullopt;
    const auto x = divide(translatedX, layout.getRadius());
    const auto y = divide(translatedY, layout.getRadius());
    if (!x.isFinite() || !y.isFinite())
        return std::nullopt;

    const auto xTerm = divide(x, std::numbers::sqrt3);
    const auto yTerm = divide(y, 3.0);
    const auto q = subtract(xTerm, yTerm);
    const auto r = multiply(y, 2.0 / 3.0);
    constexpr double supportedMagnitude = 0x1p40;
    if (!xTerm.isFinite() || !yTerm.isFinite() || !q.isFinite() || !r.isFinite() ||
        q.lower < -supportedMagnitude || q.upper > supportedMagnitude ||
        r.lower < -supportedMagnitude || r.upper > supportedMagnitude)
        return std::nullopt;

    // Decision 0002 derives the two-cell envelope for the actual cube repair arithmetic.
    const auto minimum = region.getMinimum();
    const auto maximum = region.getMaximum();
    const double lowerQ = std::max(std::floor(q.lower) - 2.0, static_cast<double>(minimum.q));
    const double upperQ = std::min(std::ceil(q.upper) + 2.0, static_cast<double>(maximum.q));
    const double lowerR = std::max(std::floor(r.lower) - 2.0, static_cast<double>(minimum.r));
    const double upperR = std::min(std::ceil(r.upper) + 2.0, static_cast<double>(maximum.r));
    if (lowerQ > upperQ || lowerR > upperR)
        return CandidateCells{{}, 1, 0, 0};

    const Axial clippedMinimum{static_cast<std::int32_t>(lowerQ),
                               static_cast<std::int32_t>(lowerR)};
    const Axial clippedMaximum{static_cast<std::int32_t>(upperQ),
                               static_cast<std::int32_t>(upperR)};
    const auto clipped = AxialRegion::tryCreate(clippedMinimum, clippedMaximum);
    if (!clipped)
        return std::nullopt;
    const auto width =
        static_cast<std::uint64_t>(static_cast<std::int64_t>(clippedMaximum.q) - clippedMinimum.q) +
        1;
    return CandidateCells{clippedMinimum, width, 0, clipped->getCellCount()};
}

std::optional<CandidateCells> CandidateCells::trySlice(std::uint64_t first,
                                                       std::uint64_t count) const noexcept
{
    if (first > count_ || count > count_ - first)
        return std::nullopt;
    return CandidateCells{minimum_, width_, first_ + first, count};
}

Axial CandidateCells::cellAt(Axial minimum, std::uint64_t width, std::uint64_t index) noexcept
{
    return {static_cast<std::int32_t>(static_cast<std::int64_t>(minimum.q) +
                                      static_cast<std::int64_t>(index % width)),
            static_cast<std::int32_t>(static_cast<std::int64_t>(minimum.r) +
                                      static_cast<std::int64_t>(index / width))};
}

Axial CandidateCells::cellAt(std::uint64_t offset) const noexcept
{
    return cellAt(minimum_, width_, first_ + offset);
}

CandidateCells::Iterator CandidateCells::begin() const noexcept
{
    return Iterator{minimum_, width_, first_};
}

CandidateCells::Iterator CandidateCells::end() const noexcept
{
    return Iterator{minimum_, width_, first_ + count_};
}

CandidateCells::Iterator::Iterator(Axial minimum, std::uint64_t width, std::uint64_t index) noexcept
    : minimum_(minimum), width_(width), index_(index)
{
}

Axial CandidateCells::Iterator::operator*() const noexcept
{
    return CandidateCells::cellAt(minimum_, width_, index_);
}

CandidateCells::Iterator& CandidateCells::Iterator::operator++() noexcept
{
    ++index_;
    return *this;
}

CandidateCells::Iterator CandidateCells::Iterator::operator++(int) noexcept
{
    const auto previous = *this;
    ++*this;
    return previous;
}
} // namespace sub0hexgrid
