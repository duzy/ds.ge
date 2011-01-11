/**
 *    Copyright 2010-09-21 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <ds/graphics/region.hpp>
#include <boost/geometry/algorithms/make.hpp>
#include <algorithm>
#include <iterator>
#include <stdint.h>
//#include <sys/types.h>
#include <limits>
#include <ostream>
#include <iostream>

namespace ds { namespace graphics {

    typedef coordinate_t VALUE;
    static inline VALUE max_value() { return std::numeric_limits<VALUE>::max(); }
    static inline VALUE min_value() { return std::numeric_limits<VALUE>::min(); }

    /* 
     * Common boolean operations:
     * value is computed as 0b101 op 0b110
     *    other boolean operation are possible, simply compute
     *    their corresponding value with the above formulae and use
     *    it when instantiating a region_operator.
     */
    static const uint32_t OPRAND_LHS = 0x5;  // 0b101
    static const uint32_t OPRAND_RHS = 0x6;  // 0b110
    enum {
      op_nand = OPRAND_LHS & ~OPRAND_RHS,
      op_and  = OPRAND_LHS &  OPRAND_RHS,
      op_or   = OPRAND_LHS |  OPRAND_RHS,
      op_xor  = OPRAND_LHS ^  OPRAND_RHS
    };

    // from android:frameworks/base/include/private/ui/RegionHelper.h
    template<typename RECT>
    class region_operator
    {
    public:
      struct region
      {
        RECT const* rects;
        size_t count;
        VALUE dx;
        VALUE dy;
        inline region(const region& rhs) 
          : rects(rhs.rects), count(rhs.count), dx(rhs.dx), dy(rhs.dy) { }
        inline region(RECT const* r, size_t c) 
          : rects(r), count(c), dx(), dy() { }
        inline region(RECT const* r, size_t c, VALUE dx, VALUE dy) 
          : rects(r), count(c), dx(dx), dy(dy) { }
      };

      inline region_operator(const region& lhs, const region& rhs, uint32_t op)
        : spanner(lhs, rhs)
        , OP(op)
      {
      }

      template<typename RegionRasterizer>
      void operator()(RegionRasterizer& rasterizer)
      {
        RECT current = boost::geometry::make<RECT>( 0, 0, 0, 0 );

        do {
          SpannerInner spannerInner(spanner.lhs, spanner.rhs);
          int inside = spanner.next(current); //.next(current.top, current.bottom)
          spannerInner.prepare(inside);
          do {
            int inside = spannerInner.next(current);//.next(current.left, current.right);
            if ((OP >> inside) & 1) {
              if (!current.empty()) {
                rasterizer(current);
              }
            }
          } while(!spannerInner.is_done());
        } while(!spanner.is_done());
      }

    private:    
      class SpannerBase
      {
      public:
        enum {
          lhs_before_rhs   = 0,
          lhs_after_rhs    = 1,
          lhs_coincide_rhs = 2
        };

      protected:
        VALUE lhs_head;
        VALUE lhs_tail;
        VALUE rhs_head;
        VALUE rhs_tail;

        inline int next(VALUE& head, VALUE& tail,
                        bool& more_lhs, bool& more_rhs) 
        {
          int inside;
          more_lhs = false;
          more_rhs = false;
          if (lhs_head < rhs_head) {
            inside = lhs_before_rhs;
            head = lhs_head;
            if (lhs_tail <= rhs_head) {
              tail = lhs_tail;
              more_lhs = true;
            } else {
              lhs_head = rhs_head;
              tail = rhs_head;
            }
          } else if (rhs_head < lhs_head) {
            inside = lhs_after_rhs;
            head = rhs_head;
            if (rhs_tail <= lhs_head) {
              tail = rhs_tail;
              more_rhs = true;
            } else {
              rhs_head = lhs_head;
              tail = lhs_head;
            }
          } else {
            inside = lhs_coincide_rhs;
            head = lhs_head;
            if (lhs_tail <= rhs_tail) {
              tail = rhs_head = lhs_tail;
              more_lhs = true;
            }
            if (rhs_tail <= lhs_tail) {
              tail = lhs_head = rhs_tail;
              more_rhs = true;
            }
          }
          return inside;
        }
      };//class SpannerBase

      class Spanner : protected SpannerBase 
      {
        friend class region_operator;
        region lhs;
        region rhs;

      public:
        inline Spanner(const region& lhs, const region& rhs)
          : lhs(lhs), rhs(rhs) 
        {
          SpannerBase::lhs_head = lhs.rects->top()      + lhs.dy;
          SpannerBase::lhs_tail = lhs.rects->bottom()   + lhs.dy;
          SpannerBase::rhs_head = rhs.rects->top()      + rhs.dy;
          SpannerBase::rhs_tail = rhs.rects->bottom()   + rhs.dy;
        }

        inline bool is_done() const
        {
          return !rhs.count && !lhs.count;
        }

        inline int next(RECT & b)//(VALUE& top, VALUE& bottom)
        {
          VALUE top( b.top() ), bottom( b.bottom() );
          bool more_lhs = false, more_rhs = false;
          int inside = SpannerBase::next(top, bottom, more_lhs, more_rhs);
          b.top(top), b.bottom(bottom);

          if (more_lhs) {
            advance(lhs, SpannerBase::lhs_head, SpannerBase::lhs_tail);
          }
          if (more_rhs) {
            advance(rhs, SpannerBase::rhs_head, SpannerBase::rhs_tail);
          }
          return inside;
        }

      private:
        static inline 
        void advance(region& reg, VALUE& aTop, VALUE& aBottom)
        {
          // got to next span
          size_t count = reg.count;
          RECT const * rects = reg.rects;
          RECT const * const end = rects + count;
          const int top = rects->top();
          while (rects != end && rects->top() == top) {
            rects++;
            count--;
          }
          if (rects != end) {
            aTop    = rects->top()    + reg.dy;
            aBottom = rects->bottom() + reg.dy;
          } else {
            aTop    = max_value();
            aBottom = max_value();
          }
          reg.rects = rects;
          reg.count = count;
        }
      };//class Spanner

      class SpannerInner : protected SpannerBase 
      {
        region lhs;
        region rhs;
        
      public:
        inline SpannerInner(const region& lhs, const region& rhs)
          : lhs(lhs), rhs(rhs) 
        {
        }

        inline void prepare(int inside) {
          if (inside == SpannerBase::lhs_before_rhs) {
            SpannerBase::lhs_head = lhs.rects->left()  + lhs.dx;
            SpannerBase::lhs_tail = lhs.rects->right() + lhs.dx;
            SpannerBase::rhs_head = max_value();
            SpannerBase::rhs_tail = max_value();
          } else if (inside == SpannerBase::lhs_after_rhs) {
            SpannerBase::lhs_head = max_value();
            SpannerBase::lhs_tail = max_value();
            SpannerBase::rhs_head = rhs.rects->left()  + rhs.dx;
            SpannerBase::rhs_tail = rhs.rects->right() + rhs.dx;
          } else {
            SpannerBase::lhs_head = lhs.rects->left()  + lhs.dx;
            SpannerBase::lhs_tail = lhs.rects->right() + lhs.dx;
            SpannerBase::rhs_head = rhs.rects->left()  + rhs.dx;
            SpannerBase::rhs_tail = rhs.rects->right() + rhs.dx;
          }
        }

        inline bool is_done() const {
          return SpannerBase::lhs_head == max_value() && 
            SpannerBase::rhs_head == max_value();
        }

        inline int next(RECT & b)//(VALUE& left, VALUE& right) 
        {
          VALUE left( b.left() ), right( b.right() );
          bool more_lhs = false, more_rhs = false;
          int inside = SpannerBase::next(left, right, more_lhs, more_rhs);
          b.left(left), b.right(right);

          if (more_lhs) {
            advance(lhs, SpannerBase::lhs_head, SpannerBase::lhs_tail);
          }
          if (more_rhs) {
            advance(rhs, SpannerBase::rhs_head, SpannerBase::rhs_tail);
          }
          return inside;
        }

      private:
        static inline 
        void advance(region& reg, VALUE& left, VALUE& right)
        {
          if (reg.rects && reg.count) {
            const int cur_span_top = reg.rects->top();
            reg.rects++;
            reg.count--;
            if (!reg.count || reg.rects->top() != cur_span_top) {
              left  = max_value();
              right = max_value();
            } else {
              left  = reg.rects->left()  + reg.dx;
              right = reg.rects->right() + reg.dx;
            }
          }
        }
      };//class SpannerInner

      Spanner spanner;
      uint32_t OP;
    };//class region_operator

    // This is our region rasterizer, which merges rects and spans together
    // to obtain an optimal region.
    class region::rasterizer
    {
      box& bounds;
      std::vector<box>& storage;

      box* head;
      box* tail;
      std::vector<box> span;
      box* current;

    public:
      rasterizer(region& reg) 
        : bounds(reg._bounds)
        , storage(reg._boxes)
        , head()
        , tail()
        , current()
      {
        bounds.top(0), bounds.bottom(0);
        bounds.left(max_value()), bounds.right(min_value());
        storage.clear();
      }

      ~rasterizer()
      {
        if (span.size()) {
          merge_span();
        }
        if (storage.size()) {
          bounds.top( storage.front().top() );
          bounds.bottom( storage.back().bottom() );
          if (storage.size() == 1) {
            storage.clear();
          }
        } else {
          bounds.left( 0 );
          bounds.right( 0 );
        }
      }
    
      void operator()(const box& rect)
      {
        //std::cout<<"add: ["<<rect.left()<<","<<rect.top()<<","<<rect.right()<<","<<rect.bottom()<<"]"<<std::endl;

        if (span.size()) {
          if (current->top() != rect.top()) {
            merge_span();
          } else if (current->right() == rect.left()) {
            current->right( rect.right() );
            return;
          }
        }
        span.push_back(rect);
        current = &span[0] + (span.size() - 1);
      }

    private:
      void merge_span()
      {
        bool merge = false;
        if (tail-head == ssize_t(span.size())) {
          box const* p = current;
          box const* q = head;
          if (p->top() == q->bottom()) {
            merge = true;
            while (q != tail) {
              if ((p->left() != q->left()) || (p->right() != q->right())) {
                merge = false;
                break;
              }
              p++, q++;
            }
          }
        }
        if (merge) {
          const int bottom = span[0].bottom();
          box* r = head;
          while (r != tail) {
            r->bottom( bottom );
            r++;
          }
        } else {
          bounds.left( std::min(span.front().left(), bounds.left()) );
          bounds.right( std::max(span.back().right(), bounds.right()) );
          std::copy( span.begin(), span.end(), std::back_inserter(storage) );
          tail = &storage[0] + storage.size();
          head = tail - span.size();
        }
        span.clear();
      }
    };//class region::rasterizer

    class region::operation
    {
      static box const * const get_array( const region & r, size_t * c )
      {
        if ( r._boxes.empty() ) {
          if (c) *c = 1;
          return &r._bounds;
        } else {
          if (c) *c = r._boxes.size();
          return &r._boxes[0];
        }
      }

    public:
      static void boolean(region& dst,
                          const region& lhs,
                          const region& rhs, int dx, int dy,
                          uint32_t OP)
      {
        size_t lhs_count;
        box const * const lhs_rects = get_array(lhs, &lhs_count);

        size_t rhs_count;
        box const * const rhs_rects = get_array(rhs, &rhs_count);

        /*typename*/ region_operator<box>::region lhs_region(lhs_rects, lhs_count);
        /*typename*/ region_operator<box>::region rhs_region(rhs_rects, rhs_count, dx, dy);
        region_operator<box> oper(lhs_region, rhs_region, OP);
        { // scope for rasterizer (dtor has side effects)
          rasterizer r(dst);
          oper(r);
        }
      }

      static void boolean(region& dst,
                          const region& lhs,
                          const box& rhs, int dx, int dy,
                          uint32_t OP)
      {
        size_t lhs_count;
        box const * const lhs_rects = get_array(lhs, &lhs_count);

        /*typename*/ region_operator<box>::region lhs_region(lhs_rects, lhs_count);
        /*typename*/ region_operator<box>::region rhs_region(&rhs, 1, dx, dy);
        region_operator<box> oper(lhs_region, rhs_region, OP);
        { // scope for rasterizer (dtor has side effects)
          rasterizer r(dst);
          oper(r);
        }
      }
    };//class region::operation

    //=======================================================================
    
    region::region()
      : _bounds()
      , _boxes()
    {
    }

    region::region( const region & o )
      : _bounds( o._bounds )
      , _boxes( o._boxes )
    {
    }

    region::region( const box & b )
      : _bounds(b)
      , _boxes()
    {
    }

    region & region::operator |= ( const box & rhs )
    {
      region lhs(*this);
      operation::boolean( *this, lhs, rhs, 0, 0, op_or );
      return *this;
    }

    region & region::operator &= ( const box & rhs )
    {
      region lhs(*this);
      operation::boolean( *this, lhs, rhs, 0, 0, op_and );
      return *this;
    }

    region & region::operator -= ( const box & rhs )
    {
      region lhs(*this);
      operation::boolean( *this, lhs, rhs, 0, 0, op_nand );
      return *this;
    }

    const region region::operator | ( const box & rhs ) const
    {
      region res;
      operation::boolean( res, *this, rhs, 0, 0, op_or );
      return res;
    }

    const region region::operator & ( const box & rhs ) const
    {
      region res;
      operation::boolean( res, *this, rhs, 0, 0, op_and );
      return res;
    }

    const region region::operator - ( const box & rhs ) const
    {
      region res;
      operation::boolean( res, *this, rhs, 0, 0, op_nand );
      return res;
    }

    region & region::operator |= ( const region & rhs )
    {
      region lhs(*this);
      operation::boolean( *this, lhs, rhs, 0, 0, op_or );
      return *this;
    }

    region & region::operator &= ( const region & rhs )
    {
      region lhs(*this);
      operation::boolean( *this, lhs, rhs, 0, 0, op_and );
      return *this;
    }

    region & region::operator -= ( const region & rhs )
    {
      region lhs(*this);
      operation::boolean( *this, lhs, rhs, 0, 0, op_nand );
      return *this;
    }

    const region region::operator | ( const region & rhs ) const
    {
      region res;
      operation::boolean( res, *this, rhs, 0, 0, op_or );
      return res;
    }

    const region region::operator & ( const region & rhs ) const
    {
      region res;
      operation::boolean( res, *this, rhs, 0, 0, op_and );
      return res;
    }

    const region region::operator - ( const region & rhs ) const
    {
      region res;
      operation::boolean( res, *this, rhs, 0, 0, op_nand );
      return res;
    }

    void region::dump( std::ostream & os ) const
    {
      os << "region {\n"
         << "  bounds: [ "
         <<       _bounds.left() << ","
         <<       _bounds.top() << ","
         <<       _bounds.right() << ","
         <<       _bounds.bottom()
         << " ]\n"
         << "  boxes: ["
        ;

      std::vector<box>::const_iterator it = _boxes.begin();
      for (; it != _boxes.end(); ++it ) {
        os << "\n"
           << "    [ " <<it->left()<<", "<<it->top()<<", "<<it->right()<<", "<<it->bottom()<<" ]"
          ;
      }

      if ( _boxes.empty() ) os <<"]\n";
      else os << "\n  ]\n";

      os << "}" ;
    }

  }//namespace graphics
}//namespace ds

