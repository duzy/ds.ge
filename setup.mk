$(call sm-check-not-empty, ds.ge.dir)
$(call sm-check-target-exists,$(ds.ge.dir))

# Options for ds.ge.base: qt, skia, agg, ds(my own engine base)
ds.ge.base := agg
ds.ge.dir.lib := $(ds.ge.dir)/out/$V/lib
ds.ge.dir.bin := $(ds.ge.dir)/out/$V/bin
ds.ui.dir := $(ds.ge.dir)/../ui
#ds.ui.dir.lib := $(ds.ui.dir)/out/$V/lib
