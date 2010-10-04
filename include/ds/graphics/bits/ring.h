/**
 *    Copyright 2010-10-03 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#define __DS_GRAPHICS_RING__hpp____by_Duzy_Chan__ 1
#       include <boost/geometry/geometries/linear_ring.hpp>
#       include "../point.hpp"

namespace ds { namespace graphics {

    struct ring : boost::geometry::linear_ring<point>
    {
    };
    
  }//namespace graphics
}//namespace ds

namespace boost { namespace geometry { namespace traits {

      template<>
      struct tag<ds::graphics::ring>
        : tag< linear_ring<ds::graphics::point> >
      {};

      template<>
      struct point_order<ds::graphics::ring>
        : point_order< linear_ring<ds::graphics::point> >
      {};

      template<>
      struct closure<ds::graphics::ring>
        : closure< linear_ring<ds::graphics::point> >
      {};

    }}} // namespace boost::geometry::traits

