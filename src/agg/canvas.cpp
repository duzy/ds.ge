/**
 *    Copyright 2010-08 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <code@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/graphics/canvas.hpp>
#include <ds/graphics/image.hpp>
#include <ds/graphics/box.hpp>
#include <ds/graphics/region.hpp>
#include <ds/graphics/segment.hpp>
#include <ds/graphics/line.hpp>
#include <ds/graphics/ring.hpp>
#include <ds/graphics/polygon.hpp>
#include <ds/graphics/color.hpp>
#include <ds/graphics/drawing_tools.hpp>
#include <agg/agg_basics.h>
#include <ds/debug.hpp>

/**
 *  agg pipline:
 *      Vectex Source -> Rasterizer -> Renderer
 *                                      |-> Scanline -> Span
 * 
 */

namespace ds { namespace graphics {

    struct canvas::IMPL
    {
      image *_image;

      IMPL(image &m) : _image(&m) {}
    };//struct canvas::IMPL

    canvas::canvas( image & img )
      : _imp( new IMPL(img) )
    {
    }

    canvas::~canvas()
    {
      delete _imp;
    }

    int canvas::sys_pixel_type()
    {
      return int( image::ABGR_8888_PIXEL );
    }

    bool canvas::clip( const box & g )
    {
      return false;
    }

    bool canvas::clip( const region & rgn )
    {
      return false;
    }

    bool canvas::clip( const ring & g )
    {
      return false;
    }

    bool canvas::clip( const polygon & g )
    {
      if ( g.outer().empty() ) return false;
      return false;
    }

    brush & canvas::default_brush()
    {
      static bool sInit = true;
      static brush sDefaultBrush;
      if (sInit) {
        sDefaultBrush.color = color::rgb(0, 0, 0);
        sDefaultBrush.anti_alias = true;
        sInit = false;
      }
      return sDefaultBrush;
    }

    pen & canvas::default_pen()
    {
      static bool sInit = true;
      static pen sDefaultPen;
      if (sInit) {
        sDefaultPen.color = color::rgb(0, 0, 0);
        sDefaultPen.anti_alias = true;
        sInit = false;
      }
      return sDefaultPen;
    }

    void canvas::render( const point & g, const brush & p )
    {
    }

    void canvas::render( const box & g, const brush & p )
    {
    }

    void canvas::render( const ring & g, const brush & p )
    {
    }

    void canvas::render( const polygon & g, const brush & p )
    {
    }//canvas::render

    void canvas::render( const color & c )
    {
    }

    void canvas::render( const image & img, coordinate_t x, coordinate_t y )
    {
    }

    void canvas::render( const std::string & s, coordinate_t x, coordinate_t y, const brush & b )
    {
    }

    void canvas::render( const std::wstring & s, coordinate_t x, coordinate_t y, const brush & b )
    {
    }

    void canvas::render( const ds::ustring & s, coordinate_t x, coordinate_t y, const brush & b )
    {
    }

    void canvas::stroke( const point & g, const pen & p )
    {
    }

    void canvas::stroke( const segment & g , const pen & p )
    {
    }

    void canvas::stroke( const line & g , const pen & p )
    {
    }

    void canvas::stroke( const ring & g, const pen & p )
    {
    }

    void canvas::stroke( const polygon & g, const pen & p )
    {
    }

    void canvas::stroke( const box & g, const pen & p )
    {
    }

    void canvas::stroke( const std::string & s, coordinate_t x, coordinate_t y, const pen & p )
    {
    }

    void canvas::stroke( const std::wstring & s, coordinate_t x, coordinate_t y, const pen & p )
    {
    }

    void canvas::stroke( const ds::ustring & s, coordinate_t x, coordinate_t y, const pen & p )
    {
    }

  }//namespace graphics
}//namespace ds
