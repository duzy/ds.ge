/**                                                             -*- c++ -*-
 *
 */

#include <boost/test/unit_test.hpp>
#include <ds/graphics/region.hpp>
#include <ds/graphics/image.hpp>
#include <ds/graphics/color.hpp>
#include <ds/graphics/box.hpp>
#include <ds/graphics/segment.hpp>
#include <ds/graphics/line.hpp>
#include <ds/graphics/polygon.hpp>
#include <ds/graphics/drawing_tools.hpp>
#include <ds/graphics/canvas.hpp>
#include <boost/geometry/algorithms/make.hpp>

using namespace ds::graphics;
using boost::geometry::make;

BOOST_AUTO_TEST_CASE( canvas_drawing )
{
  /**
   *  skia is in the RGBA color space
   */
  //image m( 500, 500, image::ARGB_8888_PIXEL );
  image m( 500, 500, image::RGBA_8888_PIXEL );

  canvas c( m );
  c.render( color::rgba(0.2, 0.2, 0.6, 0.5) );

  pen   p = canvas::default_pen();
  brush b = canvas::default_brush();
  {
    b.color = color::rgba( 0.5, 0.1, 0.1, 1 );
    c.render( make<box>( 10, 10, 60, 60 ), b );
  }
  {
    point
      pt1 = make<point>( 60 , 60  ) ,
      pt2 = make<point>( 490, 490 ) ;
    segment l( pt1, pt2 );
    p.color = color::rgb(0.9, 0.05, 0.05);
    c.stroke( l, p );
  }
  {
    line l;
    l.first  = make<point>( 400, 450 ) ;
    l.second = make<point>( 450, 400 ) ;
    p.color = color::rgb(0.9, 0.05, 0.05);
    c.stroke( l, p );

    l.first  = make<point>( 400, 440 ) ;
    l.second = make<point>( 440, 400 ) ;
    p.color = color::rgb(0.05, 0.9, 0.05);
    c.stroke( l, p );

    l.first  = make<point>( 400, 430 ) ;
    l.second = make<point>( 430, 400 ) ;
    p.color = color::rgb(0.05, 0.05, 0.9);
    c.stroke( l, p );
  }
  {
    p.color = color::rgb(0.8, 0.1, 0.1);
    c.stroke( make<box>( 100, 100, 150, 150 ), p );
    p.color = color::rgb(0.1, 0.8, 0.1);
    c.stroke( make<box>( 105, 105, 155, 155 ), p );
    p.color = color::rgb(0.1, 0.1, 0.8);
    c.stroke( make<box>( 110, 110, 160, 160 ), p );
  }
  m.save( "test-canvas.png" );
}
