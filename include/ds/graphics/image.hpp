/**
 *    Copyright 2009-03-22 DuzySoft.com, by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    All rights reserved by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id: image.hpp 801 2009-09-15 14:02:44Z duzy $
 *
 **/

#ifndef __DS_GRAPHICS_IMAGE_HPP____by_Duzy_Chan__
#define __DS_GRAPHICS_IMAGE_HPP____by_Duzy_Chan__ 1
#   include <boost/cstdint.hpp>
#   include "visibility.hpp"
#   include <string>
#   include <iosfwd>

namespace ds { namespace graphics {

    namespace gil
    {
      struct image; // Forward declaration
      struct view ; // Forward declaration
    }//namespace gil

    struct DS_GRAPHICS_PUBLIC image
    {
      image();

      // TODO: explicit image( const std::string & fileName );

      enum PixelType {
        NO_PIXEL,
        ARGB_8888_PIXEL,
        ARGB_4444_PIXEL,
        RGB_565_PIXEL,
      };

      image( int w, int h, PixelType pt );
      image( int w, int h, PixelType pt, uint8_t * data );

      virtual ~image();

      inline bool is_valid() const { return (_m != NULL) /*&& (_v != NULL)*/; }

      bool create( int w, int h, PixelType pt );
      bool create( int w, int h, PixelType pt, uint8_t * data );

      const uint8_t * pixels() const { return const_cast<image*>(this)->pixels(); }
      uint8_t * pixels();

      PixelType pixel_type() const;

      std::size_t pixel_size() const;

      std::size_t width() const;
      std::size_t height() const;

      std::size_t stride() const { return pixel_size() * width(); }

      #if 0
      enum resize_e { resize_smooth }
      void resize();
      #endif

      /**
       *  @brief Load PNG image from a disk file or ds::resource.
       *
       *  @code
       *  image img1, img2;
       *  img1.load( "/images/foo.png" ); //!< load a disk file
       *  img2.load( ":/images/foo.png" ); //!< load from ds::resource
       *  @endcode
       */
      bool load( const std::string & file );
      bool save( const std::string & file );

      bool load( std::istream & is );
      bool save( std::ostream & is );

    private:
      unsigned _isView : 1;
      union {
        gil::image      * _m;
        gil::view       * _v;
      };//union
    };//struct image

  }//namespace graphics
}//namespace ds


#endif//__DS_GRAPHICS_IMAGE_HPP____by_Duzy_Chan__
