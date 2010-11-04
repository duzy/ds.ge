/**
 *    Copyright 2010-10-03 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#define __DS_GRAPHICS_LINE__hpp____by_Duzy_Chan__ 1
#       include "point.hpp"
#       include <boost/geometry/geometries/concepts/point_concept.hpp>

namespace ds { namespace graphics {

    struct line
    {
      point first, second;

      inline line()
        : first (0, 0)
        , second(0, 0)
      {}
      

      inline line(point& p1, point& p2)
        : first (p1)
        , second(p2)
      {}
    };//struct line
    
  }//namespace graphics
}//namespace

//////////////////////////////////////////////////////////////////////

namespace boost { namespace geometry { namespace traits {

      template <>
      struct tag<ds::graphics::line>
      {
        typedef segment_tag type;
      };

      template <>
      struct point_type<ds::graphics::line>
      {
        typedef ds::graphics::point type;
      };

      template <std::size_t Dimension>
      struct indexed_access<ds::graphics::line, 0, Dimension>
      {
        typedef ds::graphics::line segment_type;
        typedef typename geometry::coordinate_type<segment_type>::type coordinate_type;

        static inline coordinate_type get(segment_type const& s)
        {
          return geometry::get<Dimension>(s.first);
        }

        static inline void set(segment_type& s, coordinate_type const& value)
        {
          geometry::set<Dimension>(s.first, value);
        }
      };

      template <std::size_t Dimension>
      struct indexed_access<ds::graphics::line, 1, Dimension>
      {
        typedef ds::graphics::line segment_type;
        typedef typename geometry::coordinate_type<segment_type>::type coordinate_type;

        static inline coordinate_type get(segment_type const& s)
        {
          return geometry::get<Dimension>(s.second);
        }

        static inline void set(segment_type& s, coordinate_type const& value)
        {
          geometry::set<Dimension>(s.second, value);
        }
      };

    }}}// namespace boost::geometry::traits
