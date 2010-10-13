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
#include <ds/graphics/gil/png_io.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <ds/debug.hpp>
#include <typeinfo>
#include <fstream>

namespace ds { namespace graphics {

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
    }

    image::image( int w, int h, PixelType pt, uint8_t * data )
      : _isView( 1 )
      , _v( NULL )
    {
      bool ok = create(w, h, pt, data);
      dsI( ok );
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
      inline result_type operator()( const Any & a ) const { return image::NO_PIXEL; }

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
      case ARGB_4444_PIXEL: _m = new gil::image(gil::rgba4_image_t (w, h)); break;
      case ABGR_4444_PIXEL: _m = new gil::image(gil::bgra4_image_t (w, h)); break;
      case RGBA_4444_PIXEL: _m = new gil::image(gil::argb4_image_t (w, h)); break;
      case BGRA_4444_PIXEL: _m = new gil::image(gil::abgr4_image_t (w, h)); break;
      case ARGB_8888_PIXEL: _m = new gil::image(gil::rgba8_image_t (w, h)); break;
      case ABGR_8888_PIXEL: _m = new gil::image(gil::bgra8_image_t (w, h)); break;
      case RGBA_8888_PIXEL: _m = new gil::image(gil::argb8_image_t (w, h)); break;
      case BGRA_8888_PIXEL: _m = new gil::image(gil::abgr8_image_t (w, h)); break;
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
      if ( gil::png_reader::check(is) )
        { // TODO: move into gil::image::read_png
          if ( _isView || _m == NULL ) { //!< convert into image
            delete _v;
            _v = NULL;
            _isView = 0;
            _m = new gil::image;
          }

          gil::png_reader rdr( is, false /* No Check */ );
          rdr.read_image( _m->any() );
          return ( 0 < _m->width() && 0 < _m->height() );
        }
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
      gil::png_writer w( os );
      if ( _isView ) w.write_view( this->_v->any() );
      else           w.write_image( this->_m->any() );
      return true;
    }

  }//namespace graphics
}//namespace ds
