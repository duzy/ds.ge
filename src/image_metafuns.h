/**                                                                -*- c++ -*-
 *    Copyright DuzySoft.com, by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    All rights reserved by Zhan Xin-Ming£¨Õ²ÐÀÃú£©
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 **/

#ifndef __image_metafuns_h____by_Duzy_Chan__
#define __image_metafuns_h____by_Duzy_Chan__ 1
#   define HANDLE_ALL_PIXEL_TYPES()             \
  HANDLE_PIXEL_TYPE(ARGB_8888_PIXEL, gil::argb8_image_t );  \
  HANDLE_PIXEL_TYPE(ABGR_8888_PIXEL, gil::abgr8_image_t );  \
  HANDLE_PIXEL_TYPE(RGBA_8888_PIXEL, gil::rgba8_image_t );  \
  HANDLE_PIXEL_TYPE(BGRA_8888_PIXEL, gil::bgra8_image_t );  \
  HANDLE_PIXEL_TYPE(ARGB_4444_PIXEL, gil::argb4_image_t );  \
  HANDLE_PIXEL_TYPE(ABGR_4444_PIXEL, gil::abgr4_image_t );  \
  HANDLE_PIXEL_TYPE(RGBA_4444_PIXEL, gil::rgba4_image_t );  \
  HANDLE_PIXEL_TYPE(BGRA_4444_PIXEL, gil::bgra4_image_t );  \
  HANDLE_PIXEL_TYPE(RGB_888_PIXEL  , gil::rgb8_image_t  );  \
  HANDLE_PIXEL_TYPE(BGR_888_PIXEL  , gil::bgr8_image_t  );  \
  HANDLE_PIXEL_TYPE(RGB_565_PIXEL  , gil::rgb565_image_t);  \
  HANDLE_PIXEL_TYPE(BGR_565_PIXEL  , gil::bgr565_image_t);

namespace
{
  template<image::PixelType PT> struct gil_image;
# define HANDLE_PIXEL_TYPE(PIXEL_TYPE,GIL_IMAGE_TYPE)                   \
  template<> struct gil_image<image::PIXEL_TYPE> { typedef GIL_IMAGE_TYPE type; }
  HANDLE_ALL_PIXEL_TYPES()
# undef  HANDLE_PIXEL_TYPE

  template<typename IM> struct image_pixel_type { static const image::PixelType value = image::NO_PIXEL; };
# define HANDLE_PIXEL_TYPE(PIXEL_TYPE,GIL_IMAGE_TYPE)                   \
  template<> struct image_pixel_type<GIL_IMAGE_TYPE> { static const image::PixelType value = image::PIXEL_TYPE; }; \
  template<> struct image_pixel_type<GIL_IMAGE_TYPE::view_t> { static const image::PixelType value = image::PIXEL_TYPE; }
  HANDLE_ALL_PIXEL_TYPES()
# undef  HANDLE_PIXEL_TYPE

}//namespace

#endif//__image_metafuns_h____by_Duzy_Chan__
