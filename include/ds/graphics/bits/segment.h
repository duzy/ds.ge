/**
 *    Copyright 2010-10-03 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#define __DS_GRAPHICS_SEGMENT__hpp____by_Duzy_Chan__ 1
#       include <boost/geometry/geometries/segment.hpp>
#       include "../point.hpp"

namespace ds { namespace graphics {

    struct segment : boost::geometry::segment<point>
    {
      explicit segment(point & a, point & b)
        : boost::geometry::segment<point>(a, b)
      {}
    };//struct segment

  }//namespace graphics
}//namespace ds

namespace boost { namespace geometry { namespace traits {

      template<>
      struct tag<ds::graphics::segment>
        : tag<segment<ds::graphics::point> >
      {};

      template<>
      struct point_type<ds::graphics::segment>
        : point_type<segment<ds::graphics::point> >
      {};

      template<std::size_t Dimension>
      struct indexed_access<ds::graphics::segment, 0, Dimension>
        : indexed_access<segment<ds::graphics::point>, 0, Dimension>
      {};

      template<std::size_t Dimension>
      struct indexed_access<ds::graphics::segment, 1, Dimension>
        : indexed_access<segment<ds::graphics::point>, 1, Dimension>
      {};

    }}} // namespace boost::geometry::traits

