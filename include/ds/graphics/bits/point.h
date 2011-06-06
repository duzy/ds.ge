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
    typedef boost::geometry::model::d2::point_xy<coordinate_t, coordinate_system> point_base;

    struct point : point_base
    {
      point()
        : point_base(0, 0)
      {}

      point(coordinate_t x, coordinate_t y)
        : point_base(x, y)
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
        : tag<ds::graphics::point_base> {};

      template<>
      struct coordinate_type<ds::graphics::point>
        : coordinate_type<ds::graphics::point_base> {};

      template<>
      struct coordinate_system<ds::graphics::point>
        : coordinate_system<ds::graphics::point_base> {};

      template<>
      struct dimension<ds::graphics::point>
        : dimension<ds::graphics::point_base> {};

      template<std::size_t Dimension>
      struct access<ds::graphics::point, Dimension>
        : access<ds::graphics::point_base, Dimension> {};

    }}} // namespace boost::geometry::traits
