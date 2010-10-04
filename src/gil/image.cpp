/**								-*- c++ -*-
 *    Copyright 2009-03-22 DuzySoft.com, by Duzy Chan£¨Õ²ÐÀÃú£©
 *    All rights reserved by Duzy Chan£¨Õ²ÐÀÃú£©
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id: graphics_image.cpp 614 2009-05-12 03:23:30Z duzy $
 *
 **/

#include <ds/debug.hpp>
#include <ds/graphics/gil/image.hpp>
#include <boost/gil/image.hpp>
extern "C" {
#   include <png.h>
}
#   if PNG_LIBPNG_VER_MAJOR==1 && PNG_LIBPNG_VER_MINOR==4 && PNG_LIBPNG_VER_RELEASE>=3
#      define png_infopp_NULL NULL /* for png_dynamic_io.hpp, libpng-1.4.x don't have */
#      define int_p_NULL NULL /* for png_dynamic_io.hpp, libpng-1.4.x don't have */
#   endif
#include <boost/gil/extension/io/png_dynamic_io.hpp>
//#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
//#include <boost/gil/extension/io/tiff_dynamic_io.hpp>

namespace ds { namespace graphics { namespace gil {

      bool image::load_png( const std::string & file )
      {
        try {
          png_read_image( file, this->any() );
          return true;
        }
        catch( std::exception const & e ) {
          dsE("can't load PNG image: "<<file<<"("<<e.what()<<")");
        }
        catch( ... ) {
          dsE("can't load PNG image: "<<file);
        }
        return false;
      }

      bool image::load_jpeg( const std::string & file )
      {
        TODO("load JPEG iamge");
        return false;
      }

      bool image::load_tiff( const std::string & file )
      {
        TODO("load TIFF image");
        return false;
      }

    }//namespace gil
  }//namespace graphics
}//namespace ds
