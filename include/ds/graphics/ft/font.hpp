/**
 *    Copyright 2008-05-06 DuzySoft.com, by Duzy Chan£¨Õ²ÐÀÃú£©
 *    All rights reserved by Duzy Chan£¨Õ²ÐÀÃú£©
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id: font.hpp 326 2009-01-11 04:00:29Z duzy $
 *
 **/

#ifndef __DS_GRAPHICS_FONT_HPP____by_Duzy_Chan__
#define __DS_GRAPHICS_FONT_HPP____by_Duzy_Chan__ 1
#       include <boost/gil/image.hpp>
#       include <boost/gil/typedefs.hpp>
#       include <boost/smart_ptr.hpp>
#       include <boost/tuple/tuple.hpp>
#       include <ds/debug.hpp>
#       include <vector>
#       include <string>
#       include <ft2build.h>
#       include FT_FREETYPE_H
#       include FT_GLYPH_H

namespace ds { namespace graphics {

#define Gm(fn, msg) do { error=(fn); dsIm(error==0, msg); } while(0)
#define G(fn) Gm((fn), "(font: no description)")

    /////////////////////////////////////////////////////////////////
    //  struct glyph
    /////////////////////////////////////////////////////////////////

    struct bound_box
    {
      FT_BBox box;

      bound_box()
        : box() {}

      bound_box( FT_BBox const & bb )
        : box(bb) {}

      int x_min() const { return box.xMin>>6; }
      int x_max() const { return box.xMax>>6; }
      int y_min() const { return box.yMin>>6; }
      int y_max() const { return box.yMax>>6; }
    };//struct bound_box

    struct transform_matrix
    {
      FT_Matrix matrix;

      transform_matrix()
        : matrix()
      {
        matrix.xx = 0;
        matrix.xy = 0;
        matrix.yx = 0;
        matrix.yy = 0;
      }

      transform_matrix( FT_Matrix const & m )
        : matrix(m) {}

      void set_scale( float horiFrac, float vertFrac = 1 )
      {
        matrix.xx = horiFrac * 0x10000L;
        matrix.yy = vertFrac * 0x10000L;
      }
    };//struct transform_matrix

    struct glyph
    {
      short charcode;
      FT_Vector origin; // the origin is in device space unit! 
      FT_Glyph image;

      glyph( short ch )
        : charcode(ch)
        , origin()
        , image()
      {
      }

      virtual ~glyph()
      {
        FT_Done_Glyph( image );
      }

      FT_BitmapGlyph bitmap_glyph() const
      {
        if ( image->format != FT_GLYPH_FORMAT_BITMAP ) {
          FT_Error e = FT_Glyph_To_Bitmap
            ( (FT_GlyphRec_**)&image,
              FT_RENDER_MODE_NORMAL, 0, 1 );
          dsI( e == 0 );
        }
        return (FT_BitmapGlyph)image;
      }


      typedef boost::gil::gray8_pixel_t pixel_t;
      typedef boost::gil::gray8_view_t view_t;
      typedef boost::gil::gray8c_view_t const_view_t;

      view_t view()
      {
        FT_BitmapGlyph bmg = bitmap_glyph();
        return boost::gil::interleaved_view
          ( bmg->bitmap.width, bmg->bitmap.rows,
            (pixel_t*)bmg->bitmap.buffer,
            sizeof(pixel_t) * bmg->bitmap.width );
      }

      bound_box bbox() const {//in 1/64th units
        FT_BBox bb;
        //FT_Glyph_Get_CBox( image, FT_GLYPH_BBOX_GRIDFIT, &bb );
        //FT_Glyph_Get_CBox( image, FT_GLYPH_BBOX_TRUNCATE, &bb );
        FT_Glyph_Get_CBox( image, FT_GLYPH_BBOX_PIXELS, &bb );
        return bound_box(bb);
      }

      std::size_t width() const {
        bound_box bb = bbox();
        return (bb.x_max() - bb.x_min())/*>>6*/;
      }

      std::size_t height() const {
        bound_box bb = bbox();
        return (bb.y_max() - bb.y_min())/*>>6*/;
      }

      int x() const { return origin.x; }
      int y() const { return origin.y; }

      int advance_x() const { return image->advance.x>>16; }
      int advance_y() const { return image->advance.y>>16; }

      void scale( float horiFrac, float vertFrac = 1 )
      {
        transform_matrix m;
        m.set_scale( horiFrac, vertFrac );
        transform( m );
      }

      void transform( transform_matrix const & m )
      {
        FT_Glyph_Transform( image,
                            &const_cast<transform_matrix&>(m).matrix,
                            0 /* translate */ );
      }
    };//struct glyph

    typedef boost::shared_ptr<glyph> glyph_ptr;

    /////////////////////////////////////////////////////////////////
    //  struct face
    /////////////////////////////////////////////////////////////////
 
    struct face
    {
      face() : ftFace_() {}

      /**!
       *  @brief Make glyph from a char code
       */
      template<typename CharT>
      glyph_ptr make_glyph( CharT ch, float kerning = 0/* 1/64th */,
                            float leading = 0 )
      {
        glyph_ptr g( new glyph(ch) );

        FT_Error error;

        #if true
        FT_UInt index = FT_Get_Char_Index( ftFace_, ch );
        G( FT_Load_Glyph( ftFace_, index, FT_LOAD_DEFAULT ) );
        G( FT_Render_Glyph( ftFace_->glyph, FT_RENDER_MODE_NORMAL ) );
        #else
        G( FT_Load_Char( ftFace_, ch, FT_LOAD_RENDER ) );
        #endif

        G( FT_Get_Glyph( ftFace_->glyph, &g->image ) );

        g->origin.x += FT_Int(kerning + .5);
        g->origin.y += FT_Int(leading + .5);
        return g;
      }

      /**!
       *  @brief Make glyphs from string.
       *
       *  The @c kerning will only effects the first glyph.
       */
      template<typename CharT>
      std::vector<glyph_ptr>
      make_glyphs( std::basic_string<CharT> const & s,
                   double kerning = 0, double leading = 0 )
      {
        FT_Vector pos;
        pos.x = FT_Int(kerning + .5);
        pos.y = 0;
        std::vector<glyph_ptr> v;
        if ( s.size() <= 0 )
          return v;
        v.push_back( make_glyph( s[0], kerning ) );
        for(std::size_t n=1; n < s.size(); ++n) {
          pos.x += ftFace_->glyph->advance.x;// >> 6;
          pos.y += ftFace_->glyph->advance.y;// >> 6;
          v.push_back( make_glyph( s[n], pos.x, pos.y ) );
        }
        return v;
      }

      /**
       *  @brief Make glyphs from array of kerned strings
       *  
       *  KernedString may be of type:
       *        boost::variant<double, std::string>
       *        boost::variant<std::string, double>
       */
      template<typename KernedString>
      std::vector<glyph_ptr>
      make_glyphs( std::vector<KernedString> const & s )
      {
        typedef std::string string_t;
        typename std::vector<KernedString>::const_iterator it;
        std::vector<glyph_ptr> v;
        const double *pk;
        const string_t *ps;
        FT_Vector pos;
        pos.x = 0;
        pos.y = 0;
        for( it = s.begin(); it != s.end(); ++it ) {
          if ( (pk = boost::get<const double>(&*it)) ) {
            pos.x += FT_Int( (*pk * 64) + .5 );
          }
          else if ( (ps = boost::get<const string_t>(&*it)) ) {
            std::vector<glyph_ptr> gs = make_glyphs(*ps, pos.x, pos.y);
            std::copy( gs.begin(), gs.end(), std::back_inserter(v) );
            pos.x += ftFace_->glyph->advance.x;// >> 6;
            pos.y += ftFace_->glyph->advance.y;// >> 6;
          }
        }
        return v;
      }

      int width( std::vector<glyph_ptr> const & gs )
      {
        int w = 0;
        std::vector<glyph_ptr>::const_iterator it;
        for( it = gs.begin(); it != gs.end(); ++it ) {
          w += (*it)->advance_x();
        }
        if ( 0 == w ) w = 1;
        return w;
      }

      int height( std::vector<glyph_ptr> const & gs )
      {
        int h = 0, tmp;
        std::vector<glyph_ptr>::const_iterator it;
        for( it = gs.begin(); it != gs.end(); ++it ) {
          //if ( h < (*it)->bitmap_glyph()->bitmap.rows )
          //  h = (*it)->bitmap_glyph()->bitmap.rows + 5;
          tmp = (*it)->height();
          if ( h < tmp )
            h = tmp;
        }
        if ( 0 == h ) h = 1;
        return h;
      }

      bool operator==( face const & o ) const
      { return ftFace_ == o.ftFace_; }

      bool operator!=( face const & o ) const
      { return !((*this) == o); }

      /**!
       *  @brief test validation
       *
       *  Return true if the face is valid
       */
      operator bool() const
      { return ftFace_ != 0; }

      void set_size( int w, int h = 0 )
      {
        FT_Error error;
        #if false
        // 1/64 uint
        G( FT_Set_Char_Size( ftFace_,
                             w * 64, /* width of points */
                             h * 64, /* height of points */
                             100, /* device horizontal resolution */
                             100 /* device vertical resolution */
                             ) );
        #else
        //!< 16.16 format
        G( FT_Set_Pixel_Sizes( ftFace_, w, h ) );
        #endif
      }

      int size_scale_x( int w ) const
      { return FT_MulFix( w, ftFace_->size->metrics.x_scale ); }

      int size_scale_y( int h ) const
      { return FT_MulFix( h, ftFace_->size->metrics.y_scale ); }

      int size_height() const { return ftFace_->size->metrics.height>>6; }

      double units_per_em() const { return ftFace_->units_per_EM; }
      double size_x_scale() const
      { return ftFace_->size->metrics.x_ppem / units_per_em(); }
      double size_y_scale() const
      { return ftFace_->size->metrics.y_ppem / units_per_em(); }

      FT_Int advance_x() const
      { return size_scale_x(ftFace_->glyph->advance.x>>6); }

      FT_Int advance_y() const
      { return size_scale_y(ftFace_->glyph->advance.y>>6); }

      int ascender() const { return ftFace_->size->metrics.ascender>>6; }
      int descender() const { return ftFace_->size->metrics.descender>>6; }
      int glyph_width() const { return ftFace_->glyph->metrics.width>>6; }
      int glyph_height() const { return ftFace_->glyph->metrics.height>>6; }

      int glyph_horizontal_bearing_x() const
      { return ftFace_->glyph->metrics.horiBearingX>>6; }
      int glyph_horizontal_bearing_y() const
      { return ftFace_->glyph->metrics.horiBearingY>>6; }
      int glyph_vertical_bearing_x() const
      { return ftFace_->glyph->metrics.vertBearingX>>6; }
      int glyph_vertical_bearing_y() const
      { return ftFace_->glyph->metrics.vertBearingY>>6; }

      void set_transform( transform_matrix const & m )
      {
        ::FT_Set_Transform( ftFace_,
                            &const_cast<transform_matrix&>(m).matrix,
                            0 /* translate */ );
      }

    private:
      friend struct font;
      friend struct text_renderer;

      face( FT_Library ft, std::string const & ff, int i = 0 )
        : ftFace_()
      {
        FT_Error error;
        Gm( FT_New_Face( ft, /* FreeType Library */
                         ff.c_str(),
                         i, /* Face Index */
                         &ftFace_
                         ),
            "Create new FreeType face of font: "<<ff );
        set_size( 16 );
      }

      template<typename BufferType>
      face( FT_Library ft, BufferType buffer, std::size_t size, int i = 0 )
        : ftFace_()
      {
        FT_Error error;
        Gm( FT_New_Memory_Face( ft, /* FreeType Library */
                                buffer,
                                size,
                                i, /* Face Index */
                                &ftFace_ ),
            "Create new FreeType face from buffer" );
        set_size( 16 );
      }

      //virtual ~face()
      //{
      //  FT_Done_Face( ftFace_ );
      //}
      void done()
      {
        FT_Done_Face( ftFace_ );
        ftFace_ = 0;
      }

    private:
      FT_Face ftFace_;
    };//struct face

    //_______________________________________________________________


    /////////////////////////////////////////////////////////////////
    //  struct font
    /////////////////////////////////////////////////////////////////
 
    /**
     *  @brief Manages faces
     */
    struct font
    {
      font()
        : ft_()
        , faces_()
      {
        FT_Error error;
        G( FT_Init_FreeType( &ft_ ) );
      }

      virtual ~font()
      {
        std::vector<face>::iterator
          it = faces_.begin();
        for(; it != faces_.end(); ++it) {
          it->done();
        }
        FT_Done_FreeType( ft_ );
      }

      /**!
       *  @brief class font::index for indexing face
       */
      struct index
      {
        index()
          : i(-1) {}

        /**
         *  @brief test validation
         *
         *  Returns true if index is valid.
         */
        operator bool() const
        { return std::size_t(-1) != i; }

      private:
        friend struct font;

        index( std::size_t _i )
          : i(_i) {}

        operator std::size_t() const
        { return i; }

      private:
        std::size_t i;
      };//struct index

      //--------------------------------------------------

      index load_face( std::string const & fontFile, int i = 0 )
      {
        face f( face( ft_, fontFile, i ) );
        faces_.push_back( f );
        return index( faces_.size() - 1 );
      }

      template<typename RandomIter>
      index load_face( RandomIter buffer, std::size_t size )
      {
        face f( face( ft_, buffer, size ) );
        faces_.push_back( f );
        return index( faces_.size() - 1 );
      }

      face & operator[]( index i )
      {
        dsI( 0 < faces_.size() );
        dsI( std::size_t(i) < faces_.size() );
        return faces_[i];
      }

      std::size_t size() const { return faces_.size(); }

    protected:
      FT_Library ft_;
      std::vector<face> faces_;
    };//struct font

#undef Gm
#undef G

  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_FONT_HPP____by_Duzy_Chan__
