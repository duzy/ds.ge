/**                                                             -*- c++ -*-
 *
 */

#define BOOST_TEST_MODULE graphics
#include <boost/test/unit_test.hpp>
#include <ds/graphics/region.hpp>
#include <boost/geometry/algorithms/make.hpp>

using namespace ds::graphics;
using boost::geometry::make;

BOOST_AUTO_TEST_CASE( box_assign )
{
  {
    box nil;
    BOOST_CHECK( nil.empty() );
  }

  box b = make<box>( 10, 15, 110, 115 ); // [ left, top, right, bottom ]
  BOOST_CHECK( b.left()   == 10 );
  BOOST_CHECK( b.top()    == 15 );
  BOOST_CHECK( b.right()  == 110 );
  BOOST_CHECK( b.bottom() == 115 );
  BOOST_CHECK( b.x()      == 10 );
  BOOST_CHECK( b.y()      == 15 );
  BOOST_CHECK( b.width()  == 100 );
  BOOST_CHECK( b.height() == 100 );
  b.left      ( 20 );
  b.top       ( 20 );
  b.right     ( 120 );
  b.bottom    ( 120 );
  BOOST_CHECK( b.left()   == 20 );
  BOOST_CHECK( b.top()    == 20 );
  BOOST_CHECK( b.right()  == 120 );
  BOOST_CHECK( b.bottom() == 120 );
  BOOST_CHECK( b.x()      == 20 );
  BOOST_CHECK( b.y()      == 20 );
  BOOST_CHECK( b.width()  == 100 );
  BOOST_CHECK( b.height() == 100 );

  b.x( 25 ), b.y( 25 );
  BOOST_CHECK( b.x()      == 25 );
  BOOST_CHECK( b.y()      == 25 );
  BOOST_CHECK( b.width()  == 95 );
  BOOST_CHECK( b.height() == 95 );

  b.set_empty();
  BOOST_CHECK( b.empty() );

  b.resize( 10, 10 );
  BOOST_CHECK( b.x()      == 0 );
  BOOST_CHECK( b.y()      == 0 );
  BOOST_CHECK( b.width()  == 10 );
  BOOST_CHECK( b.height() == 10 );
}

BOOST_AUTO_TEST_CASE( box_intersect )
{
  box b1 = make<box>( 10, 10, 110, 110 );
  box b2 = make<box>( 50, 50, 150, 150 );
  BOOST_CHECK( b1.intersects(b2) );

  box b3 = b1.intersect( b2 );
  BOOST_CHECK( !b3.empty() );
  BOOST_CHECK( b3.x() == 50 );
  BOOST_CHECK( b3.y() == 50 );
  BOOST_CHECK( b3.right() == 110 );
  BOOST_CHECK( b3.bottom() == 110 );
}

BOOST_AUTO_TEST_CASE( box_contains )
{
  {
    box b1 = make<box>( 10, 10, 110, 110 );
    box b2 = make<box>( 20, 20, 100, 100 );
    BOOST_CHECK( b1.contains(b2) );
  }
  {
    box b1 = make<box>( 10, 20, 110, 120 );
    box b2 = make<box>( 11, 21, 110, 120 );
    BOOST_CHECK( b1.contains(b2) );
  }
  {
    box b1 = make<box>( 10, 20, 110, 120 );
    box b2 = make<box>( 11, 21, 111, 121 );
    BOOST_CHECK( !b1.contains(b2) );
  }
}
