/**      							-*- c++ -*-
 *
 */

#include <ds/graphics/image.hpp>
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <iostream>

BOOST_AUTO_TEST_CASE( image )
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
