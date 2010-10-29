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
#include <boost/geometry/algorithms/assign.hpp>
#include <boost/geometry/geometries/adapted/c_array_cartesian.hpp>

using namespace ds::graphics;
using boost::geometry::assign;
using boost::geometry::make;

BOOST_AUTO_TEST_CASE( canvas_drawing )
{
  //const image::PixelType pixel_type = image::ARGB_8888_PIXEL;
  const image::PixelType pixel_type = image::RGBA_8888_PIXEL; // skia's color space 
  image m( 500, 500, pixel_type );

  canvas c( m );
  c.render( color::rgba(0.2, 0.2, 0.6, 0.5) );

  pen   p = canvas::default_pen();
  brush b = canvas::default_brush();
  {
    b.color = color::rgba( 0.5, 0.1, 0.1, 0.8 );
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
  {
    polygon g;
    {
      const coordinate_t coords[][2] = {
        {100.0, 100.0}, {150.0, 100.0}, {200.0,  50.0}, {250.0, 100.0},
        {300.0, 100.0}, {250.0, 150.0}, {300.0, 200.0}, {200.0, 250.0},
        {150.0, 150.0}, {100.0, 100.0} // closing point is opening point
      };
      assign(g, coords);
    }
    // {
    //   g.inners().resize(1);
    //   const coordinate_t coords[][2] = {
    //     {40, 20}, {70, 14}, {48, 90}, {22, 80},
    //     {40, 20} // closing point is opening point
    //   };
    //   assign(g.inners().back(), coords);
    // }
    b.color = color::rgba( 0.15, 0.25, 0.15, 0.5 );
    p.color = color::rgba( 0.25, 0.50, 0.25, 1.0 );
    c.render( g, b );
    c.stroke( g, p );
  }
  {
    image mm;
    BOOST_CHECK( mm.load("t/eyes.png") );
    BOOST_CHECK( mm.ABGR_8888_PIXEL == mm.pixel_type() );
    BOOST_CHECK( mm.convert_pixels( pixel_type ) );
    c.render( mm, 400, 30 );
  }
  m.save( "test-canvas.png" );
}
