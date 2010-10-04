/**
 *    Copyright 2010-07-30 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifndef __DS_GRAPHICS_COLOR__hpp____by_Duzy_Chan__
#define __DS_GRAPHICS_COLOR__hpp____by_Duzy_Chan__ 1

namespace ds { namespace graphics {

    struct color
    {
      uint8_t red;
      uint8_t green;
      uint8_t blue;
      uint8_t alpha;

      static inline color black()
      {
        color c;
        c.red = c.green = c.blue = 0, c.alpha = 255;
        return c;
      }

      static inline color white()
      {
        color c;
        c.red = c.green = c.blue = c.alpha = 255;
        return c;
      }

      // TODO: more named colors

      static inline color rgba( float r, float g, float b, float a )
      {
        color c;
        c.red   = ( uint32_t(0xFF * r) & 0xFF );
        c.green = ( uint32_t(0xFF * g) & 0xFF );
        c.blue  = ( uint32_t(0xFF * b) & 0xFF );
        c.alpha = ( uint32_t(0xFF * a) & 0xFF );
        return c;
      }

      static inline color argb( float a, float r, float g, float b )
      {
        return rgba( r, g, b, a );
      }

    };//struct color
    
  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_COLOR__hpp____by_Duzy_Chan__
