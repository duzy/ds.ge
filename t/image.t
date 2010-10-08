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
  BOOST_CHECK( 0 < image.pixel_size() );
  std::cout<<image.pixel_size()<<std::endl;
}
