#pragma once

#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <string>
#include <vector>
#include <cmath>

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

class FontStyle
{
 public:
    static constexpr const char* default_family = "sans-serif";

    inline FontStyle() : cairo_family(default_family) {}
    inline FontStyle(std::string aFamily) { from_string(aFamily); }
    inline FontStyle& operator=(std::string aFamily) { from_string(aFamily); return *this; }

    inline operator const char*() const { return cairo_family.c_str(); }

      // to avoid saving to json, e.g. for settings.SettingsAATransition.TransitionData
    inline operator bool() const { return cairo_family != default_family; }

 private:
    std::string cairo_family;

    inline void from_string(std::string aFamily)
        {
            if (aFamily == "default")
                cairo_family = default_family;
            else
                cairo_family = aFamily;
        }
};

// ----------------------------------------------------------------------

class TextStyle
{
 public:
    inline TextStyle() : mSlant(CAIRO_FONT_SLANT_NORMAL), mWeight(CAIRO_FONT_WEIGHT_NORMAL) {}
    inline TextStyle(std::string aFontFamily) : mFontStyle(aFontFamily), mSlant(CAIRO_FONT_SLANT_NORMAL), mWeight(CAIRO_FONT_WEIGHT_NORMAL) {}

    inline const FontStyle& font_style() const { return mFontStyle; }
    inline cairo_font_slant_t slant() const { return mSlant; }
    inline cairo_font_weight_t weight() const { return mWeight; }

      // to avoid saving to json, e.g. for settings.SettingsAATransition.TransitionData
    inline operator bool() const { return mSlant != CAIRO_FONT_SLANT_NORMAL || mWeight != CAIRO_FONT_WEIGHT_NORMAL || !mFontStyle; }

 private:
    FontStyle mFontStyle;
    cairo_font_slant_t mSlant;
    cairo_font_weight_t mWeight;

}; // class TextStyle

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
        : mContext(nullptr), mWidth(width), mHeight(height), mScale(1),
          mWallWidth(5), mSizeLineWidth(0.5), mSizeTextSize(10)
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
            line(x1, y1, x2, y2, BLACK, mWallWidth);
        }

    inline void wall_with_door(double x1, double y1, double x2, double y2)
        {
            line(x1, y1, x2, y2, BLUE, mWallWidth, {mScale * 5, mScale * 5});
        }

    inline void wall_with_size(double x1, double y1, double x2, double y2, double x_off, double y_off)
        {
            const double size = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
            std::string tx = std::to_string(size);
            const auto ts = text_size(tx, mSizeTextSize);

            line(x1, y1, x2, y2, BLACK, mWallWidth);
            line(x1, y1, x1 + x_off, y1 + y_off, BLACK, mSizeLineWidth);
            line(x2, y2, x2 + x_off, y2 + y_off, BLACK, mSizeLineWidth);

            const double txx = (size - ts.first) / 2, txy = ts.second / 2;
            text(x1 + x_off + txx, y1 + y_off + txy, tx, BLACK, mSizeTextSize);
            line(x1 + x_off, y1 + y_off, x1 + x_off + txx * 0.95, y1 + y_off, BLACK, mSizeLineWidth);
            line(x2 + x_off, y2 + y_off, x1 + x_off + txx + ts.first, y2 + y_off, BLACK, mSizeLineWidth);
        }

    inline void line(double x1, double y1, double x2, double y2, Color color = BLACK, double aWidth = 1, const std::vector<double>& aDashes = std::vector<double>(), cairo_line_cap_t aLineCap = CAIRO_LINE_CAP_BUTT)
        {
            PushContext pc(*this);
            cairo_set_line_width(mContext, aWidth * mScale);
            set_source_rgba(color);
            cairo_set_line_cap(mContext, aLineCap);
            if (!aDashes.empty())
                cairo_set_dash(mContext, aDashes.data(), static_cast<int>(aDashes.size()), 0);
            cairo_move_to(mContext, x1, y1);
            cairo_line_to(mContext, x2, y2);
            cairo_stroke(mContext);
        }

    inline void text(double x, double y, std::string aText, Color aColor, double aSize, const TextStyle& aTextStyle = TextStyle(), double aRotation = 0)
        {
            PushContext pc(*this);
            prepare_for_text(aSize, aTextStyle);
            cairo_move_to(mContext, x, y);
            cairo_rotate(mContext, aRotation);
            set_source_rgba(aColor);
            cairo_show_text(mContext, aText.c_str());
        }

    std::pair<double, double> text_size(std::string aText, double aSize, const TextStyle& aTextStyle = TextStyle())
        {
            PushContext pc(*this);
            prepare_for_text(aSize, aTextStyle);
            cairo_text_extents_t text_extents;
            cairo_text_extents(mContext, aText.c_str(), &text_extents);
            // if (x_bearing != nullptr)
            //     *x_bearing = text_extents.x_bearing;
            //std::cerr << "text_size [" << aText << "] " << text_extents.x_advance << " " << - text_extents.y_bearing << std::endl;
            return std::make_pair(text_extents.x_advance, - text_extents.y_bearing);
        }

 private:
    cairo_t* mContext;
    double mWidth, mHeight, mScale;
    double mWallWidth, mSizeLineWidth, mSizeTextSize;
    friend class PushContext;

    inline void set_source_rgba(Color aColor) const
        {
            cairo_set_source_rgba(mContext, aColor.red(), aColor.green(), aColor.blue(), aColor.alpha());
        }

    inline void prepare_for_text(double aSize, const TextStyle& aTextStyle)
        {
            cairo_select_font_face(mContext, aTextStyle.font_style(), aTextStyle.slant(), aTextStyle.weight());
            cairo_set_font_size(mContext, aSize * mScale);
        }

};

// ----------------------------------------------------------------------
