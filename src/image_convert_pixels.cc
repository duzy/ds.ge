/**                                                                -*- c++ -*-
 *    Copyright DuzySoft.com, by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    All rights reserved by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 **/

#include <ds/graphics/image.hpp>
#include <ds/graphics/gil/image.hpp>
#include <ds/debug.hpp>
#ifdef PRINT_DEMANGLE
#  include <typeinfo>
#  include <cxxabi.h>
#endif//PRINT_DEMANGLE

/*
int status;
std::clog
<<"  "<<abi::__cxa_demangle(typeid(v1).name(), 0, 0, &status)<<std::endl
<<"  "<<abi::__cxa_demangle(typeid(v2).name(), 0, 0, &status)<<std::endl
;
*/

namespace ds { namespace graphics {

# ifndef NO_METHOD_convert_pixels

    namespace
    {
      struct pixels_converter
      {
        typedef bool result_type;

        template<
          template<typename> class Iterator1,
          template<typename> class Iterator2,
          template<typename> class Locator1,
          template<typename> class Locator2,
          typename VT1, typename Layout1,
          typename VT2, typename Layout2
          >
        result_type operator()
        ( const boost::gil::image_view<Locator1<Iterator1<boost::gil::pixel<VT1,Layout1>*> > > & v1,
          const boost::gil::image_view<Locator2<Iterator2<boost::gil::pixel<VT2,Layout2>*> > > & v2
          ) /*const*/
        {
          boost::gil::copy_and_convert_pixels( v1, v2 );
          return true;
        }

        template<
          template<typename> class Iterator1,
          template<typename> class Iterator2,
          template<typename> class Locator1,
          template<typename> class Locator2,
          typename VT1, typename PF1, typename Layout1,
          typename VT2, typename PF2, typename Layout2
          >
        result_type operator()
        ( const boost::gil::image_view<Locator1<Iterator1<boost::gil::packed_pixel<VT1, PF1, Layout1>*> > > & v1,
          const boost::gil::image_view<Locator2<Iterator2<boost::gil::packed_pixel<VT2, PF2, Layout2>*> > > & v2
          ) /*const*/
        {
          return false;
        }

        template<
          template<typename> class Iterator1,
          template<typename> class Iterator2,
          template<typename> class Locator1,
          template<typename> class Locator2,
          typename VT1, typename PF, typename Layout1,
          typename VT2,              typename Layout2
          >
        result_type operator()
        ( const boost::gil::image_view<Locator1<Iterator1<boost::gil::packed_pixel<VT1, PF, Layout1>*> > > & v1,
          const boost::gil::image_view<Locator2<Iterator2<boost::gil::       pixel<VT2,     Layout2>*> > > & v2
          ) /*const*/
        {
          return false;
        }

        template<
          template<typename> class Iterator1,
          template<typename> class Iterator2,
          template<typename> class Locator1,
          template<typename> class Locator2,
          typename VT1,              typename Layout1,
          typename VT2, typename PF, typename Layout2
          >
        result_type operator()
        ( const boost::gil::image_view<Locator1<Iterator1<boost::gil::       pixel<VT1,     Layout1>*> > > & v1,
          const boost::gil::image_view<Locator2<Iterator2<boost::gil::packed_pixel<VT2, PF, Layout2>*> > > & v2
          ) /*const*/
        {
          return false;
        }
      };//struct pixels_converter
    }//namespace

    bool image::convert_pixels( PixelType pt )
    {
      if (pixel_type() == pt) return false;

      image t;
      this->swap( t );

      dsI( t._m != 0 || t._v != 0 );
      dsI( this->_m == 0 && this->_v == 0 );

      if (pt == NO_PIXEL) return true;

      bool ok = this->create( t.width(), t.height(), pt );
      dsI( ok );

      gil::any_image_t::view_t v1 = t._isView ? t._v->any()
        : boost::gil::view( t._m->any() );
      gil::any_image_t::view_t v2 = this->_isView ? this->_v->any()
        : boost::gil::view( this->_m->any() );

#     if 0
      boost::gil::copy_and_convert_pixels( v1, v2 );
      return true;
#     else
      return boost::gil::apply_operation( v1, v2, pixels_converter() );
#     endif
    }

# endif//NO_METHOD_convert_pixels

  }//namespace graphics
}//namespace ds
