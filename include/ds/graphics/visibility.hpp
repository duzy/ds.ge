#ifndef __DS_GRAPHICS_VISIBILITY__hpp____by_Duzy_Chan__
#define __DS_GRAPHICS_VISIBILITY__hpp____by_Duzy_Chan__ 1
#if defined _WIN32 || defined __CYGWIN__
#  ifdef DS_BUILDING_GRAPHICS
#    ifdef __GNUC__
#      define DS_GRAPHICS_PUBLIC __attribute__((dllexport))
#    else
#      define DS_GRAPHICS_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#    endif
#  else
#    ifdef __GNUC__
#      define DS_GRAPHICS_PUBLIC __attribute__((dllimport))
#    else
#      define DS_GRAPHICS_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#    endif
#  endif /* DS_BUILDING_GRAPHICS */
#  define DS_GRAPHICS_PRIVATE
#else
#  if __GNUC__ >= 4
#    define DS_GRAPHICS_PUBLIC __attribute__ ((visibility("default")))
#    define DS_GRAPHICS_PRIVATE  __attribute__ ((visibility("hidden")))
#  else
#    define DS_GRAPHICS_PUBLIC
#    define DS_GRAPHICS_PRIVATE
#  endif
#endif
#endif//__DS_GRAPHICS_VISIBILITY__hpp____by_Duzy_Chan__
