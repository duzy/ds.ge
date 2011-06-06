/**
 *    Copyright 2010-08 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
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
#include <skia/core/SkCanvas.h>
#include <skia/core/SkBitmap.h>
#include <skia/core/SkRect.h>
#include <skia/core/SkPath.h>
#include <skia/core/SkTypeface.h>
#include <skia/core/SkFontHost.h>
#include <ds/log.hpp>

namespace ds { namespace graphics {
    namespace
    {
      inline void to_SkPaint( SkPaint & paint, const drawing_tool & d )
      {
        paint.setAntiAlias( d.anti_alias ? true : false );
        paint.setARGB( d.color.alpha, d.color.red, d.color.green, d.color.blue );

        // TODO: more SkPain properties
      }

      inline void to_SkPaint( SkPaint & paint, const brush & b )
      {
        to_SkPaint( paint, reinterpret_cast<const drawing_tool&>(b) );

        paint.setStyle( SkPaint::kFill_Style );

        // TODO: more SkPain properties
      }

      inline void to_SkPaint( SkPaint & paint, const pen & p )
      {
        to_SkPaint( paint, reinterpret_cast<const drawing_tool&>(p) );

        paint.setStyle( SkPaint::kStroke_Style );

        // TODO: more SkPain properties
      }

      inline void to_SkBitmap( SkBitmap & bmp, const image & img )
      {
        SkBitmap::Config config = SkBitmap::kNo_Config;
        switch ( img.pixel_type() ) {
        case image::ARGB_8888_PIXEL:
        case image::ABGR_8888_PIXEL:
        case image::RGBA_8888_PIXEL:
        case image::BGRA_8888_PIXEL:
          config = SkBitmap::kARGB_8888_Config;
          break;
        case image::ARGB_4444_PIXEL:
        case image::ABGR_4444_PIXEL:
        case image::RGBA_4444_PIXEL:
        case image::BGRA_4444_PIXEL:
          config = SkBitmap::kARGB_4444_Config;
          break;
        case image::RGB_888_PIXEL:
        case image::BGR_888_PIXEL:
          TODO("handle with RGB_888 and BGR_888");
          dsE("canvas does not support RGB_888 and BGR_888");
          break;
        case image::RGB_565_PIXEL:
        case image::BGR_565_PIXEL:
          config = SkBitmap::kRGB_565_Config;
          break;
        default:
          //dsIm( false, "unsupported pixel format: "<<img.pixel_type()<<", "<<img.width()<<"x"<<img.height() );
          //break;
          dsE( "unsupported pixel format: "<<img.pixel_type()<<", "<<img.width()<<"x"<<img.height() );
          return;
        }

        bmp.setConfig( config, img.width(), img.height() );
        bmp.setPixels( (void*)img.pixels() );
      }

      template<typename Ring>
      inline void ring_to_SkPath( SkPath & skPath, const Ring & g )
      {
        ring::const_iterator it( g.begin() );
        skPath.moveTo(SkPoint::Make(it->x(), it->y()));
        for(++it; it != g.end(); ++it)
          skPath.lineTo(SkPoint::Make(it->x(), it->y()));
      }

      inline void to_SkRect( SkRect & rect, const box & b )
      {
        rect = SkRect::MakeLTRB(b.left(), b.top(), b.right(), b.bottom());
      }

      inline SkScalar to_SkScalar( coordinate_t v )
      {
        return SkFloatToScalar(v);
      }

      template<typename TChar> struct ChooseTextEncoding;
      template<> struct ChooseTextEncoding<std::string::value_type>
      {
        static const SkPaint::TextEncoding value = SkPaint::kUTF8_TextEncoding;
      };
      template<> struct ChooseTextEncoding<std::wstring::value_type>
      {
        static const SkPaint::TextEncoding value = SkPaint::kUTF16_TextEncoding;
      };
      /*
      template<> struct ChooseTextEncoding<ds::ustring::value_type>
      {
        static const SkPaint::TextEncoding value = SkPaint::kUTF16_TextEncoding;
      };
      */
    }//namespace

    //--------------------------------------------------------------------

    struct canvas::IMPL
    {
      image * _image;

      SkBitmap _skBitmap;
      SkCanvas _skCanvas;

      IMPL( image & img )
        : _image( &img )
      {
        to_SkBitmap(_skBitmap, img), _skCanvas.setBitmapDevice(_skBitmap);
      }

      template<typename G, typename D>
      inline void draw( const G & g, const D & d )
      {
        SkPaint paint;
        to_SkPaint( paint, d ), this->draw( g, paint );
      }

      template<typename TChar, typename D>
      inline void draw( const std::basic_string<TChar> & s, coordinate_t x, coordinate_t y, const D & d )
      {
        SkPaint paint;
        SkTypeface *typeface = NULL;
        typeface = SkTypeface::CreateFromFile("t/foo.ttf");
        //typeface = SkFontHost::CreateTypefaceFromFile("./foo.ttf");
        //typeface = SkFontHost::CreateTypeface(typeface, "sans", SkTypeface::kBoldItalic);

        paint.setTypeface(typeface);
        paint.setTextEncoding(ChooseTextEncoding<TChar>::value);

        SkScalar sx = to_SkScalar(x), sy = to_SkScalar(y);
        to_SkPaint( paint, d ), this->draw( s, sx, sy, paint );
      }

      void draw( const point & g,       const SkPaint & p );
      void draw( const segment & g,     const SkPaint & p );
      void draw( const line & g,        const SkPaint & p );
      void draw( const box & g,         const SkPaint & p );
      void draw( const ring & g,        const SkPaint & p );
      void draw( const polygon & g,     const SkPaint & p );

      template<typename TChar>
      void draw( const std::basic_string<TChar> & s, SkScalar x, SkScalar y, const SkPaint & p );
    };//struct canvas::IMPL

    template<typename TChar>
    void canvas::IMPL::draw( const std::basic_string<TChar> & s, SkScalar x, SkScalar y, const SkPaint & paint )
    {
      const int bytes = s.size() * sizeof(s[0]);
      _skCanvas.drawText( s.c_str(), bytes, x, y, paint );
    }

    void canvas::IMPL::draw( const point & g, const SkPaint & p )
    {
      _skCanvas.drawPoint(g.x(), g.y(), p);
    }

    void canvas::IMPL::draw( const segment & g, const SkPaint & p )
    {
      _skCanvas.drawLine(g.first .x(), g.first .y(),
                         g.second.x(), g.second.y(),
                         p);
    }

    void canvas::IMPL::draw( const line & g, const SkPaint & p )
    {
      _skCanvas.drawLine(g.first .x(), g.first .y(),
                         g.second.x(), g.second.y(),
                         p);
    }

    void canvas::IMPL::draw( const box & g, const SkPaint & p )
    {
      SkRect r;
      to_SkRect(r, g), _skCanvas.drawRect(r, p);
    }

    void canvas::IMPL::draw( const ring & g, const SkPaint & skPaint )
    {
      if ( g.empty() ) return;

      SkPath skPath;
      ring_to_SkPath( skPath, g ), _skCanvas.drawPath(skPath, skPaint);
    }

    void canvas::IMPL::draw( const polygon & g, const SkPaint & skPaint )
    {
      if ( g.outer().empty() ) return;

      SkPath skPath;
      ring_to_SkPath( skPath, g.outer() );
      _skCanvas.drawPath(skPath, skPaint);

      if (g.inners().empty()) return;

      polygon::inner_container_type::const_iterator inner;
      for( inner  = g.inners().begin();
           inner != g.inners().end(); ++inner ) {
        if ( inner->empty() ) continue;

        skPath.rewind(), ring_to_SkPath( skPath, *inner );

        _skCanvas.drawPath(skPath, skPaint);
      }//foreach inner
    }

    //////////////////////////////////////////////////////////////////////

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
      SkRect r;
      to_SkRect(r, g);
      return _imp->_skCanvas.clipRect(r, SkRegion::kReplace_Op);
    }

    bool canvas::clip( const region & rgn )
    {
      if ( rgn.empty() ) return false;

      dsI( 0 < rgn.size() );

      SkRegion skRgn;
      SkIRect * rects = new SkIRect[rgn.size()];
      skRgn.setRects( rects, rgn.size() );
      delete [] rects;
      return _imp->_skCanvas.clipRegion(skRgn, SkRegion::kReplace_Op);
    }

    bool canvas::clip( const ring & g )
    {
      if ( g.empty() ) return false;
      SkPath skPath;
      ring_to_SkPath( skPath, g );
      return _imp->_skCanvas.clipPath(skPath, SkRegion::kReplace_Op);
    }

    bool canvas::clip( const polygon & g )
    {
      if ( g.outer().empty() ) return false;

      SkPath skPath;
      ring_to_SkPath( skPath, g.outer() );
      if ( !_imp->_skCanvas.clipPath(skPath, SkRegion::kReplace_Op) )
        return false;

      if ( g.inners().empty() ) return true;

      polygon::inner_container_type::const_iterator it = g.inners().begin();
      for (; it != g.inners().end(); ++it) {
        skPath.rewind(), ring_to_SkPath( skPath, *it );
        _imp->_skCanvas.clipPath(skPath, SkRegion::kDifference_Op);
      }

      return true;
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
      _imp->draw( g, p );
    }

    void canvas::render( const box & g, const brush & p )
    {
      _imp->draw( g, p );
    }

    void canvas::render( const ring & g, const brush & p )
    {
      _imp->draw( g, p );
    }

    void canvas::render( const polygon & g, const brush & p )
    {
      _imp->_skCanvas.save( SkCanvas::kClip_SaveFlag );

      clip( g ), _imp->draw( g, p );

      _imp->_skCanvas.restore();
    }//canvas::render

    void canvas::render( const color & c )
    {
      _imp->_skCanvas.drawARGB( c.alpha, c.red, c.green, c.blue );
    }

    void canvas::render( const image & img, coordinate_t x, coordinate_t y )
    {
      SkBitmap bmp;
      to_SkBitmap( bmp, img ), _imp->_skCanvas.drawBitmap( bmp, x, y );
    }

    void canvas::render( const std::string & s, coordinate_t x, coordinate_t y, const brush & b )
    {
      _imp->draw( s, x, y, b );
    }

    void canvas::render( const std::wstring & s, coordinate_t x, coordinate_t y, const brush & b )
    {
      _imp->draw( s, x, y, b );
    }

    /*
    void canvas::render( const ds::ustring & s, coordinate_t x, coordinate_t y, const brush & b )
    {
      _imp->draw( s, x, y, b );
    }
    */

    void canvas::stroke( const point & g, const pen & p )
    {
      _imp->draw( g, p );
    }

    void canvas::stroke( const segment & g , const pen & p )
    {
      _imp->draw( g, p );
    }

    void canvas::stroke( const line & g , const pen & p )
    {
      _imp->draw( g, p );
    }

    void canvas::stroke( const ring & g, const pen & p )
    {
      _imp->draw( g, p );
    }

    void canvas::stroke( const polygon & g, const pen & p )
    {
      _imp->draw( g, p );
    }

    void canvas::stroke( const box & g, const pen & p )
    {
      _imp->draw( g, p );
    }

    void canvas::stroke( const std::string & s, coordinate_t x, coordinate_t y, const pen & p )
    {
      _imp->draw( s, x, y, p );
    }

    void canvas::stroke( const std::wstring & s, coordinate_t x, coordinate_t y, const pen & p )
    {
      _imp->draw( s, x, y, p );
    }

    /*
    void canvas::stroke( const ds::ustring & s, coordinate_t x, coordinate_t y, const pen & p )
    {
      _imp->draw( s, x, y, p );
    }
    */

  }//namespace graphics
}//namespace ds
