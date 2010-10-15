/**      							-*- c++ -*-
 *
 */

#include <ds/graphics/image.hpp>
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <iostream>

BOOST_AUTO_TEST_CASE( image_pixel_format )
{
  /*
        ARGB_8888_PIXEL,
        ABGR_8888_PIXEL,
        RGBA_8888_PIXEL,
        BGRA_8888_PIXEL,
        ARGB_4444_PIXEL,
        ABGR_4444_PIXEL,
        RGBA_4444_PIXEL,
        BGRA_4444_PIXEL,
        RGB_888_PIXEL,
        BGR_888_PIXEL,
        RGB_565_PIXEL,
        BGR_565_PIXEL,
  */
  {
    ds::graphics::image image( 5, 5, ds::graphics::image::ARGB_8888_PIXEL );
    BOOST_CHECK( image.width() == 5 );
    BOOST_CHECK( image.height() == 5 );
    BOOST_CHECK( 4 == image.pixel_size() );
    BOOST_CHECK( image.ARGB_8888_PIXEL == image.pixel_type() );
  }
  {
    ds::graphics::image image( 5, 5, ds::graphics::image::ABGR_8888_PIXEL );
    BOOST_CHECK( image.width() == 5 );
    BOOST_CHECK( image.height() == 5 );
    BOOST_CHECK( 4 == image.pixel_size() );
    BOOST_CHECK( image.ABGR_8888_PIXEL == image.pixel_type() );
  }
  {
    ds::graphics::image image( 5, 5, ds::graphics::image::RGBA_8888_PIXEL );
    BOOST_CHECK( image.width() == 5 );
    BOOST_CHECK( image.height() == 5 );
    BOOST_CHECK( 4 == image.pixel_size() );
    BOOST_CHECK( image.RGBA_8888_PIXEL == image.pixel_type() );
  }
  {
    ds::graphics::image image( 5, 5, ds::graphics::image::BGRA_8888_PIXEL );
    BOOST_CHECK( image.width() == 5 );
    BOOST_CHECK( image.height() == 5 );
    BOOST_CHECK( 4 == image.pixel_size() );
    BOOST_CHECK( image.BGRA_8888_PIXEL == image.pixel_type() );
  }

  {
    ds::graphics::image image( 5, 5, ds::graphics::image::ARGB_4444_PIXEL );
    BOOST_CHECK( image.width() == 5 );
    BOOST_CHECK( image.height() == 5 );
    BOOST_CHECK( 2 == image.pixel_size() );
    BOOST_CHECK( image.ARGB_4444_PIXEL == image.pixel_type() );
  }
  {
    ds::graphics::image image( 5, 5, ds::graphics::image::ABGR_4444_PIXEL );
    BOOST_CHECK( image.width() == 5 );
    BOOST_CHECK( image.height() == 5 );
    BOOST_CHECK( 2 == image.pixel_size() );
    BOOST_CHECK( image.ABGR_4444_PIXEL == image.pixel_type() );
  }
  {
    ds::graphics::image image( 5, 5, ds::graphics::image::RGBA_4444_PIXEL );
    BOOST_CHECK( image.width() == 5 );
    BOOST_CHECK( image.height() == 5 );
    BOOST_CHECK( 2 == image.pixel_size() );
    BOOST_CHECK( image.RGBA_4444_PIXEL == image.pixel_type() );
  }
  {
    ds::graphics::image image( 5, 5, ds::graphics::image::BGRA_4444_PIXEL );
    BOOST_CHECK( image.width() == 5 );
    BOOST_CHECK( image.height() == 5 );
    BOOST_CHECK( 2 == image.pixel_size() );
    BOOST_CHECK( image.BGRA_4444_PIXEL == image.pixel_type() );
  }

  {
    ds::graphics::image image( 5, 5, ds::graphics::image::RGB_888_PIXEL );
    BOOST_CHECK( image.width() == 5 );
    BOOST_CHECK( image.height() == 5 );
    BOOST_CHECK( 3 == image.pixel_size() );
    BOOST_CHECK( image.RGB_888_PIXEL == image.pixel_type() );
  }
  {
    ds::graphics::image image( 5, 5, ds::graphics::image::BGR_888_PIXEL );
    BOOST_CHECK( image.width() == 5 );
    BOOST_CHECK( image.height() == 5 );
    BOOST_CHECK( 3 == image.pixel_size() );
    BOOST_CHECK( image.BGR_888_PIXEL == image.pixel_type() );
  }

  {
    ds::graphics::image image( 5, 5, ds::graphics::image::RGB_565_PIXEL );
    BOOST_CHECK( image.width() == 5 );
    BOOST_CHECK( image.height() == 5 );
    BOOST_CHECK( 3 == image.pixel_size() );
    BOOST_CHECK( image.RGB_565_PIXEL == image.pixel_type() );
  }
  {
    ds::graphics::image image( 5, 5, ds::graphics::image::BGR_565_PIXEL );
    BOOST_CHECK( image.width() == 5 );
    BOOST_CHECK( image.height() == 5 );
    BOOST_CHECK( 3 == image.pixel_size() );
    BOOST_CHECK( image.BGR_565_PIXEL == image.pixel_type() );
  }
}

BOOST_AUTO_TEST_CASE( image_load )
{
  {
    ds::graphics::image image;
    bool ok = image.load("t/eyes.png");
    BOOST_CHECK( ok );
    BOOST_CHECK( image.width() == 60 );
    BOOST_CHECK( image.height() == 60 );
    BOOST_CHECK( 4 == image.pixel_size() );
    BOOST_CHECK( image.ABGR_8888_PIXEL == image.pixel_type() );
  }
  {
    ds::graphics::image image;
    bool ok = image.load("t/10x10-ABGR.png");
    BOOST_CHECK( ok );
    BOOST_CHECK( image.width () == 10 );
    BOOST_CHECK( image.height() == 10 );
    BOOST_CHECK( 4 == image.pixel_size() );
    BOOST_CHECK( image.ABGR_8888_PIXEL == image.pixel_type() );
  }
  {
    ds::graphics::image image;
    bool ok = image.load("t/10x10-ARGB.png");
    BOOST_CHECK( ok );
    BOOST_CHECK( image.width () == 10 );
    BOOST_CHECK( image.height() == 10 );
    BOOST_CHECK( 4 == image.pixel_size() );
    BOOST_CHECK( image.ARGB_8888_PIXEL == image.pixel_type() );
  }
  {
    ds::graphics::image image;
    bool ok = image.load("t/10x10-BGR.png");
    BOOST_CHECK( ok );
    BOOST_CHECK( image.width () == 10 );
    BOOST_CHECK( image.height() == 10 );
    BOOST_CHECK( 4 == image.pixel_size() );
    BOOST_CHECK( image.BGR_888_PIXEL == image.pixel_type() );
  }
  {
    ds::graphics::image image;
    bool ok = image.load("t/10x10-RGB.png");
    BOOST_CHECK( ok );
    BOOST_CHECK( image.width () == 10 );
    BOOST_CHECK( image.height() == 10 );
    BOOST_CHECK( 4 == image.pixel_size() );
    BOOST_CHECK( image.RGB_888_PIXEL == image.pixel_type() );
  }
}

BOOST_AUTO_TEST_CASE( image_save )
{
  {
    ds::graphics::image image;
    bool ok = image.load("t/eyes.png");
    BOOST_CHECK( ok );
    BOOST_CHECK( image.width() == 60 );
    BOOST_CHECK( image.height() == 60 );
    BOOST_CHECK( 4 == image.pixel_size() );
    BOOST_CHECK( image.ABGR_8888_PIXEL == image.pixel_type() );
    ok = image.save("test-save.png");
    BOOST_CHECK( ok );
    {
      ds::graphics::image m;
      ok = m.load("test-save.png");
      BOOST_CHECK( ok );
      BOOST_CHECK( m.width() == 60 );
      BOOST_CHECK( m.height() == 60 );
      BOOST_CHECK( 4 == m.pixel_size() );
      BOOST_CHECK( m.ABGR_8888_PIXEL == m.pixel_type() );
    }
  }
}
