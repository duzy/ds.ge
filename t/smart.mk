#

$(call sm-new-module, graphics-unit-tests, tests, gcc)
$(call sm-check-not-empty, ds.dir)
$(call sm-check-not-empty, ds.ge.dir)
$(call sm-check-not-empty, ds.ge.dir.lib)

sm.this.verbose := true
sm.this.lang := c++

sm.this.includes := \
  $(ds.dir)/include \
  $(ds.ge.dir)/include \
  $(ds.third.dir.inc) \
  $(ds.third.dir.inc)/zlib \
  $(ds.third.dir.inc)/libpng \
  $(ds.third.boost.dir) \
  $(ds.third.boost.geometry)

sm.this.sources := \
  box.t \
  canvas.t \
  region.t \
  png_io.t \
  image.t \

sm.this.link.flags := \
  -Wl,--rpath,$(ds.ge.dir.lib) \
  -Wl,--rpath,$(ds.third.dir.lib)

sm.this.libdirs := \
  $(ds.ge.dir.lib) \
  $(ds.third.dir.lib) \
  $(ds.third.boost.dir.lib)

sm.this.libs := dsge \
  $(call ds.third.boost.use, system) \
  $(call ds.third.boost.use, filesystem) \
  $(call ds.third.boost.use, unit_test_framework) \
  $(ds.third.libpng.libname) \

$(sm-build-this)
