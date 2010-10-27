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
//#       include <boost/mpl/vector.hpp>
#       include <boost/mpl/vector/vector30.hpp>
#       include <boost/gil/packed_pixel.hpp>
#       include <boost/gil/typedefs.hpp>
#       include <boost/gil/image_view_factory.hpp>
#       include <boost/gil/extension/dynamic_image/any_image.hpp>
#       include <boost/gil/extension/dynamic_image/apply_operation.hpp>
#       include <iosfwd>

namespace ds { namespace graphics { namespace gil {

      using boost::gil::gray8_image_t;
      using boost::gil::gray16_image_t;
      using boost::gil::rgb8_image_t;
      using boost::gil::rgb16_image_t;
      using boost::gil::rgba8_image_t;
      using boost::gil::rgba16_image_t;
      using boost::gil::bgr8_image_t;
      using boost::gil::bgr16_image_t;
      using boost::gil::bgra8_image_t;
      using boost::gil::bgra16_image_t;
      using boost::gil::argb8_image_t;
      using boost::gil::argb16_image_t;
      using boost::gil::abgr8_image_t;
      using boost::gil::abgr16_image_t;
      using boost::gil::cmyk8_image_t;
      using boost::gil::cmyk16_image_t;

      typedef boost::gil::packed_pixel_type<uint16_t, boost::mpl::vector4_c<unsigned,4,4,4,4>, boost::gil::rgba_layout_t>::type rgba4_pixel_t; // rgba4444
      typedef boost::gil::packed_pixel_type<uint16_t, boost::mpl::vector4_c<unsigned,4,4,4,4>, boost::gil::bgra_layout_t>::type bgra4_pixel_t; // bgra4444
      typedef boost::gil::packed_pixel_type<uint16_t, boost::mpl::vector4_c<unsigned,4,4,4,4>, boost::gil::argb_layout_t>::type argb4_pixel_t; //
      typedef boost::gil::packed_pixel_type<uint16_t, boost::mpl::vector4_c<unsigned,4,4,4,4>, boost::gil::abgr_layout_t>::type abgr4_pixel_t; //

      typedef boost::gil::packed_pixel_type<uint16_t, boost::mpl::vector3_c<unsigned,5,6,5>, boost::gil::rgb_layout_t>::type rgb565_pixel_t;
      typedef boost::gil::packed_pixel_type<uint16_t, boost::mpl::vector3_c<unsigned,5,6,5>, boost::gil::bgr_layout_t>::type bgr565_pixel_t;

      BOOST_STATIC_ASSERT((sizeof(rgb565_pixel_t)==2));
      BOOST_STATIC_ASSERT((sizeof(bgr565_pixel_t)==2));
      BOOST_STATIC_ASSERT((sizeof(rgba4_pixel_t )==2));
      BOOST_STATIC_ASSERT((sizeof(bgra4_pixel_t )==2));
      BOOST_STATIC_ASSERT((sizeof(argb4_pixel_t )==2));
      BOOST_STATIC_ASSERT((sizeof(abgr4_pixel_t )==2));

      typedef boost::gil::image<rgba4_pixel_t,false,std::allocator<uint8_t> > rgba4_image_t;
      typedef boost::gil::image<bgra4_pixel_t,false,std::allocator<uint8_t> > bgra4_image_t;
      typedef boost::gil::image<argb4_pixel_t,false,std::allocator<uint8_t> > argb4_image_t;
      typedef boost::gil::image<abgr4_pixel_t,false,std::allocator<uint8_t> > abgr4_image_t;
      typedef boost::gil::image<rgb565_pixel_t,false,std::allocator<uint8_t> > rgb565_image_t;
      typedef boost::gil::image<bgr565_pixel_t,false,std::allocator<uint8_t> > bgr565_image_t;

#     if 1
      typedef boost::mpl::vector22<
        gray8_image_t,  gray16_image_t,
        
        rgb565_image_t, rgb8_image_t,  rgb16_image_t,
        bgr565_image_t, bgr8_image_t,  bgr16_image_t,
        
        rgba4_image_t,  rgba8_image_t, rgba16_image_t,
        bgra4_image_t,  bgra8_image_t, bgra16_image_t,
        argb4_image_t,  argb8_image_t, argb16_image_t,
        abgr4_image_t,  abgr8_image_t, abgr16_image_t,
        
        cmyk8_image_t,  cmyk16_image_t
        > supported_image_types;
#     else
      typedef boost::mpl::vector<
        gray8_image_t,  gray16_image_t,
        
        rgb8_image_t,  rgb16_image_t,
        bgr8_image_t,  bgr16_image_t,
        
        rgba8_image_t, rgba16_image_t,
        bgra8_image_t, bgra16_image_t,
        argb8_image_t, argb16_image_t,
        abgr8_image_t, abgr16_image_t,
        
        cmyk8_image_t,  cmyk16_image_t
        > supported_image_types;
#     endif

      typedef boost::gil::any_image<supported_image_types> any_image_t;

      struct image : public any_image_t
      {
        image() : any_image_t() {}

        template<typename T>
        explicit image(const T & o) : any_image_t(o) {}

        const any_image_t & any() const { return *this; }
        /**/  any_image_t & any()       { return *this; }

        bool read_jpeg( std::istream & is );
        bool read_png ( std::istream & is );
        bool read_gif ( std::istream & is );
        bool read_tiff( std::istream & is );

        bool write_jpeg( std::ostream & os );
        bool write_png ( std::ostream & os );
        bool write_gif ( std::ostream & os );
        bool write_tiff( std::ostream & os );
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
        /**/  any_image_t::view_t & any()       { return *this; }

        bool read_jpeg( std::istream & is );
        bool read_png ( std::istream & is );
        bool read_gif ( std::istream & is );
        bool read_tiff( std::istream & is );

        bool write_jpeg( std::ostream & os );
        bool write_png ( std::ostream & os );
        bool write_gif ( std::ostream & os );
        bool write_tiff( std::ostream & os );
      };//struct view

    }//namespace gil
  }//namespace graphics
}//namespace ds

namespace boost { namespace gil {
    template <typename ChannelValue, typename TS, typename Layout>
    struct channel_type<packed_pixel<ChannelValue,TS,Layout> > {
      typedef ChannelValue type; // FIXME: the channel value type
    };

    // template <typename ChannelValue, typename TS, typename Layout>
    // struct default_color_converter_impl<rgba_t,packed_pixel<ChannelValue,TS,Layout> >
    // {
    //   template <typename P1, typename P2>
    //   void operator()(const P1& src, P2& dst) const
    //   {
    //     // TODO: ...
    //   }
    // };

    // template <typename ColorBase, int K>
    // inline typename kth_semantic_element_const_reference_type<ColorBase,K>::type
    // channel_multiply(typename kth_semantic_element_const_reference_type<ColorBase,K>::type a,
    //                  typename kth_semantic_element_const_reference_type<ColorBase,K>::type b)
    // {
    //   //return channel_multiplier<typename channel_traits<Channel>::value_type>()(a,b);
    //   //TODO: ...
    // }

  }//namespace gil
}//namespace boost

#endif//__DS_GRAPHICS_GIL_IMAGE_HPP____by_Duzy_Chan__
