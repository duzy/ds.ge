/**                                                             -*- c++ -*-
 *
 */

#include <boost/test/unit_test.hpp>
#include <ds/graphics/region.hpp>
#include <ds/graphics/image.hpp>
#include <ds/graphics/color.hpp>
#include <ds/graphics/box.hpp>
#include <ds/graphics/drawing_tools.hpp>
#include <ds/graphics/canvas.hpp>
#include <boost/geometry/algorithms/make.hpp>

using namespace ds::graphics;
using boost::geometry::make;

BOOST_AUTO_TEST_CASE( canvas_drawing )
{
  image m( 500, 500, image::ARGB_8888_PIXEL );
  canvas c( m );
  c.render( color::rgb(0.5, 0.5, 0.6) );

  pen   p = canvas::default_pen();
  brush b = canvas::default_brush();
  {
    b.color = color::rgba( 0.5, 0.1, 0.1, 1 );
    c.render( make<box>( 10, 10, 60, 60 ), b );
  }
  m.save( "test-canvas.png" );
}
