#ifndef __DS_GRAPHICS_GIL_PNG_READER__HPP____by_Duzy_Chan__
#define __DS_GRAPHICS_GIL_PNG_READER__HPP____by_Duzy_Chan__ 1
#	include <ds/debug.hpp>
#	include <cstdio> //!< for fopen, fclose in gil/extension/io on MinGW
extern "C" {
#	include <png.h>
#   if PNG_LIBPNG_VER_MAJOR==1 && PNG_LIBPNG_VER_MINOR==4 && PNG_LIBPNG_VER_RELEASE>=3
#      define png_infopp_NULL NULL /* for png_dynamic_io.hpp, libpng-1.4.x don't have */
#      define int_p_NULL NULL /* for png_dynamic_io.hpp, libpng-1.4.x don't have */
#   endif
}
#	include <boost/gil/extension/io/dynamic_io.hpp>
#	include <boost/gil/extension/io/png_io.hpp>
#	include <boost/gil/extension/io/png_dynamic_io.hpp>
#	include <istream>

namespace ds { namespace graphics { namespace gil {

      /**
       *  @brief Read PNG image from std::istream
       */
      struct png_reader
      {
        png_reader( std::istream & is )
          : _png(NULL)
          , _info(NULL)
          , _is(is)
        {
          const std::size_t sigSize( boost::gil::detail::PNG_BYTES_TO_CHECK );
          char buf[ sigSize ];
          is.read( buf, sigSize );
          if ( is.gcount() != sigSize )
            { dsL("png_check_validity: failed to read file"); return; }
          if ( png_sig_cmp((png_bytep)buf, (png_size_t)0, sigSize) != 0 )
            { dsL("png_check_validity: invalid png file"); return; }
          _png = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
          if ( _png == NULL )
            { dsL("failed to call png_create_read_struct()"); return; }
          _info = png_create_info_struct( _png );
          if ( _info == NULL ) {
            png_destroy_read_struct(&_png,png_infopp_NULL,png_infopp_NULL);
            dsL("failed to call png_create_info_struct()"); return;
          }
          if (setjmp(png_jmpbuf(_png))) {
            png_destroy_read_struct(&_png,&_info,png_infopp_NULL);
            dsL("failed to call setjmp()"); return;
          }
          //png_init_io(_png,get());
          png_set_read_fn(_png,this,&png_reader::read_data);
          png_set_sig_bytes(_png,sigSize);
          png_read_info(_png,_info);
          if (boost::gil::little_endian() && png_get_bit_depth(_png,_info)>8)
            png_set_swap(_png);
        }

        virtual ~png_reader()
        {
          png_destroy_read_struct(&_png,&_info,png_infopp_NULL);
        }

        boost::gil::point2<std::ptrdiff_t> get_dimensions()
        {
          return boost::gil::point2<std::ptrdiff_t>
            (png_get_image_width(_png,_info),png_get_image_height(_png,_info));
        }

        template <typename View>
        void apply(const View& view)
        {
          png_uint_32 width, height;
          int bit_depth, color_type, interlace_type;
          png_get_IHDR(_png,_info, &width, &height,&bit_depth,&color_type,&interlace_type, int_p_NULL, int_p_NULL);
          boost::gil::io_error_if
            (((png_uint_32)view.width()!=width || (png_uint_32)view.height()!= height),
             "png_read_view: input view size does not match PNG file size");

          using boost::gil::detail::png_read_support_private;
          using boost::gil::color_space_type;
          using boost::gil::channel_type;
          if(png_read_support_private<typename channel_type<View>::type,typename color_space_type<View>::type>::bit_depth!=bit_depth ||
             png_read_support_private<typename channel_type<View>::type,typename color_space_type<View>::type>::color_type!=color_type)
            boost::gil::io_error("png_read_view: input view type is incompatible with the image type");

          using boost::gil::pixel;
          using boost::gil::layout;
          std::vector<pixel<typename channel_type<View>::type, layout<typename color_space_type<View>::type> > > row(width);
          for(png_uint_32 y=0;y<height;++y) {
            png_read_row(_png,(png_bytep)&row.front(),NULL);
            std::copy(row.begin(),row.end(),view.row_begin(y));
          }
          png_read_end(_png,NULL);
        }

        template <typename Image>
        void read_image(Image& im) {
          im.recreate(get_dimensions());
          apply(view(im));
        }

        template <typename Images>
        void read_image(boost::gil::any_image<Images>& im)
        {
          using boost::gil::detail::png_type_format_checker;
          using boost::gil::construct_matched;
          png_uint_32 width, height;
          int bit_depth, color_type, interlace_type;
          png_get_IHDR(_png,_info, &width, &height,&bit_depth,&color_type,&interlace_type, int_p_NULL, int_p_NULL);
          if (!construct_matched(im,png_type_format_checker(bit_depth,color_type))) {
            boost::gil::io_error("png_reader_dynamic::read_image(): no matching image type between those of the given any_image and that of the file");
          } else {
            im.recreate(width,height);
            boost::gil::detail::dynamic_io_fnobj
              <boost::gil::detail::png_read_is_supported, png_reader> op(this);
            boost::gil::apply_operation(boost::gil::view(im),op);
          }
        }

      private:
        static void read_data(png_structp ptr, png_bytep out, png_size_t sz)
        {
          png_reader *that((png_reader*)png_get_io_ptr(ptr));
          that->_is.read((char*)out, sz);
        }

      protected:
        png_structp _png;
        png_infop _info;
        std::istream & _is;
      };//struct png_reader

    }//namespace gil
  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_GIL_PNG_READER__HPP____by_Duzy_Chan__
