#pragma once

#include <string>
#include <cstdio>
#include <cmath>

// ----------------------------------------------------------------------

inline std::string to_string(double value)
{
    char b[100];
    std::sprintf(b, "%.1f", value);
    return b;

} // to_string

// ----------------------------------------------------------------------

inline double length(double x1, double y1, double x2, double y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

} // length

// ----------------------------------------------------------------------

inline std::pair<double, double> at_offset(double x1, double y1, double x2, double y2, double offset)
{
    const double part = offset / length(x1, y1, x2, y2);
    return std::make_pair(x1 + (x2 - x1) * part, y1 + (y2 - y1) * part);

} // at_offset

// ----------------------------------------------------------------------

inline std::pair<double, double> perpendicular(double x1, double y1, double x2, double y2, bool left)
{
    if (left)
        return std::make_pair(y1 - y2 + x2, x2 - x1 + y2);
    else
        return std::make_pair(y2 - y1 + x2, x1 - x2 + y2);

} // perpendicular

// ----------------------------------------------------------------------
