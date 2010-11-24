#	Copyright Duzy Chan <code@duzy.info>
#

NULL :=

$(call sm-new-module, dsge, shared)

ds.ge.dir := $(sm.this.dir)

include $(ds.ge.dir)/check-deps.mk
include $(ds.ge.dir)/setup.mk

sm.this.verbose = true
sm.this.toolset = gcc

sm.this.compile.options.infile := true
sm.this.compile.options := \
  -DDS_TRACE_LEVEL=3 \
  -DDS_DEBUG_LEVEL=3 \
  -DDS_LOG_LEVEL=3 \
  -DDS_BUILDING_GRAPHICS=1 \
  $(NULL)

sm.this.link.options.infile := true
sm.this.link.options := \
  -Wl,--rpath,$(ds.ge.dir)/$(strip $(ds.third.dir.lib)) \
  $(NULL)

sm.this.includes := \
  $(ds.ge.dir)/include \
  $(ds.ui.dir)/include \
  $(ds.third.dir.inc) \
  $(ds.third.dir.inc)/zlib \
  $(ds.third.dir.inc)/libpng \
  $(ds.third.boost.dir) \
  $(ds.third.boost.geometry) \
  $(NULL)

sm.this.sources := \
  $(wildcard src/*.cpp) \
  $(wildcard src/gil/*.cpp) \
  $(NULL)

sm.this.libdirs := \
  $(ds.third.dir.lib) \
  $(ds.third.boost.dir.lib) \
  $(NULL)

$(call sm-check-not-empty, ds.third.skia.libname)
$(call sm-check-not-empty, ds.third.libpng.libname)
$(call sm-check-not-empty, ds.third.freetype.libname)

#  $(call ds.third.boost.use, system) \

sm.this.depends :=

ifeq ($(ds.ge.base),ds)
  sm.this.sources += $(wildcard src/ds/*.cpp)
else
ifeq ($(ds.ge.base),skia)
  sm.this.sources += $(wildcard src/skia/*.cpp)
  sm.this.includes += \
    $(ds.third.dir.inc)/skia/config \
    $(NULL)
  sm.this.libs += \
    $(ds.third.skia.libname) \
    $(ds.third.freetype.libname) \
    $(ds.third.libpng.libname) \
    $(ds.third.zlib.libname) \
    $(NULL)
else
ifeq ($(ds.ge.base),cairo)
  sm.this.sources += $(wildcard src/cairo/*.cpp)
else
ifeq ($(ds.ge.base),qt)
  QT := $(if $(sm.os.name.linux),/usr/local/Trolltech/Qt-4.7.1,c:/Qt/4.7.0)
  $(call sm-check-target-exists,$(QT))

  sm.this.sources += $(wildcard src/qt/*.cpp)
  sm.this.libs += $(if $(sm.os.name.win32),QtCored4 QtGuid4,QtCore QtGui pthread)
  sm.this.libdirs += $(QT)/lib
  sm.this.compile.options += -DQT=1
  sm.this.includes += $(QT)/include
  sm.this.link.options += -Wl,--rpath,$(if $(sm.os.name.win32),$(QT)/bin,$(QT)/lib)
endif#qt-based
endif#cairo-based
endif#skia-based
endif#ds-based

ifeq ($(sm.os.name),linux)
  sm.this.libs += pthread
  sm.this.targets += $(sm.out.lib)/libdsge.so
  $(sm.out.lib)/libdsge.so : $(sm.out.lib) $(sm.var.dsge.module_targets)
	$(call sm.tool.common.ln,$(sm.top)/$(sm.var.dsge.module_targets),$@)
else
ifeq ($(sm.os.name),win32)
  sm.this.compile.options += -mwindows
  sm.this.link.options += -mwindows \
    -Wl,--out-implib,$(sm.out.lib)/libdsge.a \
    -Wl,--enable-runtime-pseudo-reloc \
    -Wl,--enable-auto-import \
    $(NULL)
  sm.this.targets += $(sm.out.lib)/libdsge.a
  $(sm.out.lib)/libdsge.a : $(sm.out.lib) $(sm.var.dsge.module_targets)

  thirdlibs += \
    $(ds.ge.dir.bin)/$(ds.third.libpng.libname).so \
    $(ds.ge.dir.bin)/$(ds.third.libxml.libname).so
  $(call sm-copy-files,$(thirdlibs),$(ds.ge.dir.bin))
else
ifeq ($(sm.os.name),mac)
  # ...
endif#mac
endif#win32
endif#linux

$(call sm-build-this)
$(call sm-load-subdirs)

