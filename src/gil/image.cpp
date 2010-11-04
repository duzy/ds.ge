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
#include <ds/graphics/gil/png_io.hpp>

namespace ds { namespace graphics { namespace gil {

      bool image::read_jpeg( std::istream & is )
      {
        return false;
      }

      bool image::read_png( std::istream & is )
      {
        try {
          gil::png_reader r( is );
          r.read( this->any() );
          return true;
        }
        catch( std::exception const & e ) {
          dsE("can't load PNG image: "<<e.what());
        }
        return false;
      }

      bool image::read_gif( std::istream & is )
      {
        return false;
      }

      bool image::read_tiff( std::istream & is )
      {
        return false;
      }

      bool image::write_jpeg( std::ostream & os )
      {
        return false;
      }

      bool image::write_png( std::ostream & os )
      {
        try {
          gil::png_writer w( os );
          w.write( this->any() );
          return true;
        }
        catch( std::exception const & e ) {
          dsE("can't load PNG image: "<<e.what());
        }
        return false;
      }

      bool image::write_gif( std::ostream & os )
      {
        return false;
      }

      bool image::write_tiff( std::ostream & os )
      {
        return false;
      }

      bool view::read_jpeg( std::istream & is )
      {
        return false;
      }

      bool view::read_png( std::istream & is )
      {
        try {
          gil::png_reader r( is );
          r.read( this->any() );
          return true;
        }
        catch( std::exception const & e ) {
          dsE("can't load PNG image: "<<e.what());
        }
        return false;
      }

      bool view::read_gif( std::istream & is )
      {
        return false;
      }

      bool view::read_tiff( std::istream & is )
      {
        return false;
      }

      bool view::write_jpeg( std::ostream & os )
      {
        return false;
      }

      bool view::write_png( std::ostream & os )
      {
        try {
          gil::png_writer w( os );
          w.write( this->any() );
          return true;
        }
        catch( std::exception const & e ) {
          dsE("can't load PNG image: "<<e.what());
        }
        return false;
      }

      bool view::write_gif( std::ostream & os )
      {
        return false;
      }

      bool view::write_tiff( std::ostream & os )
      {
        return false;
      }

    }//namespace gil
  }//namespace graphics
}//namespace ds
