#include <iostream>
#include <getopt.h>

#include "room.hh"

// ----------------------------------------------------------------------

[[noreturn]] static void usage(std::string progname);
static void draw(std::string filename, size_t canvas_height, size_t canvas_width);

// ----------------------------------------------------------------------

int main(int argc, char * const * argv)
{
    size_t canvas_height = 850;
    static struct option longopts[] = {
        {"size",    required_argument, nullptr, 's'},
        {nullptr,         0,                      NULL,           0 }
    };
    int ch;
    while ((ch = getopt_long(argc, argv, "s:", longopts, nullptr)) != -1) {
        switch (ch) {
          case 's':
              canvas_height = std::stoul(optarg);
              break;
          default:
              usage(argv[0]);
        }
    }
    if (argc == optind)
        usage(argv[0]);
    argc -= optind;
    argv += optind;

    draw(argv[0], canvas_height, static_cast<size_t>(canvas_height * 0.7070707));

    return 0;
}

// ----------------------------------------------------------------------

void usage(std::string progname)
{
    std::cerr << "Usage: " << progname << " [-m|--monospace] [-v|--height <canvas-height>] [-t|--text <text-to-show>] <output.pdf>" << std::endl;
    exit(1);

} // usage

// ----------------------------------------------------------------------

void draw(std::string filename, size_t canvas_height, size_t canvas_width)
{
    Surface s(filename, canvas_width, canvas_height);
    s.drawing_area(0.1, 10.34);

    s.line( 0,    0,    10.34, 0,    RED);
    s.line(10.34, 0,    10.34, 5.64, RED);
    s.line(10.34, 5.64,  0,    5.64, RED);
    s.line( 0,    5.64,  0,    0,    RED);

      // ----------------------------------------------------------------------
      // 2.OG

    room_work(s);
    heizung_2og(s);
    flur_2og(s);
    bad_2og(s);
    bett_2og(s);

} // draw

// ----------------------------------------------------------------------
