#pragma once

#include "cairo.hh"

// ----------------------------------------------------------------------

class SurfaceError : public std::runtime_error
{
 public: using std::runtime_error::runtime_error;
};

// ----------------------------------------------------------------------

class Color
{
 public:
    inline Color() : mColor(0xFF00FF) {}
    template <typename Uint, typename std::enable_if<std::is_integral<Uint>::value>::type* = nullptr> constexpr inline Color(Uint aColor) : mColor(static_cast<uint32_t>(aColor)) {}
    template <typename Uint, typename std::enable_if<std::is_integral<Uint>::value>::type* = nullptr> inline Color& operator=(Uint aColor) { mColor = static_cast<uint32_t>(aColor); return *this; }

    inline bool operator == (const Color& aColor) const { return mColor == aColor.mColor; }
    inline bool operator != (const Color& aColor) const { return ! operator==(aColor); }

    inline double alpha() const { return double(0xFF - ((mColor >> 24) & 0xFF)) / 255.0; }
    inline double red() const { return double((mColor >> 16) & 0xFF) / 255.0; }
    inline double green() const { return double((mColor >> 8) & 0xFF) / 255.0; }
    inline double blue() const { return double(mColor & 0xFF) / 255.0; }

    inline size_t alphaI() const { return static_cast<size_t>((mColor >> 24) & 0xFF); }
    inline void alphaI(uint32_t v) { mColor = (mColor & 0xFFFFFF) | ((v & 0xFF) << 24); }
    inline size_t rgbI() const { return static_cast<size_t>(mColor & 0xFFFFFF); }

 private:
    uint32_t mColor; // 4 bytes, most->least significant: transparency-red-green-blue, 0x00FF0000 - opaque red, 0xFF000000 - fully transparent

}; // class Color

constexpr const Color BLACK = 0;
constexpr const Color WHITE = 0xFFFFFF;
constexpr const Color GREY = 0xA0A0A0;
constexpr const Color LIGHT_GREY = 0xE0E0E0;
constexpr const Color RED = 0xFF0000;
constexpr const Color GREEN = 0x00FF00;
constexpr const Color BLUE = 0x0000FF;
constexpr const Color PINK = 0xFFC0CB;
constexpr const Color TRANSPARENT = 0xFF000000;

// ----------------------------------------------------------------------

class Surface
{
 private:
    class PushContext
    {
     public:
        inline PushContext(Surface& aSurface) : mContext(aSurface.mContext) { cairo_save(mContext); }
        inline ~PushContext() { cairo_restore(mContext); }

     private:
        cairo_t* mContext;
    };

 public:
    inline Surface(std::string filename, size_t width, size_t height)
        : mContext(nullptr), mWidth(width), mHeight(height), mScale(1)
        {
            auto surface = cairo_pdf_surface_create(filename.empty() ? nullptr : filename.c_str(), width, height);
            if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS)
                throw SurfaceError("cannot create pdf surface");
            mContext = cairo_create(surface);
            cairo_surface_destroy(surface);
            if (cairo_status(mContext) != CAIRO_STATUS_SUCCESS)
                throw SurfaceError("cannot create mContext");
        }

    inline ~Surface()
        {
            if (mContext != nullptr)
                cairo_destroy(mContext);
        }

    inline void drawing_area(double padding, double width)
        {
            const double space = mWidth * padding;
            cairo_translate(mContext, space, space);
            const double scale = (mWidth - space * 2.0) / width;
            mScale = 1.0 / scale;
            cairo_scale(mContext, scale, scale);
        }

    inline void wall(double x1, double y1, double x2, double y2)
        {
            line(x1, y1, x2, y2, BLACK, 1);
        }

    inline void wall_with_door(double x1, double y1, double x2, double y2)
        {
            line(x1, y1, x2, y2, GREY, 1);
        }

    inline void line(double x1, double y1, double x2, double y2, Color color = BLACK, double aWidth = 1, cairo_line_cap_t aLineCap = CAIRO_LINE_CAP_BUTT)
        {
            PushContext pc(*this);
            cairo_set_line_width(mContext, aWidth * mScale);
            set_source_rgba(color);
            cairo_set_line_cap(mContext, aLineCap);
            cairo_move_to(mContext, x1, y1);
            cairo_line_to(mContext, x2, y2);
            cairo_stroke(mContext);
        }

 private:
    cairo_t* mContext;
    double mWidth, mHeight, mScale;
    friend class PushContext;

    inline void set_source_rgba(Color aColor) const
        {
            cairo_set_source_rgba(mContext, aColor.red(), aColor.green(), aColor.blue(), aColor.alpha());
        }

};

// ----------------------------------------------------------------------
