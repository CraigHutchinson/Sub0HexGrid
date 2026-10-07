#include "sub0hexgrid/PointyLayout.hpp"

#include <cmath>
#include <limits>
#include <numbers>

namespace sub0hexgrid {
PointyLayout::PointyLayout(double radius, Point origin) noexcept
    : m_Radius(radius), m_Origin(origin) {}

std::optional<PointyLayout> PointyLayout::TryCreate(double radius, Point origin) noexcept {
    if (!std::isfinite(radius) || radius <= 0 ||
        !std::isfinite(origin.x) || !std::isfinite(origin.y))
        return std::nullopt;
    return PointyLayout{radius, origin};
}

std::optional<Point> PointyLayout::TryCellCenter(Axial cell) const noexcept {
    const Point center{
        m_Origin.x + m_Radius * (std::numbers::sqrt3 * (cell.q + 0.5 * cell.r)),
        m_Origin.y + m_Radius * (1.5 * cell.r)};
    if (!std::isfinite(center.x) || !std::isfinite(center.y) || TryCellAt(center) != cell) return std::nullopt;
    return center;
}

std::optional<Axial> PointyLayout::TryCellAt(Point position) const noexcept {
    if (!std::isfinite(position.x) || !std::isfinite(position.y)) return std::nullopt;
    const double x = (position.x - m_Origin.x) / m_Radius;
    const double y = (position.y - m_Origin.y) / m_Radius;
    const double q = x / std::numbers::sqrt3 - y / 3.0;
    const double r = y * (2.0 / 3.0);
    const double s = -q - r;
    if (!std::isfinite(q) || !std::isfinite(r) || !std::isfinite(s)) return std::nullopt;

    double rounded_q = std::round(q);
    double rounded_r = std::round(r);
    const double rounded_s = std::round(s);
    const double q_error = std::abs(rounded_q - q);
    const double r_error = std::abs(rounded_r - r);
    const double s_error = std::abs(rounded_s - s);
    if (q_error >= r_error && q_error >= s_error) rounded_q = -rounded_r - rounded_s;
    else if (r_error >= s_error) rounded_r = -rounded_q - rounded_s;

    constexpr double minimum = std::numeric_limits<std::int32_t>::min();
    constexpr double maximum = std::numeric_limits<std::int32_t>::max();
    if (!std::isfinite(rounded_q) || !std::isfinite(rounded_r) ||
        rounded_q < minimum || rounded_q > maximum || rounded_r < minimum || rounded_r > maximum)
        return std::nullopt;
    return Axial{static_cast<std::int32_t>(rounded_q), static_cast<std::int32_t>(rounded_r)};
}
}
