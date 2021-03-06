/**
 *    Copyright 2010-08-29 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#ifndef __DS_GRAPHICS_DRAWING_TOOLS__hpp____by_Duzy_Chan__
#define __DS_GRAPHICS_DRAWING_TOOLS__hpp____by_Duzy_Chan__ 1
#       include "color.hpp"

namespace ds { namespace graphics {

    struct drawing_tool
    {
      graphics::color color;
      bool anti_alias : 1; // pass in 'const bool &' ?
    };

    struct brush : drawing_tool
    {
    };

    struct pen : drawing_tool
    {
    };

  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_DRAWING_TOOLS__hpp____by_Duzy_Chan__
