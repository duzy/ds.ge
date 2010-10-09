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
#include <ds/graphics/gil/png_reader.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <ds/debug.hpp>
#include <typeinfo>

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
      else delete _m;
    }

    struct get_pixel_type_f
    {
      typedef image::PixelType result_type;

      template<typename Any>
      inline result_type operator()( const Any & a ) const { dsL("type: "<<sizeof(typename Any::value_type)<<", "<<typeid(a).name()); return image::NO_PIXEL; }

      // TODO: match image formats

      inline result_type operator()( const gil::argb8_image_t::view_t & ) const { return image::ARGB_8888_PIXEL; }
      inline result_type operator()( const gil::argb8_image_t & ) const { return image::ARGB_8888_PIXEL; }

      inline result_type operator()( const gil::abgr8_image_t::view_t & ) const { return image::ARGB_8888_PIXEL; }
      inline result_type operator()( const gil::abgr8_image_t & ) const { return image::ARGB_8888_PIXEL; }

      inline result_type operator()( const gil::rgba8_image_t::view_t & ) const { return image::ARGB_8888_PIXEL; }
      inline result_type operator()( const gil::rgba8_image_t & ) const { return image::ARGB_8888_PIXEL; }

      inline result_type operator()( const gil::bgra8_image_t::view_t & ) const { return image::ARGB_8888_PIXEL; }
      inline result_type operator()( const gil::bgra8_image_t & ) const { return image::ARGB_8888_PIXEL; }

      inline result_type operator()( const gil::rgb8_image_t::view_t & ) const { return image::RGB_888_PIXEL; }
      inline result_type operator()( const gil::rgb8_image_t & ) const { return image::RGB_888_PIXEL; }

      inline result_type operator()( const gil::bgr8_image_t::view_t & ) const { return image::RGB_888_PIXEL; }
      inline result_type operator()( const gil::bgr8_image_t & ) const { return image::RGB_888_PIXEL; }
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
      case ARGB_8888_PIXEL: return sizeof(gil::rgba8_image_t::value_type);
      case ARGB_4444_PIXEL: return 0;
      case RGB_888_PIXEL:   return sizeof(gil::rgb8_image_t::value_type);
      case RGB_565_PIXEL:   return 0;
      }
      return 0;
    }

    bool image::create( int w, int h, PixelType pt )
    {
      if ( _isView ) delete _v;
      else delete _m;

      _isView = 0;
      _m = NULL;

      switch ( pt ) {
      case RGB_565_PIXEL:
      case ARGB_4444_PIXEL:
        break;
      case ARGB_8888_PIXEL:
        _m = new gil::image(gil::rgba8_image_t(w, h));
        break;
      }

      if ( _m ) {
        _m->recreate( w, h );
      }

      return ( _m != NULL );
    }

    bool image::create( int w, int h, PixelType pt, uint8_t * data )
    {
      if ( _isView ) delete _v;
      else delete _m;

      _isView = 1;
      _v = NULL;

      switch ( pt ) {
      case RGB_565_PIXEL:
      case ARGB_4444_PIXEL:
        break;
      case ARGB_8888_PIXEL:
        {
          typedef gil::rgba8_image_t::value_type pixel_t;
          _v = new gil::view( w, h, (pixel_t*)data, w * sizeof(pixel_t) );
        } break;
      }

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

        if ( iends_with(suffix, ".png" ) ) return _m->load_png ( file );
        if ( iends_with(suffix, ".jpg" ) ) return _m->load_jpeg( file );
        if ( iends_with(suffix, ".jpeg") ) return _m->load_jpeg( file );
        if ( iends_with(suffix, ".tiff") ) return _m->load_tiff( file );
        if ( iends_with(suffix, ".skin") ) return _m->load_png ( file );
      }

      return false;
    }

    bool image::load( std::istream & is )
    {
      if ( gil::png_reader::check(is) )
        {
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
      // TODO: ...
      return false;
    }

    bool image::save( std::ostream & is )
    {
      // TODO: ...
      return false;
    }

  }//namespace graphics
}//namespace ds
