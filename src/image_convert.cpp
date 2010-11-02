/**                                                                -*- c++ -*-
 *    Copyright 2009-03-22 DuzySoft.com, by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    All rights reserved by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id: image.ipp 801 2009-09-15 14:02:44Z duzy $
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

    namespace
    {
      struct pixel_converter
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
      };//struct pixel_converter

      /////////////////////////////////////////////

      template<image::PixelType PT> struct gil_image;
      template<> struct gil_image<image::ARGB_8888_PIXEL> { typedef gil::argb8_image_t  type; };
      template<> struct gil_image<image::ABGR_8888_PIXEL> { typedef gil::abgr8_image_t  type; };
      template<> struct gil_image<image::RGBA_8888_PIXEL> { typedef gil::rgba8_image_t  type; };
      template<> struct gil_image<image::BGRA_8888_PIXEL> { typedef gil::bgra8_image_t  type; };
      template<> struct gil_image<image::ARGB_4444_PIXEL> { typedef gil::argb4_image_t  type; };
      template<> struct gil_image<image::ABGR_4444_PIXEL> { typedef gil::abgr4_image_t  type; };
      template<> struct gil_image<image::RGBA_4444_PIXEL> { typedef gil::rgba4_image_t  type; };
      template<> struct gil_image<image::BGRA_4444_PIXEL> { typedef gil::bgra4_image_t  type; };
      template<> struct gil_image<image::RGB_888_PIXEL  > { typedef gil::rgb8_image_t   type; };
      template<> struct gil_image<image::BGR_888_PIXEL  > { typedef gil::bgr8_image_t   type; };
      template<> struct gil_image<image::RGB_565_PIXEL  > { typedef gil::rgb565_image_t type; };
      template<> struct gil_image<image::BGR_565_PIXEL  > { typedef gil::bgr565_image_t type; };

      template<image::PixelType PT>
      struct converter
      {
        typedef bool result_type;
        typedef typename gil_image<PT>::type gil_image_t;
        
        int width, height;
        gil_image_t output;

        converter(int w, int h) : width(w), height(h), output() {}

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
          return output.width() == width && output.height() == height;
        }

        template<
          template<typename> class Iterator,
          template<typename> class Locator,
          template<typename,typename,typename> class PackedPixel,
          typename VT, typename PF, typename Layout >
        result_type operator()( const boost::gil::image_view<Locator<Iterator<PackedPixel<VT,PF,Layout>*> > > & v ) /*const*/
        {
          return false;
        }
      };//struct converter<>

      template<> struct converter<image::NO_PIXEL>
      {
        typedef bool result_type;
        template<typename V> result_type operator()(const V &) { return false; }
      };//struct converter<image::NO_PIXEL>

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
      return boost::gil::apply_operation( v1, v2, pixel_converter() );
#     endif
    }

    /////////////////////////////////////////////////////////////////

#define IMPLENMENT_CONVERT(PIXEL_TYPE)                              \
    template<> bool image::convert<image::PIXEL_TYPE>()             \
    {                                                               \
      if ( !_m /* || _v */ ) return false;                          \
      if (pixel_type() == PIXEL_TYPE) return false;                 \
                                                                    \
      image t; this->swap(t);                                       \
      dsI( t._m != 0 || t._v != 0 );                                \
      dsI( this->_m == 0 && this->_v == 0 );                        \
                                                                    \
      gil::any_image_t::view_t vin = t._isView                      \
        ? t._v->any() : boost::gil::view( t._m->any() );            \
                                                                    \
      converter<image::PIXEL_TYPE> conv( t.width(), t.height() );   \
      if (boost::gil::apply_operation( vin, conv )) {               \
        dsI(conv.output.width() == t.width());                      \
        dsI(conv.output.height() == t.height());                    \
        dsI(this->_m == NULL);                                      \
        this->_isView = 0;                                          \
        this->_m = new gil::image(conv.output);                     \
        return true;                                                \
      }                                                             \
      return false;                                                 \
    }
    /*
    template<> bool image::convert<image::NO_PIXEL>()
    {
      if (pixel_type() == NO_PIXEL) return false;
      image t; this->swap(t);
      return true;
    }
    IMPLENMENT_CONVERT(ARGB_8888_PIXEL);
    IMPLENMENT_CONVERT(ABGR_8888_PIXEL);
    IMPLENMENT_CONVERT(RGBA_8888_PIXEL);
    IMPLENMENT_CONVERT(BGRA_8888_PIXEL);
    IMPLENMENT_CONVERT(ARGB_4444_PIXEL);
    IMPLENMENT_CONVERT(ABGR_4444_PIXEL);
    IMPLENMENT_CONVERT(RGBA_4444_PIXEL);
    IMPLENMENT_CONVERT(BGRA_4444_PIXEL);
    IMPLENMENT_CONVERT(RGB_888_PIXEL  );
    IMPLENMENT_CONVERT(BGR_888_PIXEL  );
    IMPLENMENT_CONVERT(RGB_565_PIXEL  );
    IMPLENMENT_CONVERT(BGR_565_PIXEL  );
    */
#undef IMPLENMENT_CONVERT

  }//namespace graphics
}//namespace ds
