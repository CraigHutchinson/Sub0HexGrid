#include <sub0hexgrid/candidates/CandidateCells.hpp>

#include <algorithm>
#include <cmath>
#include <limits>
#include <numbers>

namespace sub0hexgrid {
namespace {
static_assert(std::numeric_limits<double>::is_iec559 &&
    std::numeric_limits<double>::digits == 53 &&
    std::numeric_limits<double>::max_exponent == 1024);

struct Interval {
    double lower;
    double upper;
    [[nodiscard]] bool IsFinite() const noexcept {
        return std::isfinite(lower) && std::isfinite(upper);
    }
};

[[nodiscard]] double Down(double value) noexcept {
    return std::nextafter(value, -std::numeric_limits<double>::infinity());
}

[[nodiscard]] double Up(double value) noexcept {
    return std::nextafter(value, std::numeric_limits<double>::infinity());
}

[[nodiscard]] Interval Subtract(Interval from, Interval amount) noexcept {
    return {Down(from.lower - amount.upper), Up(from.upper - amount.lower)};
}

[[nodiscard]] Interval Divide(Interval interval, double positiveDivisor) noexcept {
    return {Down(interval.lower / positiveDivisor), Up(interval.upper / positiveDivisor)};
}

[[nodiscard]] Interval Multiply(Interval interval, double positiveFactor) noexcept {
    return {Down(interval.lower * positiveFactor), Up(interval.upper * positiveFactor)};
}
}

CandidateCells::CandidateCells(Axial minimum, std::uint64_t width,
    std::uint64_t first, std::uint64_t count) noexcept
    : m_Minimum(minimum), m_Width(width), m_First(first), m_Count(count) {}

std::optional<CandidateCells> CandidateCells::TryCreate(
    PointyLayout layout, AxialRegion region, Point center, double radius) noexcept {
    if (!std::isfinite(center.x) || !std::isfinite(center.y) ||
        !std::isfinite(radius) || radius < 0) return std::nullopt;

    const auto origin = layout.GetOrigin();
    const Interval worldX{Down(center.x - radius), Up(center.x + radius)};
    const Interval worldY{Down(center.y - radius), Up(center.y + radius)};
    if (!worldX.IsFinite() || !worldY.IsFinite()) return std::nullopt;

    const auto translatedX = Subtract(worldX, {origin.x, origin.x});
    const auto translatedY = Subtract(worldY, {origin.y, origin.y});
    if (!translatedX.IsFinite() || !translatedY.IsFinite()) return std::nullopt;
    const auto x = Divide(translatedX, layout.GetRadius());
    const auto y = Divide(translatedY, layout.GetRadius());
    if (!x.IsFinite() || !y.IsFinite()) return std::nullopt;

    const auto xTerm = Divide(x, std::numbers::sqrt3);
    const auto yTerm = Divide(y, 3.0);
    const auto q = Subtract(xTerm, yTerm);
    const auto r = Multiply(y, 2.0 / 3.0);
    constexpr double supportedMagnitude = 0x1p40;
    if (!xTerm.IsFinite() || !yTerm.IsFinite() || !q.IsFinite() || !r.IsFinite() ||
        q.lower < -supportedMagnitude || q.upper > supportedMagnitude ||
        r.lower < -supportedMagnitude || r.upper > supportedMagnitude) return std::nullopt;

    // Decision 0002 derives the two-cell envelope for the actual cube repair arithmetic.
    const auto minimum = region.GetMinimum();
    const auto maximum = region.GetMaximum();
    const double lowerQ = std::max(std::floor(q.lower) - 2.0, static_cast<double>(minimum.q));
    const double upperQ = std::min(std::ceil(q.upper) + 2.0, static_cast<double>(maximum.q));
    const double lowerR = std::max(std::floor(r.lower) - 2.0, static_cast<double>(minimum.r));
    const double upperR = std::min(std::ceil(r.upper) + 2.0, static_cast<double>(maximum.r));
    if (lowerQ > upperQ || lowerR > upperR) return CandidateCells{{}, 1, 0, 0};

    const Axial clippedMinimum{static_cast<std::int32_t>(lowerQ), static_cast<std::int32_t>(lowerR)};
    const Axial clippedMaximum{static_cast<std::int32_t>(upperQ), static_cast<std::int32_t>(upperR)};
    const auto clipped = AxialRegion::TryCreate(clippedMinimum, clippedMaximum);
    if (!clipped) return std::nullopt;
    const auto width = static_cast<std::uint64_t>(
        static_cast<std::int64_t>(clippedMaximum.q) - clippedMinimum.q) + 1;
    return CandidateCells{clippedMinimum, width, 0, clipped->GetCellCount()};
}

std::optional<CandidateCells> CandidateCells::TrySlice(
    std::uint64_t first, std::uint64_t count) const noexcept {
    if (first > m_Count || count > m_Count - first) return std::nullopt;
    return CandidateCells{m_Minimum, m_Width, m_First + first, count};
}

Axial CandidateCells::CellAt(Axial minimum, std::uint64_t width, std::uint64_t index) noexcept {
    return {
        static_cast<std::int32_t>(static_cast<std::int64_t>(minimum.q) +
            static_cast<std::int64_t>(index % width)),
        static_cast<std::int32_t>(static_cast<std::int64_t>(minimum.r) +
            static_cast<std::int64_t>(index / width))};
}

Axial CandidateCells::CellAt(std::uint64_t offset) const noexcept {
    return CellAt(m_Minimum, m_Width, m_First + offset);
}

CandidateCells::Iterator CandidateCells::begin() const noexcept {
    return Iterator{m_Minimum, m_Width, m_First};
}

CandidateCells::Iterator CandidateCells::end() const noexcept {
    return Iterator{m_Minimum, m_Width, m_First + m_Count};
}

CandidateCells::Iterator::Iterator(
    Axial minimum, std::uint64_t width, std::uint64_t index) noexcept
    : m_Minimum(minimum), m_Width(width), m_Index(index) {}

Axial CandidateCells::Iterator::operator*() const noexcept {
    return CandidateCells::CellAt(m_Minimum, m_Width, m_Index);
}

CandidateCells::Iterator& CandidateCells::Iterator::operator++() noexcept {
    ++m_Index;
    return *this;
}

CandidateCells::Iterator CandidateCells::Iterator::operator++(int) noexcept {
    const auto previous = *this;
    ++*this;
    return previous;
}
}
