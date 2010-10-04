/**                                                             -*- c++ -*-
 *
 */

#include <boost/test/unit_test.hpp>
#include <ds/graphics/region.hpp>
#include <boost/geometry/algorithms/make.hpp>
#include <iostream>

using namespace ds::graphics;
using boost::geometry::make;

BOOST_AUTO_TEST_CASE( region_merge )
{
  {
    box b;
    BOOST_CHECK( b.empty() );
    
    region r;
    BOOST_CHECK( r.empty() );
  }
  {
    region reg1( make<box>( 10, 10, 110, 110 ) );
    //reg1.dump( std::cout ), std::cout << std::endl;
    BOOST_CHECK( reg1.bounds() == make<box>(10, 10, 110, 110) );
    BOOST_CHECK( reg1.boxes().empty() );

    reg1 |= make<box>( 50, 50, 200, 200 );
    //reg1.dump( std::cout ), std::cout << std::endl;
    BOOST_CHECK( reg1.bounds() == make<box>(10, 10, 200, 200) );
    BOOST_CHECK( reg1.boxes().size() == 3 );
    BOOST_CHECK( reg1.boxes()[0] == make<box>(10,10,110,50) );
    BOOST_CHECK( reg1.boxes()[1] == make<box>(10,50,200,110) );
    BOOST_CHECK( reg1.boxes()[2] == make<box>(50,110,200,200) );

    reg1 |= make<box>( 50, 200, 200, 300 );
    //reg1.dump( std::cout ), std::cout << std::endl;
    BOOST_CHECK( reg1.bounds() == make<box>(10, 10, 200, 300) );
    BOOST_CHECK( reg1.boxes().size() == 3 );
    BOOST_CHECK( reg1.boxes()[0] == make<box>(10,10,110,50) );
    BOOST_CHECK( reg1.boxes()[1] == make<box>(10,50,200,110) );
    BOOST_CHECK( reg1.boxes()[2] == make<box>(50,110,200,300) );
  }
  {
    box b1(make<box>( 10, 10, 110, 110 ));
    region reg1( b1 );
    region reg2( b1 );
    region reg3 = reg1 | reg2;

    /*
      reg1.dump( std::cout ), std::cout << std::endl;
      reg2.dump( std::cout ), std::cout << std::endl;
      reg3.dump( std::cout ), std::cout << std::endl;
    */

    BOOST_CHECK( 0 < reg3.bounds().width() );
    BOOST_CHECK( 0 < reg3.bounds().height() );
    BOOST_CHECK( reg1.bounds().width() == reg3.bounds().width() );
    BOOST_CHECK( reg2.bounds().width() == reg3.bounds().width() );
    BOOST_CHECK( reg1.bounds() == reg3.bounds() );
    BOOST_CHECK( reg2.bounds() == reg3.bounds() );
    BOOST_CHECK( reg3.boxes().empty() );
  }
  {
    region r;
    BOOST_CHECK( r.empty() );

    r |= make<box>( 50, 60, 150, 160 );
    //r.dump( std::cout );
    BOOST_CHECK( !r.empty() );
    BOOST_CHECK( r.size() == 1 );
    BOOST_CHECK( r.bounds() == make<box>( 50, 60, 150, 160 ) );

    int count = 10;
    box b = make<box>( 100, 10, 150, 160 );
    while ( !b.empty() && 0 < --count )
      {
        r |= b;
        //r.dump( std::cout );

        BOOST_CHECK( !r.empty() );

        int n;
        for (n = 0; n < r.boxes().size()-2; ++n) {
          BOOST_CHECK( r.boxes()[n] == make<box>( 100-n, 10+n, 150, 11+n ) );
        }

        BOOST_CHECK( r.boxes()[n] == make<box>( 100-n, 10+n, 150, 60 ) );

        b.left( b.left() - 1 );
        b.top( b.top() + 1 );
        if ( b.left() <= 0 ) break;
        if ( b.bottom() <= b.top () ) break;
      }//while
  }
}
