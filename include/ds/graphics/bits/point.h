/**
 *    Copyright 2010-10-03 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#define __DS_GRAPHICS_POINT__hpp____by_Duzy_Chan__ 1
#       include <boost/geometry/geometries/point_xy.hpp>

namespace ds { namespace graphics {

    typedef float coordinate_t;
    typedef boost::geometry::cs::cartesian coordinate_system;
    typedef boost::geometry::point_xy<coordinate_t, coordinate_system> base_point;

    struct point : base_point
    {
      point()
        : base_point(0, 0)
      {}

      point(coordinate_t x, coordinate_t y)
        : base_point(x, y)
      {}

      bool operator == ( const point & rhs ) const
      {
        return x() == rhs.x() && y() == rhs.y() ;
      }
    };
    
  }//namespace graphics
}//namespace ds

namespace boost { namespace geometry { namespace traits {

      template<>
      struct tag<ds::graphics::point>
        : tag<ds::graphics::base_point> {};

      template<>
      struct coordinate_type<ds::graphics::point>
        : coordinate_type<ds::graphics::base_point> {};

      template<>
      struct coordinate_system<ds::graphics::point>
        : coordinate_system<ds::graphics::base_point> {};

      template<>
      struct dimension<ds::graphics::point>
        : dimension<ds::graphics::base_point> {};

      template<std::size_t Dimension>
      struct access<ds::graphics::point, Dimension>
        : access<ds::graphics::base_point, Dimension> {};

    }}} // namespace boost::geometry::traits

