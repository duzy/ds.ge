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
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/gil/extension/dynamic_image/algorithm.hpp>
#include <ds/debug.hpp>
#include <fstream>
#ifdef PRINT_DEMANGLE
#  include <typeinfo>
#  include <cxxabi.h>
#endif//PRINT_DEMANGLE

namespace ds { namespace graphics {

    namespace
    {
      struct pixel_converter
      {
        typedef void result_type;

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
#       ifdef PRINT_DEMANGLE
          int status;
          std::clog
            <<"convert: (pixel, pixel)"<<std::endl
            <<"  "<<abi::__cxa_demangle(typeid(v1).name(), 0, 0, &status)<<std::endl
            <<"  "<<abi::__cxa_demangle(typeid(v2).name(), 0, 0, &status)<<std::endl
            ;
#       endif//PRINT_DEMANGLE
          boost::gil::copy_and_convert_pixels( v1, v2 );
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
#       ifdef PRINT_DEMANGLE
          int status;
          std::clog
            <<"convert: (packed, packed)"<<std::endl
            <<"  "<<abi::__cxa_demangle(typeid(v1).name(), 0, 0, &status)<<std::endl
            <<"  "<<abi::__cxa_demangle(typeid(v2).name(), 0, 0, &status)<<std::endl
            ;
#       endif//PRINT_DEMANGLE
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
#       ifdef PRINT_DEMANGLE
          int status;
          std::clog
            <<"convert: (packed, pixel)"<<std::endl
            <<"  "<<abi::__cxa_demangle(typeid(v1).name(), 0, 0, &status)<<std::endl
            <<"  "<<abi::__cxa_demangle(typeid(v2).name(), 0, 0, &status)<<std::endl
            ;
#       endif//PRINT_DEMANGLE
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
#       ifdef PRINT_DEMANGLE
          int status;
          std::clog
            <<"convert: (pixel, packed)"<<std::endl
            <<"  "<<abi::__cxa_demangle(typeid(v1).name(), 0, 0, &status)<<std::endl
            <<"  "<<abi::__cxa_demangle(typeid(v2).name(), 0, 0, &status)<<std::endl
            ;
#       endif//PRINT_DEMANGLE
        }
      };//struct pixel_converter
    }//namespace

    image::image()
      : _isView( 0 )
      , _m( NULL )
    {
    }

    image::image( int w, int h, PixelType pt )
      : _isView( 0 )
      , _m( NULL )
    {
      bool ok = create(w, h, pt);
      dsI( ok );
      dsI( pixel_type() == pt );
    }

    image::image( int w, int h, PixelType pt, uint8_t * data )
      : _isView( 1 )
      , _v( NULL )
    {
      bool ok = create(w, h, pt, data);
      dsI( ok );
      dsI( pixel_type() == pt );
    }

    image::~image()
    {
      if ( _isView ) delete _v;
      else           delete _m;
    }

    struct get_pixel_type_f
    {
      typedef image::PixelType result_type;

      template<typename Any>
      inline result_type operator()( const Any & a )                      const { return image::NO_PIXEL; }

      inline result_type operator()( const gil::argb4_image_t::view_t & ) const { return image::ARGB_4444_PIXEL; }
      inline result_type operator()( const gil::argb4_image_t &         ) const { return image::ARGB_4444_PIXEL; }

      inline result_type operator()( const gil::abgr4_image_t::view_t & ) const { return image::ABGR_4444_PIXEL; }
      inline result_type operator()( const gil::abgr4_image_t &         ) const { return image::ABGR_4444_PIXEL; }

      inline result_type operator()( const gil::bgra4_image_t::view_t & ) const { return image::BGRA_4444_PIXEL; }
      inline result_type operator()( const gil::bgra4_image_t &         ) const { return image::BGRA_4444_PIXEL; }

      inline result_type operator()( const gil::rgba4_image_t::view_t & ) const { return image::RGBA_4444_PIXEL; }
      inline result_type operator()( const gil::rgba4_image_t &         ) const { return image::RGBA_4444_PIXEL; }

      inline result_type operator()( const gil::argb8_image_t::view_t & ) const { return image::ARGB_8888_PIXEL; }
      inline result_type operator()( const gil::argb8_image_t &         ) const { return image::ARGB_8888_PIXEL; }

      inline result_type operator()( const gil::abgr8_image_t::view_t & ) const { return image::ABGR_8888_PIXEL; }
      inline result_type operator()( const gil::abgr8_image_t &         ) const { return image::ABGR_8888_PIXEL; }

      inline result_type operator()( const gil::bgra8_image_t::view_t & ) const { return image::BGRA_8888_PIXEL; }
      inline result_type operator()( const gil::bgra8_image_t &         ) const { return image::BGRA_8888_PIXEL; }

      inline result_type operator()( const gil::rgba8_image_t::view_t & ) const { return image::RGBA_8888_PIXEL; }
      inline result_type operator()( const gil::rgba8_image_t &         ) const { return image::RGBA_8888_PIXEL; }

      inline result_type operator()( const gil::bgr8_image_t::view_t &  ) const { return image::BGR_888_PIXEL; }
      inline result_type operator()( const gil::bgr8_image_t &          ) const { return image::BGR_888_PIXEL; }

      inline result_type operator()( const gil::rgb8_image_t::view_t &  ) const { return image::RGB_888_PIXEL; }
      inline result_type operator()( const gil::rgb8_image_t &          ) const { return image::RGB_888_PIXEL; }

      inline result_type operator()( const gil::bgr565_image_t::view_t &  ) const { return image::BGR_565_PIXEL; }
      inline result_type operator()( const gil::bgr565_image_t &          ) const { return image::BGR_565_PIXEL; }

      inline result_type operator()( const gil::rgb565_image_t::view_t &  ) const { return image::RGB_565_PIXEL; }
      inline result_type operator()( const gil::rgb565_image_t &          ) const { return image::RGB_565_PIXEL; }
    };//struct get_pixel_type_f

    image::PixelType image::pixel_type() const
    {
      if ( !is_valid() ) return NO_PIXEL;

      get_pixel_type_f f;
      return _isView
        ? boost::gil::apply_operation( _v->any(), f )
        : boost::gil::apply_operation( _m->any(), f )
        ;
    }

    std::size_t image::pixel_size() const
    {
      switch ( this->pixel_type() ) {
      case ARGB_8888_PIXEL: return sizeof(gil::argb8_image_t::value_type);
      case ABGR_8888_PIXEL: return sizeof(gil::abgr8_image_t::value_type);
      case RGBA_8888_PIXEL: return sizeof(gil::rgba8_image_t::value_type);
      case BGRA_8888_PIXEL: return sizeof(gil::bgra8_image_t::value_type);
      case ARGB_4444_PIXEL: return sizeof(gil::argb4_image_t::value_type);
      case ABGR_4444_PIXEL: return sizeof(gil::abgr4_image_t::value_type);
      case RGBA_4444_PIXEL: return sizeof(gil::rgba4_image_t::value_type);
      case BGRA_4444_PIXEL: return sizeof(gil::bgra4_image_t::value_type);
      case RGB_888_PIXEL:   return sizeof(gil::rgb8_image_t::value_type );
      case BGR_888_PIXEL:   return sizeof(gil::bgr8_image_t::value_type );
      case RGB_565_PIXEL:   return sizeof(gil::rgb565_image_t::value_type);
      case BGR_565_PIXEL:   return sizeof(gil::bgr565_image_t::value_type);
      }
      return 0;
    }

    bool image::create( int w, int h, PixelType pt )
    {
      if ( _isView ) delete _v;
      else           delete _m;

      _isView = 0;
      _m = NULL;

      switch ( pt ) {
      case RGB_565_PIXEL:   _m = new gil::image(gil::rgb565_image_t(w, h)); break;
      case BGR_565_PIXEL:   _m = new gil::image(gil::bgr565_image_t(w, h)); break;
      case RGB_888_PIXEL:   _m = new gil::image(gil::rgb8_image_t  (w, h)); break;
      case BGR_888_PIXEL:   _m = new gil::image(gil::bgr8_image_t  (w, h)); break;
      case ARGB_4444_PIXEL: _m = new gil::image(gil::argb4_image_t (w, h)); break;
      case ABGR_4444_PIXEL: _m = new gil::image(gil::abgr4_image_t (w, h)); break;
      case RGBA_4444_PIXEL: _m = new gil::image(gil::rgba4_image_t (w, h)); break;
      case BGRA_4444_PIXEL: _m = new gil::image(gil::bgra4_image_t (w, h)); break;
      case ARGB_8888_PIXEL: _m = new gil::image(gil::argb8_image_t (w, h)); break;
      case ABGR_8888_PIXEL: _m = new gil::image(gil::abgr8_image_t (w, h)); break;
      case RGBA_8888_PIXEL: _m = new gil::image(gil::rgba8_image_t (w, h)); break;
      case BGRA_8888_PIXEL: _m = new gil::image(gil::bgra8_image_t (w, h)); break;
      }

      if ( _m && _m->width() != w && _m->height() != h ) {
        _m->recreate( w, h );
      }

      return ( _m != NULL );
    }

    bool image::create( int w, int h, PixelType pt, uint8_t * data )
    {
      if ( _isView ) delete _v;
      else           delete _m;

      _isView = 1;
      _v = NULL;

#     define NEW_GIL_VIEW(CS) new gil::view( w, h, (gil::CS##_image_t::value_type*)data, w * sizeof(gil::CS##_image_t::value_type*) )
      switch ( pt ) {
      case RGB_565_PIXEL:   _v = NEW_GIL_VIEW(rgb565); break;
      case BGR_565_PIXEL:   _v = NEW_GIL_VIEW(bgr565); break;
      case RGB_888_PIXEL:   _v = NEW_GIL_VIEW(rgb8  ); break;
      case BGR_888_PIXEL:   _v = NEW_GIL_VIEW(bgr8  ); break;
      case ARGB_4444_PIXEL: _v = NEW_GIL_VIEW(argb4); break;
      case ABGR_4444_PIXEL: _v = NEW_GIL_VIEW(abgr4); break;
      case RGBA_4444_PIXEL: _v = NEW_GIL_VIEW(rgba4); break;
      case BGRA_4444_PIXEL: _v = NEW_GIL_VIEW(bgra4); break;
      case ARGB_8888_PIXEL: _v = NEW_GIL_VIEW(argb8); break;
      case ABGR_8888_PIXEL: _v = NEW_GIL_VIEW(abgr8); break;
      case RGBA_8888_PIXEL: _v = NEW_GIL_VIEW(rgba8); break;
      case BGRA_8888_PIXEL: _v = NEW_GIL_VIEW(bgra8); break;
      }
#     undef NEW_GIL_VIEW

      return ( _v != NULL );
    }

    struct get_image_pixels_f
    {
      typedef uint8_t * result_type;

      template<typename V>
      inline result_type operator()( const V & v ) const
      {
        return reinterpret_cast<result_type>(&(v[0]));
      }
    };//struct get_image_pixels_f

    uint8_t * image::pixels()
    {
      if ( !is_valid() ) return NULL;

      get_image_pixels_f f;
      return _isView
        ? boost::gil::apply_operation( _v->any(), f )
        : boost::gil::apply_operation( boost::gil::view(_m->any()), f )
        ;
    }

    void image::swap( image & o )
    {
      unsigned isView = o._isView;
      gil::image * m  = o._m;
      o._isView = this->_isView;
      o._m = this->_m;
      this->_isView = isView;
      this->_m = m;
      //gil::view  * v  = o._v;
      //o._v = this->_v;
      //this->_v = v;
    }

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
#     else
      pixel_converter pc;
      boost::gil::apply_operation( v1, v2, pc );
      //dsI( pc.done );
#     endif
      return true;
    }

    std::size_t image::width() const
    {
      if ( !is_valid() ) return 0;
      return _isView ? _v->width() : _m->width();
    }

    std::size_t image::height() const
    {
      if ( !is_valid() ) return 0;
      return _isView ? _v->height() : _m->height();
    }

    bool image::load( const std::string & file )
    {
      //TODO: using boost::filesystem::path?
      if ( file.empty() ) {
        dsE("empty image file name");
        return false;
      }

      if ( file[0] == ':' ) {
        TODO("load image from ds::resource");
        return false;
      }

      if ( 5 < file.size() ) {

        if ( _isView || _m == NULL ) { //!< convert into image
          delete _v;
          _v = NULL;
          _isView = 0;
          _m = new gil::image;
        }

        using boost::to_lower;
        using boost::iends_with;
        std::string suffix( file.substr(file.size()-5, std::string::npos) );

        to_lower( suffix );

        std::ifstream is( file.c_str(), is.in | is.binary );
        if ( iends_with(suffix, ".png" ) ) return _m->read_png ( is );
        if ( iends_with(suffix, ".jpg" ) ) return _m->read_jpeg( is );
        if ( iends_with(suffix, ".jpeg") ) return _m->read_jpeg( is );
        if ( iends_with(suffix, ".tiff") ) return _m->read_tiff( is );
        if ( iends_with(suffix, ".skin") ) return _m->read_png ( is );
      }

      return false;
    }

    bool image::load( std::istream & is )
    {
      if ( _isView || _m == NULL ) { //!< convert into image
        delete _v;
        _v = NULL;
        _isView = 0;
        _m = new gil::image;
      }
      // if ( gil::png_reader::check(is) ) return _m->read_png( is );
      return _m->read_png( is );

      // TODO: jpeg, gif, ...
      return false;
    }

    bool image::save( const std::string & file )
    {
      std::ofstream o( file.c_str(), o.out | o.binary );
      return this->save( o );
    }

    bool image::save( std::ostream & os )
    {
      if ( _isView ) return this->_v->write_png( os );
      else           return this->_m->write_png( os );
    }

  }//namespace graphics
}//namespace ds
