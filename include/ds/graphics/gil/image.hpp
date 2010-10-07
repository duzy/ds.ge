/**
 *    Copyright 2009-03-22 DuzySoft.com, by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    All rights reserved by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id: image.hpp 614 2009-05-12 03:23:30Z duzy $
 *
 **/

#ifndef __DS_GRAPHICS_GIL_IMAGE_HPP____by_Duzy_Chan__
#define __DS_GRAPHICS_GIL_IMAGE_HPP____by_Duzy_Chan__ 1
#       include <boost/mpl/vector.hpp>
#       include <boost/gil/typedefs.hpp>
#       include <boost/gil/image_view_factory.hpp>
#       include <boost/gil/extension/dynamic_image/any_image.hpp>
#       include <boost/gil/extension/dynamic_image/apply_operation.hpp>

namespace ds { namespace graphics { namespace gil {

      typedef boost::gil::gray8_image_t         gray8_image_t;
      typedef boost::gil::gray16_image_t        gray16_image_t;
      typedef boost::gil::rgb8_image_t          rgb8_image_t;
      typedef boost::gil::rgb16_image_t         rgb16_image_t;
      typedef boost::gil::rgba8_image_t         rgba8_image_t;
      typedef boost::gil::rgba16_image_t        rgba16_image_t;
      typedef boost::gil::bgr8_image_t          bgr8_image_t;
      typedef boost::gil::bgr16_image_t         bgr16_image_t;
      typedef boost::gil::bgra8_image_t         bgra8_image_t;
      typedef boost::gil::bgra16_image_t        bgra16_image_t;
      typedef boost::gil::argb8_image_t         argb8_image_t;
      typedef boost::gil::argb16_image_t        argb16_image_t;
      typedef boost::gil::abgr8_image_t         abgr8_image_t;
      typedef boost::gil::abgr16_image_t        abgr16_image_t;
      typedef boost::gil::cmyk8_image_t         cmyk8_image_t;
      typedef boost::gil::cmyk16_image_t        cmyk16_image_t;

      // TODO: image rgb565, rgba444

      typedef boost::mpl::vector<
        gray8_image_t,        gray16_image_t,
        rgb8_image_t,         rgb16_image_t,
        rgba8_image_t,        rgba16_image_t,
        bgr8_image_t,         bgr16_image_t,
        bgra8_image_t,        bgra16_image_t,
        argb8_image_t,        argb16_image_t,
        abgr8_image_t,        abgr16_image_t,
        cmyk8_image_t,        cmyk16_image_t
        > supported_image_types;

      typedef boost::gil::any_image<supported_image_types> any_image_t;

      struct image : public any_image_t
      {
        image() : any_image_t() {}

        template<typename T>
        explicit image(const T & o) : any_image_t(o) {}

        const any_image_t & any() const { return *this; }
        any_image_t & any() { return *this; }

        bool load_jpeg( const std::string & file );
        bool load_png ( const std::string & file );
        bool load_tiff( const std::string & file );
      };//struct image

      struct view : public any_image_t::view_t
      {
        view()
          : any_image_t::view_t()
        {}

        explicit view( image & img )
          : any_image_t::view_t(boost::gil::view(img))
        {
        }

        template<typename PixelIter>
        view(std::size_t w, std::size_t h, PixelIter *p, std::ptrdiff_t rowBytes)
          : any_image_t::view_t(boost::gil::interleaved_view(w, h, p, rowBytes))
        {
        }

        //any_image_t::const_view_t & any() const { return this->const_view(); }
        const any_image_t::view_t & any() const { return *this; }
        any_image_t::view_t & any() { return *this; }
      };//struct view

    }//namespace gil
  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_GIL_IMAGE_HPP____by_Duzy_Chan__
