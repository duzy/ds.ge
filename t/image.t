/**      							-*- c++ -*-
 *
 */

#include <ds/graphics/image.hpp>
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <iostream>

BOOST_AUTO_TEST_CASE( image )
{
  ds::graphics::image image;
  image.load("t/eyes.png");
  BOOST_CHECK( image.width() == 60 );
  BOOST_CHECK( image.height() == 60 );
  BOOST_CHECK( 4 == image.pixel_size() );
  BOOST_CHECK( image.ABGR_8888_PIXEL == image.pixel_type() );
}
