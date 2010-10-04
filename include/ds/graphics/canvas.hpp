/**
 *    Copyright 2010-08-15 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifndef __DS_GRAPHICS_CANVAS_hpp____by_Duzy_Chan__
#define __DS_GRAPHICS_CANVAS_hpp____by_Duzy_Chan__ 1
#   include <boost/noncopyable.hpp>
#   include "visibility.hpp"

namespace ds { namespace graphics {

    struct image;
    struct color;

    struct point;
    struct segment;
    struct linestring;
    struct box;
    struct ring;
    struct polygon;

    struct drawing_tool;
    struct brush;
    struct pen;

    struct region; // bits/region.h

    class DS_GRAPHICS_PUBLIC canvas : boost::noncopyable
    {
      struct IMPL;

    public:
      canvas( image & );
      ~canvas();

      // struct clip_t
      // {
      //   clip_t & operator  = ( const box & ); //!< replace
      //   clip_t & operator -= ( const box & ); //!< subtract
      //   //clip_t & operator += ( const box & ); //!< same as '|='
      //   clip_t & operator |= ( const box & ); //!< inclusive-or
      //   clip_t & operator ^= ( const box & ); //!< exclusive-or
      //   clip_t & intersect( const box & ); //!< intersect
      // };//struct clip_t

      // /**
      //    myCanvas.clip() |= box1;
      //  */
      // clip_t clip();

      /**
       *  Replace the current clip.
       *  @{
       */
      bool clip( const box & );
      bool clip( const region & );
      bool clip( const ring & );
      bool clip( const polygon & );
      /** @} */

      //void save_clip();
      //void restore_clip();

      void render( const color & );
      void render( const image & );

      void render( const point &,       const brush & = default_brush() );
      void render( const box &,         const brush & = default_brush() );
      void render( const ring &,        const brush & = default_brush() );
      void render( const polygon &,     const brush & = default_brush() );
      //void render( const ds::ustring &, const brush & = default_brush() );

      void stroke( const point &,       const pen & = default_pen() );
      void stroke( const segment &,     const pen & = default_pen() );
      void stroke( const box &,         const pen & = default_pen() );
      void stroke( const ring &,        const pen & = default_pen() );
      void stroke( const polygon &,     const pen & = default_pen() );
      //void stroke( const ds::ustring &, const pen & = default_pen() );

      static brush & default_brush();
      static pen &   default_pen();

    private:
      IMPL * _imp;
    };//class canvas
    
  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_CANVAS_hpp____by_Duzy_Chan__
