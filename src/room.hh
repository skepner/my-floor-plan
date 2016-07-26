#pragma once

#include "surface.hh"

// ----------------------------------------------------------------------

void room(Surface& s, double x, double y, double w, double h);

// ----------------------------------------------------------------------

void room_work(Surface& s);

inline void heizung_2og(Surface& s)
{
    room(s, 0, 4.29, 3.09, 1.35);

} // heizung_2og

// ----------------------------------------------------------------------

inline void flur_2og(Surface& s)
{
    room(s, 3.21, 2.88, 2.85, 2.76);
}


// ----------------------------------------------------------------------
