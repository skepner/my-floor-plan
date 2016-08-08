#include "room.hh"

// ----------------------------------------------------------------------

void room_work(Surface& s)
{
    const double w1 = 4.14, w2 = 4.29, c1 = 1.05, c2 = 1.53, sq = w1 * (w2 - c2) + (w1 - c1) * c2;

    s.wall_with_size(- s.wall_width_2(),  0,       w1,      0, 0, -0.2);
    s.wall(w1, 0,       w1,      w2 - c2);
    s.wall(w1, w2 - c2, w1 - c1 - s.wall_width_2(), w2 - c2);
      // s.wall_with_door(w1 - c1, w2 - c2, w1 - c1, w2, 0.2, 0.9);
      // s.wall_with_door(w1 - c1, w2, 0, w2, 0.2, 0.9);
    s.wall_with_size(0, w2, 0, 0, -0.2, 0);

    s.text(w1 / 2 - 0.5, w2 / 2, to_string(sq) + "m2", BLACK, 16);

} // room_work

// ----------------------------------------------------------------------

void heizung_2og(Surface& s)
{
    const double x = 0, y = 4.29, w = 3.09, h = 1.35, sq = w * h;
    s.wall_with_door(x, y, x + w, y, w - 1.1, 0.9, DoorDirection::firstLeft);
    s.wall(x + w, y, x + w, y + h);
    s.wall_with_size(x + w, y + h, x - s.wall_width_2(), y + h, 0, 0.3);
    s.wall_with_size(x, y + h, x, y, -0.2, 0);

    s.text(w / 2 - 0.5, y + h / 2, to_string(sq) + "m2", BLACK, 16);
}

// ----------------------------------------------------------------------

void flur_2og(Surface& s)
{
      // const double x = 3.21, y = 2.88, w = 2.85, h = 2.76;
    const double x = 3.09, y = 2.76, w = 2.97, h = 2.88, sq = 2.85 * 2.76;
    s.wall_with_door(x, y, x + w, y, w - 1, 0.9, DoorDirection::firstLeft);
    s.wall_with_door(x + w, y, x + w, y + h, 0.1, 0.9, DoorDirection::secondLeft);
    s.wall_with_size(x + w, y + h, x, y + h, 0, 0.3);
    s.wall_with_door(x, y + h, x, y, h - 0.98, 0.9, DoorDirection::firstLeft);

} // flur_2og

// ----------------------------------------------------------------------

void bad_2og(Surface& s)
{
    const double x = 4.14, y = 0, w = 1.92, h = 2.76, sq = w * h;
    s.wall_with_size(x, y, x + w, y, 0, -0.2);
    s.wall(x + w, y, x + w, y + h);

    s.text(x + w / 2 - 0.5, y + h / 2, to_string(sq) + "m2", BLACK, 16);

} // bad_2og

// ----------------------------------------------------------------------

void bett_2og(Surface& s)
{
    const double x = 6.06, y = 0, w = 4.28, h = 5.64, sq = w * h;
    s.wall_with_size(x, y, x + w + s.wall_width_2(), y, 0, -0.2);
    s.wall_with_size(x + w, y, x + w, y + h + s.wall_width_2(), 0.3, 0);
    s.wall(x + w, y + h, x, y + h);

    s.text(x + w / 2 - 0.5, y + h / 2, to_string(sq) + "m2", BLACK, 16);

} // bett_2og

// ----------------------------------------------------------------------


// void room(Surface& s, double x, double y, double w, double h)
// {
//     s.wall(x, y, x + w, y);
//     s.wall(x + w, y, x + w, y + h);
//     s.wall(x + w, y + h, x, y + h);
//     s.wall_with_door(x, y + h, x, y, h - 0.98, 0.9);

// } // room

// ----------------------------------------------------------------------
