#
#

$(call sm-new-module, dsge, shared)

ds.ge.dir := $(sm.this.dir)
ds.ge.dir.lib := $(ds.ge.dir)/out/$V/lib
ds.ge.dir.bin := $(ds.ge.dir)/out/$V/bin
ds.ge.qt_based := false
ds.ui.dir := $(ds.ge.dir)/../ui
#ds.ui.dir.lib := $(ds.ui.dir)/out/$V/lib

include $(ds.ge.dir)/check-deps.mk

sm.this.verbose = true
sm.this.toolset = gcc

sm.this.compile.options.infile := true
sm.this.compile.options := \
  -DDS_TRACE_LEVEL=3 \
  -DDS_DEBUG_LEVEL=3 \
  -DDS_LOG_LEVEL=3 \
  -DDS_BUILDING_GRAPHICS=1 \

sm.this.link.options.infile := true
sm.this.link.options := \
  -Wl,--no-undefined \
  -Wl,--rpath,$(ds.ge.dir)/$(strip $(ds.third.dir.lib))

sm.this.includes := \
  $(ds.ge.dir)/include \
  $(ds.ui.dir)/include \
  $(ds.third.dir.inc) \
  $(ds.third.dir.inc)/zlib \
  $(ds.third.dir.inc)/libpng \
  $(ds.third.dir.inc)/skia/config \
  $(ds.third.boost.dir) \
  $(ds.third.boost.geometry)

sm.this.sources := \
  $(wildcard src/*.cpp) \
  $(wildcard src/gil/*.cpp) \
  $(wildcard src/skia/*.cpp) \

sm.this.libdirs := \
  -L$(ds.third.dir.lib) \
  -L$(ds.third.boost.dir.lib)

$(call sm-check-not-empty, ds.third.skia.libname)
$(call sm-check-not-empty, ds.third.libpng.libname)
$(call sm-check-not-empty, ds.third.freetype.libname)
sm.this.libs += \
  $(ds.third.skia.libname) \
  $(ds.third.libpng.libname) \
  $(ds.third.freetype.libname) \
  $(ds.third.zlib.libname) \

#  $(call ds.third.boost.use, system) \

sm.this.depends :=

ifeq ($(ds.ge.qt_based),true)
  QT := $(if $(sm.os.name.linux),/usr/local/Trolltech/Qt-4.7.1,c:/Qt/4.7.0)
  $(call sm-check-target-exists,$(QT))

  sm.this.sources += $(wildcard src/qt/*.cpp)
  sm.this.libs += $(if $(sm.os.name.win32),QtCored4 QtGuid4,QtCore QtGui pthread)
  sm.this.libdirs += $(QT)/lib
  sm.this.compile.options += -DQT=1
  sm.this.includes += $(QT)/include
  sm.this.link.options += -Wl,--rpath,$(if $(sm.os.name.win32),$(QT)/bin,$(QT)/lib)
endif#qt-based

ifeq ($(sm.os.name),linux)
  sm.this.libs += pthread
  sm.this.depends += $(sm.out.lib)/libdsge.so
  $(sm.out.lib)/libdsge.so : $(sm.out.lib) $(sm.var.dsge.targets)
	$(call sm.tool.common.ln,$(sm.top)/$(sm.var.dsge.targets),$@)
else
ifeq ($(sm.os.name),win32)
  sm.this.compile.options += -mwindows
  sm.this.link.options += -mwindows \
    -Wl,--out-implib,$(sm.out.lib)/libdsge.a \
    -Wl,--enable-runtime-pseudo-reloc \
    -Wl,--enable-auto-import
  sm.this.depends += $(sm.out.lib)/libdsge.a
  $(sm.out.lib)/libdsge.a : $(sm.out.lib) $(sm.this.targets)

  sm.this.depends += \
    $(ds.ge.dir.bin)/$(ds.third.libpng.libname).so \
    $(ds.ge.dir.bin)/$(ds.third.libxml.libname).so
  $(ds.ge.dir.bin)/$(ds.third.libpng.libname).so: $(ds.third.dir.bin)/$(ds.third.libpng.libname).so ; @cp -vf $< $@
  $(ds.ge.dir.bin)/$(ds.third.libxml.libname).so: $(ds.third.dir.bin)/$(ds.third.libxml.libname).so ; @cp -vf $< $@
else
ifeq ($(sm.os.name),mac)
  # ...
endif#mac
endif#win32
endif#linux

$(call sm-build-this)
$(call sm-load-subdirs)
