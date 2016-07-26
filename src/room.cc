#include "room.hh"

// ----------------------------------------------------------------------

void room_work(Surface& s)
{
    const double w1 = 4.14, w2 = 4.29, c1 = 1.05, c2 = 1.53;

    s.wall_with_size(0,  0,       w1,      0, 0, -0.2);
    s.wall(w1, 0,       w1,      w2 - c2);
    s.wall(w1, w2 - c2, w1 - c1, w2 - c2);
    s.wall_with_door(w1 - c1, w2 - c2, w1 - c1, w2);
    s.wall(w1 - c1, w2, 0, w2);
    s.wall(0, w2, 0, 0);

} // room_work

// ----------------------------------------------------------------------

void room(Surface& s, double x, double y, double w, double h)
{
    s.wall(x, y, x + w, y);
    s.wall(x + w, y, x + w, y + h);
    s.wall(x + w, y + h, x, y + h);
    s.wall(x, y + h, x, y);

} // room

// ----------------------------------------------------------------------
