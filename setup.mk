$(call sm-check-not-empty, ds.ge.dir)
$(call sm-check-target-exists,$(ds.ge.dir))

# Options for ds.ge.base: qt, skia, agg, ds(my own engine base)
ds.ge.base := skia
ds.ge.dir.lib := $(ds.ge.dir)/$(sm.out.lib)
ds.ge.dir.bin := $(ds.ge.dir)/$(sm.out.bin)
