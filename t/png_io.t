/**      							-*- c++ -*-
 *
 */

#include <ds/graphics/gil/image.hpp>
#include <ds/graphics/gil/png_io.hpp>
#include <boost/test/unit_test.hpp>
#include <fstream>

BOOST_AUTO_TEST_CASE( png_reader )
{
  std::ifstream ifs( "eyes.png", ifs.binary|ifs.in );
  if ( !ifs ) ifs.open( "t/eyes.png", ifs.binary|ifs.in );
  BOOST_CHECK( ifs );

  ds::graphics::gil::png_reader rdr( ifs );
  ds::graphics::gil::image img;
  rdr.read_image( img.any() );
  BOOST_CHECK( img.width() == 60 );
  BOOST_CHECK( img.height() == 60 );

  boost::gil::png_write_view( "test-out.png", view(img.any()) );

  //== Check test-out.png
  std::ifstream ifs2( "test-out.png", ifs2.binary|ifs2.in );
  BOOST_CHECK( ifs2 );
  ds::graphics::gil::image img2;
  rdr.read_image( img2.any() );
  BOOST_CHECK( img2.width() == 60 );
  BOOST_CHECK( img2.height() == 60 );
}

BOOST_AUTO_TEST_CASE( png_writer )
{
  {
    std::ofstream os( "test-out.png", os.out | os.binary );
    ds::graphics::gil::image image(ds::graphics::gil::rgba8_image_t(5,5));
    ds::graphics::gil::png_writer w( os );
    w.write_image( image );
    {
      std::ifstream i( "test-out.png", i.binary | i.in );
      BOOST_CHECK( i );
      ds::graphics::gil::image m;
      rdr.read_image( m.any() );
      BOOST_CHECK( m.width() == 5 );
      BOOST_CHECK( m.height() == 5 );
    }
  }
}

