/**
 *    Copyright 2010-10-03 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#define __DS_GRAPHICS_BOX__hpp____by_Duzy_Chan__ 1
#       include <boost/geometry/geometries/box.hpp>
#       include "../point.hpp"

namespace ds { namespace graphics {

    namespace traits
    {
      enum 
        {
          box_index_left   = 0,
          box_index_top    = 1,
          box_index_right  = 2,
          box_index_bottom = 3,
          box_index_count  = 4
        };

      template<std::size_t K> class coord_access;
      template<> struct coord_access<box_index_left>
      {
        template<typename Box> static inline void get(Box const * b)           { b->min_corner().template get<0>(); }
        template<typename Box> static inline void set(Box * b, coordinate_t v) { b->min_corner().template set<0>(v); }
      };
      template<> struct coord_access<box_index_top>
      {
        template<typename Box> static inline void get(Box const * b)           { b->min_corner().template get<1>(); }
        template<typename Box> static inline void set(Box * b, coordinate_t v) { b->min_corner().template set<1>(v); }
      };
      template<> struct coord_access<box_index_right>
      {
        template<typename Box> static inline void get(Box const * b)           { b->max_corner().template get<0>(); }
        template<typename Box> static inline void set(Box * b, coordinate_t v) { b->max_corner().template set<0>(v); }
      };
      template<> struct coord_access<box_index_bottom>
      {
        template<typename Box> static inline void get(Box const * b)           { b->max_corner().template get<1>(); }
        template<typename Box> static inline void set(Box * b, coordinate_t v) { b->max_corner().template set<1>(v); }
      };
    }//namespace traits
    
    struct box : boost::geometry::model::box<point>
    {
      enum 
        {
          index_left   = traits::box_index_left,
          index_top    = traits::box_index_top,
          index_right  = traits::box_index_right,
          index_bottom = traits::box_index_bottom,
          index_count  = traits::box_index_count,

          index_x = index_left,
          index_y = index_top
        };

      box()
        : boost::geometry::model::box<point>()
      {}

      box(const point & min, const point & max)
        : boost::geometry::model::box<point>(min, max)
      {}

      template<std::size_t K>
      inline void set(coordinate_t v)
      {
        BOOST_STATIC_ASSERT(K < index_count);
        traits::coord_access<K>::set(this, v);
      }

      template<std::size_t K>
      inline void get() const
      {
        BOOST_STATIC_ASSERT(K < index_count);
        traits::coord_access<K>::get(this);
      }

      bool empty() const
      {
        return min_corner().x() >= max_corner().x()
          ||   min_corner().y() >= max_corner().y() ;
      }

      void set_empty()
      {
        set<0>(0);
        set<1>(0);
        set<2>(0);
        set<3>(0);
      }

      coordinate_t x() const { return min_corner().x(); }
      coordinate_t y() const { return min_corner().y(); }

      coordinate_t left() const   { return x(); }
      coordinate_t top() const    { return y(); }
      coordinate_t right() const  { return max_corner().x(); }
      coordinate_t bottom() const { return max_corner().y(); }

      void x(coordinate_t v)      { min_corner().x(v); }
      void y(coordinate_t v)      { min_corner().y(v); }

      void left(coordinate_t v)   { x(v); }
      void top(coordinate_t v)    { y(v); }
      void right(coordinate_t v)  { max_corner().x(v); }
      void bottom(coordinate_t v) { max_corner().y(v); }

      coordinate_t width() const  { return max_corner().x() - min_corner().x(); }
      coordinate_t height() const { return max_corner().y() - min_corner().y(); }
      void width(coordinate_t w)  { max_corner().x(w + min_corner().x()); }
      void height(coordinate_t h) { max_corner().y(h + min_corner().y()); }

      void resize(coordinate_t w, coordinate_t h) { width(w), height(h); }

      bool operator == ( const box & rhs ) const
      {
        return min_corner() == rhs.min_corner()
          &&   max_corner() == rhs.max_corner() ;
      }

      box intersect(const box & r) const
      {
        box o;
        o.set_empty();
        if ( r.empty() ) return o;
        o.left  ( left()   < r.left()   ? r.left() : left() );
        o.top   ( top()    < r.top()    ? r.top()  : top() );
        o.right ( right()  < r.right()  ? right()  : r.right() );
        o.bottom( bottom() < r.bottom() ? bottom() : r.bottom() );
        return o;
      }

      bool intersects(const box & o) const
      {
        return (!empty())
          && o.left() < right() && left() < o.right()
          && o.top() < bottom() && top() < o.bottom()
          ;
      }

      bool contains(const box & o) const
      {
        return (!empty())
          && left() <= o.left() && o.right() <= right()
          && top() <= o.top() && o.bottom() <= bottom()
          ;
      }
    };//struct box
    
  }//namespace graphics
}//namespace ds

namespace boost { namespace geometry { namespace traits {

      template<>
      struct tag<ds::graphics::box>
        : tag<model::box<ds::graphics::point> > {};

      template<>
      struct point_type<ds::graphics::box>
        : point_type<model::box<ds::graphics::point> > {};

      template<std::size_t Dimension>
      struct indexed_access<ds::graphics::box, min_corner, Dimension>
        : indexed_access<model::box<ds::graphics::point>, min_corner, Dimension> {};

      template<std::size_t Dimension>
      struct indexed_access<ds::graphics::box, max_corner, Dimension>
        : indexed_access<model::box<ds::graphics::point>, max_corner, Dimension> {};

    }}} // namespace boost::geometry::traits
