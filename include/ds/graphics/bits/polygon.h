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

    typedef boost::geometry::model::polygon<point> polygon_base;

    struct polygon : polygon_base
    {
    };
    
  }//namespace graphics
}//namespace ds

namespace boost { namespace geometry { namespace traits {

      template<>
      struct tag<ds::graphics::polygon>
        : tag<ds::graphics::polygon_base> {};

      template<>
      struct ring_const_type<ds::graphics::polygon>
        : ring_const_type<ds::graphics::polygon_base> {};

      template<>
      struct ring_mutable_type<ds::graphics::polygon>
        : ring_mutable_type<ds::graphics::polygon_base> {};

      template<>
      struct interior_const_type<ds::graphics::polygon>
        : interior_const_type<ds::graphics::polygon_base> {};

      template<>
      struct interior_mutable_type<ds::graphics::polygon>
        : interior_mutable_type<ds::graphics::polygon_base> {};

      template<>
      struct exterior_ring<ds::graphics::polygon>
        : exterior_ring<ds::graphics::polygon_base> {};

      template<>
      struct interior_rings<ds::graphics::polygon>
        : interior_rings<ds::graphics::polygon_base> {};

    }}} // namespace boost::geometry::traits

