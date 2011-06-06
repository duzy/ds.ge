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
#include <ds/log.hpp>
#include <fstream>

namespace ds { namespace graphics {

#   include "image_metafuns.h"

    namespace
    {
      struct get_pixel_type_f
      {
        typedef image::PixelType result_type;
        template<typename Any> result_type operator()( const Any & a ) const
        {
          return image_pixel_type<Any>::value;
        }
      };//struct get_pixel_type_f

      struct get_image_pixels_f
      {
        typedef uint8_t * result_type;

        template<typename V>
        inline result_type operator()( const V & v ) const
        {
          return reinterpret_cast<result_type>(&(v[0]));
        }
      };//struct get_image_pixels_f

    }//namespace

    //-----------------------------------------------------------------

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

    image::PixelType image::pixel_type() const
    {
      if ( !is_valid() ) return NO_PIXEL;

      get_pixel_type_f f;
      return _isView
        ? boost::gil::apply_operation( _v->any(), f )
        : boost::gil::apply_operation( _m->any_view(), f )
        ;
    }

    std::size_t image::pixel_size() const
    {
      //case PIXEL_TYPE: return sizeof(gil_image<image::PIXEL_TYPE>::type::value_type)
#   define HANDLE_PIXEL_TYPE(PIXEL_TYPE,GIL_IMAGE_TYPE)             \
      case PIXEL_TYPE: return sizeof(GIL_IMAGE_TYPE::value_type)
      switch ( this->pixel_type() ) { HANDLE_ALL_PIXEL_TYPES() }
#   undef  HANDLE_PIXEL_TYPE
      return 0;
    }

    bool image::create( int w, int h, PixelType pt )
    {
      if ( _isView ) delete _v;
      else           delete _m;

      _isView = 0;
      _m = NULL;

      //case PIXEL_TYPE: _m = new gil::image(gil_image<image::PIXEL_TYPE>::type(w,h)); break
#   define HANDLE_PIXEL_TYPE(PIXEL_TYPE,GIL_IMAGE_TYPE)                 \
      case PIXEL_TYPE: _m = new gil::image(GIL_IMAGE_TYPE(w,h)); break
      switch ( pt ) { HANDLE_ALL_PIXEL_TYPES() }
#   undef  HANDLE_PIXEL_TYPE

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

#   define HANDLE_PIXEL_TYPE(PIXEL_TYPE,GIL_IMAGE_TYPE)                 \
      case PIXEL_TYPE: _v = new gil::view(w, h, (GIL_IMAGE_TYPE::value_type*)data, w*sizeof(GIL_IMAGE_TYPE::value_type)); break
      switch ( pt ) { HANDLE_ALL_PIXEL_TYPES() }
#   undef  HANDLE_PIXEL_TYPE

      return ( _v != NULL );
    }

    uint8_t * image::pixels()
    {
      if ( !is_valid() ) return NULL;

      get_image_pixels_f f;
      return _isView
        ? boost::gil::apply_operation( _v->any(), f )
        : boost::gil::apply_operation( _m->any_view(), f )
        ;
    }

    void image::swap( image & o )
    {
      bool isView = o._isView;
      gil::image * m  = o._m;
      o._isView = this->_isView;
      o._m = this->_m;
      this->_isView = isView ? 1 : 0;
      this->_m = m;
      //gil::view  * v  = o._v;
      //o._v = this->_v;
      //this->_v = v;
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
