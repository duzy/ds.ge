/**      							-*- c++ -*-
 *
 */

#include <ds/graphics/gil/image.hpp>
#include <ds/graphics/gil/png_io.hpp>
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <typeinfo>
#include <iostream>
#include <cxxabi.h>

template<typename T>
struct check_image_type_f
{
  typedef bool result_type;

  result_type result;

  int pixel_size;

  check_image_type_f() : result( false ), pixel_size(0) {}

  result_type operator()( /*const*/ T & ) /*const*/
  {
    pixel_size = sizeof(typename T::value_type);
    return result = true;
  }

  template<typename U>
  result_type operator()( /*const*/ U & o ) /*const*/
  {
    pixel_size = sizeof(typename T::value_type);

    int status;
    std::cerr
      <<__FILE__<<":"<<__LINE__<<": unexpected type: "
      <<abi::__cxa_demangle(typeid(o).name(), 0, 0, &status)
      <<std::endl
      ;

    return result = false;
  }

  operator bool() const { return result; }
};//struct check_image_type_f

template<typename T, typename U>
bool check_image_type( /*const*/ U & o )
{
  check_image_type_f<T> f;
  return boost::gil::apply_operation( o, f );
}

struct get_image_pixel_size_f
{
  typedef int result_type;

  template<class T> result_type operator()( T & o )
  {
    return sizeof(typename T::value_type);
  }
};//struct get_image_pixel_size_f

template<typename M>
int image_pixel_size( /*const*/ M & o )
{
  get_image_pixel_size_f f;
  return boost::gil::apply_operation( o, f );
}

BOOST_AUTO_TEST_CASE( png_reader_test )
{
  typedef ds::graphics::gil::abgr8_image_t image_t;

  {
    std::ifstream i( "eyes.png", i.binary|i.in );
    if ( !i ) i.open( "t/eyes.png", i.binary|i.in );
    BOOST_CHECK( i );

    ds::graphics::gil::png_reader rdr( i );
    ds::graphics::gil::image m;
    rdr.read_image( m.any() );
    BOOST_CHECK( m.width () == 60 );
    BOOST_CHECK( m.height() == 60 );
    BOOST_CHECK( image_pixel_size(m.any()) == 4 );
    BOOST_CHECK( check_image_type<image_t>( m.any() ) );

    boost::gil::png_write_view( "test-out.png", view(m.any()) );
  }

  { //== Check test-out.png
    std::ifstream is( "test-out.png", is.binary|is.in );
    BOOST_CHECK( is );

    ds::graphics::gil::image m;
    ds::graphics::gil::png_reader r( is );
    r.read_image( m.any() );
    BOOST_CHECK( m.width () == 60 );
    BOOST_CHECK( m.height() == 60 );
    BOOST_CHECK( image_pixel_size(m.any()) == 4 );
    BOOST_CHECK( check_image_type<image_t>( m.any() ) );
  }
}

template<typename image_t, std::size_t PixelSize>
void test_png_writer()
{
  /*
  int status;
  std::clog
    <<"test_png_writer<"
    <<abi::__cxa_demangle(typeid(image_t).name(), 0, 0, &status)
    <<">"
    <<std::endl
    ;
  */

  ds::graphics::gil::image image(image_t(5,5));
  BOOST_CHECK( image.width () == 5 );
  BOOST_CHECK( image.height() == 5 );
  BOOST_CHECK( image_pixel_size(image.any()) == PixelSize );
  BOOST_CHECK( check_image_type<image_t>(image.any()) );

  {
    std::ofstream os( "test-out.png", os.out | os.binary );
    BOOST_CHECK( os );

    ds::graphics::gil::png_writer w( os );
    //w.write_image( image );
    w.write_image( image.any() );
  }
  {
    std::ifstream i( "test-out.png", i.binary | i.in );
    BOOST_CHECK( i );

    ds::graphics::gil::image m;
    ds::graphics::gil::png_reader r( i );
      
    r.read_image( m.any() );

    BOOST_CHECK( m.width () == 5 );
    BOOST_CHECK( m.height() == 5 );
    BOOST_CHECK( image_pixel_size(m.any()) == PixelSize );
    BOOST_CHECK( check_image_type<image_t>( image.any() ) );
  }
}

BOOST_AUTO_TEST_CASE( png_writer_test )
{
  test_png_writer<ds::graphics::gil::rgba8_image_t, 4>();
  test_png_writer<ds::graphics::gil::bgra8_image_t, 4>();
  test_png_writer<ds::graphics::gil::argb8_image_t, 4>();
  test_png_writer<ds::graphics::gil::abgr8_image_t, 4>();
  test_png_writer<ds::graphics::gil::rgb8_image_t,  3>();
  test_png_writer<ds::graphics::gil::bgr8_image_t,  3>();

  test_png_writer<ds::graphics::gil::rgba4_image_t, 2>();
  test_png_writer<ds::graphics::gil::bgra4_image_t, 2>();
  test_png_writer<ds::graphics::gil::argb4_image_t, 2>();
  test_png_writer<ds::graphics::gil::abgr4_image_t, 2>();
  test_png_writer<ds::graphics::gil::rgb565_image_t,2>();
  test_png_writer<ds::graphics::gil::bgr565_image_t,2>();
}
