#ifndef __DS_GRAPHICS_GIL_PNG_READER__HPP____by_Duzy_Chan__
#define __DS_GRAPHICS_GIL_PNG_READER__HPP____by_Duzy_Chan__ 1
#	include <ds/log.hpp>
#	include <cstdio> //!< for fopen, fclose in gil/extension/io on MinGW
extern "C" {
#	include <png.h>
#   if PNG_LIBPNG_VER_MAJOR==1 && PNG_LIBPNG_VER_MINOR==4 && PNG_LIBPNG_VER_RELEASE>=3
#      define png_infopp_NULL NULL /* for png_dynamic_io.hpp, libpng-1.4.x don't have */
#      define int_p_NULL NULL /* for png_dynamic_io.hpp, libpng-1.4.x don't have */
#   endif
}
#   include <boost/gil/channel.hpp>
#	include <boost/gil/extension/io/dynamic_io.hpp>
#	include <boost/gil/extension/io/png_io.hpp>
#	include <boost/gil/extension/io/png_dynamic_io.hpp>
#	include <istream>
#   include <cxxabi.h>
/*
  int cxa_status;
  std::clog<<__FILE__<<":"<<__LINE__<<":"<<abi::__cxa_demangle(typeid(view).name(), 0, 0, &cxa_status)<<std::endl;
*/

namespace ds { namespace graphics { namespace gil {

      /**
       *  @brief Read PNG image from std::istream
       */
      struct png_reader
      {
        static bool check( std::istream & is )
        {
          const std::size_t sigSize( boost::gil::detail::PNG_BYTES_TO_CHECK );
          char buf[ sigSize ];
          is.read( buf, sigSize );
          if ( is.gcount() != sigSize )
            { dsE("png_check_validity: failed to read file"); return false; }
          if ( png_sig_cmp((png_bytep)buf, (png_size_t)0, sigSize) != 0 )
            { dsE("png_check_validity: invalid png file"); return false; }
          return true; //!< good png stream
        }

        explicit png_reader( std::istream & is, bool doCheck = true )
          : _png  ( NULL )
          , _info ( NULL )
          , _is   ( is )
        {
          const std::size_t sigSize( boost::gil::detail::PNG_BYTES_TO_CHECK );
          if ( doCheck && !check(_is) ) { dsE("png_reader::check: bad png stream"); }
          _png = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
          if ( _png == NULL ) { dsE("failed to call png_create_read_struct()"); return; }
          _info = png_create_info_struct( _png );
          if ( _info == NULL ) {
            png_destroy_read_struct(&_png,png_infopp_NULL,png_infopp_NULL);
            dsE("failed to call png_create_info_struct()"); return;
          }
          if (setjmp(png_jmpbuf(_png))) {
            png_destroy_read_struct(&_png,&_info,png_infopp_NULL);
            dsE("failed to call setjmp()"); return;
          }
          png_set_read_fn(_png,this,&png_reader::read_data); //png_init_io(_png,get());
          png_set_sig_bytes(_png,sigSize);
          png_read_info(_png,_info);
          if (boost::gil::little_endian() && 8 < png_get_bit_depth(_png,_info))
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
          this->read(view);
        }

        template<
          template<typename> class Iterator,
          template<typename> class Locator,
          typename VT, typename PF, typename Layout >
        void read(const boost::gil::image_view<Locator<Iterator<boost::gil::packed_pixel<VT,PF,Layout>*> > > & view)
        {
          // TODO: read packed pixels
          int cxa_status;
          std::clog<<__FILE__<<":"<<__LINE__<<":TODO: read "<<abi::__cxa_demangle(typeid(view).name(), 0, 0, &cxa_status)<<std::endl;
        }

        template <typename Locator>
        void read(const boost::gil::image_view<Locator> & view)
        {
          using boost::gil::detail::png_read_support_private;
          using boost::gil::color_space_type;
          using boost::gil::channel_type;
          using boost::gil::io_error_if;
          using boost::gil::io_error;
          using boost::gil::pixel;
          using boost::gil::layout;
          //typedef typename channel_type<View>::type channel_t;
          //typedef typename color_space_type<View>::type color_space_t;
          typedef typename channel_type<Locator>::type channel_t;
          typedef typename color_space_type<Locator>::type color_space_t;
          typedef layout<color_space_t> layout_t;
          typedef pixel<channel_t, layout_t> pixel_t;

          png_uint_32 width, height;
          int bit_depth, color_type, interlace_type;
          png_get_IHDR(_png,_info, &width, &height,&bit_depth,&color_type,&interlace_type, int_p_NULL, int_p_NULL);
          io_error_if(((png_uint_32)view.width()!=width || (png_uint_32)view.height()!= height),
                      "png_read_view: input view size does not match PNG file size");

          if(png_read_support_private<channel_t,color_space_t>::bit_depth!=bit_depth ||
             png_read_support_private<channel_t,color_space_t>::color_type!=color_type)
            io_error("png_read_view: input view type is incompatible with the image type");

          std::vector<pixel_t> row(width);
          for(png_uint_32 y=0;y<height;++y) {
            png_read_row(_png,(png_bytep)&row.front(),NULL);
            std::copy(row.begin(),row.end(),view.row_begin(y));
          }
          png_read_end(_png,NULL);
        }

        template <typename Views>
        void read(const boost::gil::any_image_view<Views> & view)
        {
          using boost::gil::detail::png_read_is_supported;
          using boost::gil::detail::dynamic_io_fnobj;
          dynamic_io_fnobj <png_read_is_supported, png_reader> op(this);
          boost::gil::apply_operation(view,op);
        }

        template <class Pixel, bool IsPlanar, class Alloc>
        void read(/*const*/ boost::gil::image<Pixel, IsPlanar, Alloc>& m)
        {
          m.recreate(get_dimensions());
          this->read(view(m));
        }

        template <typename Images>
        void read(/*const*/ boost::gil::any_image<Images>& m)
        {
          using boost::gil::detail::png_type_format_checker;
          using boost::gil::detail::dynamic_io_fnobj;
          using boost::gil::construct_matched;
          png_uint_32 status, width, height;
          int bit_depth, color_type, interlace_type;
          status = png_get_IHDR(_png,_info, &width, &height,&bit_depth,&color_type,&interlace_type, int_p_NULL, int_p_NULL);
          if (status == 0) { boost::gil::io_error("png_reader_dynamic::read_image(): can't get PNG header info"); }
          if (!construct_matched(m,png_type_format_checker(bit_depth,color_type))) {
            boost::gil::io_error("png_reader_dynamic::read_image(): no matching image type between those of the given any_image and that of the file");
          } else {
            m.recreate(width,height);
            this->read( boost::gil::view(m) );
          }
        }

      private:
        static void read_data(png_structp ptr, png_bytep out, png_size_t sz)
        {
          png_reader * self((png_reader*)png_get_io_ptr(ptr));
          self->_is.read((char*)out, sz);
          /* dsI( self->_is.gcount() == sz ); */
          dsEif( self->_is.gcount() != sz, "png: hungry-read" );
        }

      protected:
        png_structp     _png;
        png_infop       _info;
        std::istream &  _is;
      };//struct png_reader

      struct png_writer
      {
        png_writer( std::ostream & os )
          : _png    ( NULL )
          , _info   ( NULL )
          , _os     ( os )
        {
          using boost::gil::io_error;
          using boost::gil::io_error_if;
          _png = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
          io_error_if(!_png,"png_write_initialize: fail to call png_create_write_struct()");
          _info = png_create_info_struct(_png);
          if (!_info) {
            png_destroy_write_struct(&_png,png_infopp_NULL);
            io_error("png_write_initialize: fail to call png_create_info_struct()");
          }
          if (setjmp(png_jmpbuf(_png))) {
            png_destroy_write_struct(&_png, &_info);
            io_error("png_write_initialize: fail to call setjmp(png_jmpbuf())");
          }
          png_set_write_fn(_png, this, &png_writer::write_data, &png_writer::flush_data); //png_init_io(_png,get());
        }

        ~png_writer() {
          png_destroy_write_struct(&_png,&_info);
        }

        template <typename View>
        void apply(const View& view)
        {
          this->write(view);
        }

        template<
          template<typename> class Iterator,
          template<typename> class Locator,
          typename VT, typename PF, typename Layout >
        void write(const boost::gil::image_view<Locator<Iterator<boost::gil::packed_pixel<VT,PF,Layout>*> > > & view)
        {
          // TODO: write packed pixels
          int cxa_status;
          std::clog<<__FILE__<<":"<<__LINE__<<":TODO: write "<<abi::__cxa_demangle(typeid(view).name(), 0, 0, &cxa_status)<<std::endl;
        }

        template <typename Locator>
        void write(const boost::gil::image_view<Locator> & view)
        {
          using boost::gil::detail::png_write_support_private;
          using boost::gil::color_space_type;
          using boost::gil::channel_type;
          using boost::gil::little_endian;
          using boost::gil::pixel;
          using boost::gil::layout;
          //typedef typename channel_type<View>::type channel_t;
          //typedef typename color_space_type<View>::type color_space_t;
          typedef typename channel_type<Locator>::type channel_t;
          typedef typename color_space_type<Locator>::type color_space_t;
          png_set_IHDR(_png, _info, view.width(), view.height(),
                       png_write_support_private<channel_t,color_space_t>::bit_depth,
                       png_write_support_private<channel_t,color_space_t>::color_type,
                       PNG_INTERLACE_NONE,
                       PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);
          png_write_info(_png,_info);
          if (little_endian() &&
              png_write_support_private<channel_t,color_space_t>::bit_depth>8)
            png_set_swap(_png);
          std::vector<pixel<channel_t,layout<color_space_t> > > row(view.width());
          for (int y=0;y<view.height();++y) {
            std::copy(view.row_begin(y),view.row_end(y),row.begin());
            png_write_row(_png,(png_bytep)&row.front());
          }
          png_write_end(_png,_info);
        }

        template <typename Views>
        void write(const boost::gil::any_image_view<Views>& v)
        {
          using boost::gil::detail::png_read_is_supported;
          using boost::gil::detail::dynamic_io_fnobj;
          dynamic_io_fnobj <png_read_is_supported, png_writer> op(this);
          boost::gil::apply_operation(v,op);
        }

        template <class Pixel, bool IsPlanar, class Alloc>
        void write(const boost::gil::image<Pixel, IsPlanar, Alloc>& m)
        {
          this->write(boost::gil::view(m));
        }

        template <typename Images>
        void write(const boost::gil::any_image<Images>& im)
        {
          typename boost::gil::any_image<Images>::const_view_t v = boost::gil::const_view(im);
          this->write(v);
        }

      private:
        static void write_data(png_structp ptr, png_bytep out, png_size_t sz)
        {
          png_writer * self((png_writer*)png_get_io_ptr(ptr));
          self->_os.write((char*)out, sz);
          //dsI( self->_os.pcount() == sz );
        }

        static void flush_data(png_structp ptr)
        {
          png_writer * self((png_writer*)png_get_io_ptr(ptr));
          self->_os.flush();
          //dsI( self->_os.pcount() == sz );
        }

      protected:
        png_structp _png;
        png_infop _info;
        std::ostream & _os;
      };//struct png_writer

    }//namespace gil
  }//namespace graphics
}//namespace ds

#endif//__DS_GRAPHICS_GIL_PNG_READER__HPP____by_Duzy_Chan__
