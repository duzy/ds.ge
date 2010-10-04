/**
 *    Copyright 2010-10-03 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#define __DS_GRAPHICS_REGION__hpp____by_Duzy_Chan__ 1
#       include "../box.hpp"
#       include <vector>
#       include <iosfwd>
#       include "../visibility.hpp"

namespace ds { namespace graphics {
    
    struct DS_GRAPHICS_PUBLIC region
    {
      region();
      region( const region & );
      explicit region( const box & );

      const box & bounds() const { return _bounds; }
      const std::vector<box> & boxes() const { return _boxes; }

      bool empty() const { return _bounds.empty() && _boxes.empty(); }

      typedef const box * const_iterator;
      const_iterator begin() const { return _boxes.empty() ? (&_bounds) : (&_boxes[0]); }
      const_iterator end() const { return _boxes.empty() ? (&_bounds+1) : (&_boxes[0]+_boxes.size()); }
      std::size_t size() const { return end() - begin(); }

      void clear() { _boxes.clear(), _bounds.set_empty(); }

      region & operator |= ( const box & );
      region & operator &= ( const box & );
      region & operator -= ( const box & );

      const region operator | ( const box & ) const;
      const region operator & ( const box & ) const;
      const region operator - ( const box & ) const;

      region & operator |= ( const region & );
      region & operator &= ( const region & );
      region & operator -= ( const region & );

      const region operator | ( const region & ) const;
      const region operator & ( const region & ) const;
      const region operator - ( const region & ) const;

      void dump( std::ostream & ) const;

    private:
      class rasterizer;
      template<int> class operation;

      box _bounds;
      std::vector<box> _boxes;
    };//struct region
    
  }//namespace graphics
}//namespace ds

