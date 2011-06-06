/**                                                                -*- c++ -*-
 *    Copyright 2009-03-22 DuzySoft.com, by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    All rights reserved by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 **/

#include <ds/graphics/image.hpp>
#include <ds/graphics/gil/image.hpp>
#include <ds/log.hpp>
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

# ifdef NO_METHOD_convert_pixels
#   include "image_metafuns.h"

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
          ) const
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
          ) const
        {
          // TODO: convert pixels...
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
          ) const
        {
          // TODO: convert pixels...
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
          ) const
        {
          // TODO: convert pixels...
          return false;
        }
      };//struct pixels_converter

      /////////////////////////////////////////////////////////////////////

      template<image::PixelType PT>
      struct converter
      {
        typedef bool result_type;
        typedef typename gil_image<PT>::type gil_image_t;
        
        int width, height;
        gil_image_t & output;

        converter(int w, int h, gil_image_t & o) : width(w), height(h), output(o) {}

        template<
          template<typename> class Iterator,
          template<typename> class Locator,
          template<typename,typename> class Pixel,
          typename VT, typename Layout >
        result_type operator()( const boost::gil::image_view<Locator<Iterator<Pixel<VT,Layout>*> > > & v ) /*const*/
        {
          dsI( output.width() == 0 );
          dsI( output.height() == 0 );
          output.recreate(width, height);
          boost::gil::copy_and_convert_pixels( v, boost::gil::view(output) );
          return (output.width() == width) && (output.height() == height);
        }

        template<
          template<typename> class Iterator,
          template<typename> class Locator,
          template<typename,typename,typename> class PackedPixel,
          typename VT, typename PF, typename Layout >
        result_type operator()( const boost::gil::image_view<Locator<Iterator<PackedPixel<VT,PF,Layout>*> > > & v ) /*const*/
        {
          // TODO: convert packed pixels 
          return false;
        }
      };//struct converter<>

      template<> struct converter<image::NO_PIXEL>
      {
        typedef bool result_type;
        template<typename V> result_type operator()(const V &) { return false; }
      };//struct converter<image::NO_PIXEL>

    }//namespace

#   define HANDLE_PIXEL_TYPE(PIXEL_TYPE,GIL_IMAGE_TYPE)             \
    template<> bool image::convert<image::PIXEL_TYPE>()             \
    {                                                               \
      dsI( PIXEL_TYPE != NO_PIXEL );                                \
                                                                    \
      const PixelType pt = this->pixel_type();                      \
      if (pt == PIXEL_TYPE) return false;                           \
      if (pt == NO_PIXEL  ) return false;                           \
      dsI( this->_m != NULL || this->_v != NULL );                  \
                                                                    \
      image t; this->swap(t);                                       \
      dsI( t._m     != NULL && t._v     != NULL );                  \
      dsI( this->_m == NULL && this->_v == NULL );                  \
                                                                    \
      bool ok = this->create( t.width(), t.height(), PIXEL_TYPE );      \
      dsI( ok );                                                        \
                                                                        \
      gil::any_image_t::view_t v1 = t._isView ? t._v->any() : t._m->any_view(); \
      gil::any_image_t::view_t v2 = this->_isView ? this->_v->any() : this->_m->any_view(); \
      return boost::gil::apply_operation( v1, v2, pixels_converter() ); \
    }

    // TODO: reduce a copy-construction produced by 'new gil::image(output)'
    // TODO: refactor this template
#   define HANDLE_PIXEL_TYPE_(PIXEL_TYPE,GIL_IMAGE_TYPE)            \
    template<> bool image::convert<image::PIXEL_TYPE>()             \
    {                                                               \
      const PixelType pt = this->pixel_type();                      \
      if (pt == PIXEL_TYPE) return false;                           \
      if (pt == NO_PIXEL  ) return false;                           \
      dsI( this->_m != NULL || this->_v != NULL );                  \
                                                                    \
      image t; this->swap(t);                                       \
      dsI( t._m     != NULL && t._v     != NULL );                  \
      dsI( this->_m == NULL && this->_v == NULL );                  \
                                                                    \
      gil::any_image_t::view_t vin = t._isView                      \
        ? t._v->any() : t._m->any_view();                           \
                                                                    \
      GIL_IMAGE_TYPE output;                                        \
      converter<image::PIXEL_TYPE> conv( t.width(), t.height(), output ); \
      if (boost::gil::apply_operation( vin, conv )) {                   \
        dsI(output.width() == t.width());                               \
        dsI(output.height() == t.height());                             \
        dsI(this->_m == NULL);                                          \
        this->_isView = 0;                                              \
        this->_m = new gil::image(output);                              \
        return true;                                                    \
      }                                                                 \
      return false;                                                     \
    }
    HANDLE_ALL_PIXEL_TYPES()
#   undef HANDLE_PIXEL_TYPE

    template<> bool image::convert<image::NO_PIXEL>()
    {
      if (pixel_type() == NO_PIXEL) return false;
      image t; this->swap(t);
      return true;
    }

# endif//NO_METHOD_convert_pixels

  }//namespace graphics
}//namespace ds
