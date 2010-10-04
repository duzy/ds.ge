/**
 *    Copyright 2010-10-03 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#define __DS_GRAPHICS_POLYGON__hpp____by_Duzy_Chan__ 1
#       include <boost/geometry/geometries/polygon.hpp>
#       include "../point.hpp"

namespace ds { namespace graphics {

    struct polygon : boost::geometry::polygon<point>
    {
    };
    
  }//namespace graphics
}//namespace ds

namespace boost { namespace geometry { namespace traits {

      template<>
      struct tag<ds::graphics::polygon>
        : tag< polygon<ds::graphics::point> > {};

      template<>
      struct ring_type<ds::graphics::polygon>
        : ring_type< polygon<ds::graphics::point> > {};

      template<>
      struct interior_type<ds::graphics::polygon>
        : interior_type< polygon<ds::graphics::point> > {};

      template<>
      struct exterior_ring<ds::graphics::polygon>
        : exterior_ring< polygon<ds::graphics::point> > {};

      template<>
      struct interior_rings<ds::graphics::polygon>
        : interior_rings< polygon<ds::graphics::point> > {};

    }}} // namespace boost::geometry::traits

