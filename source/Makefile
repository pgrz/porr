SUBDIRS := $(wildcard */.)

.PHONY: $(SUBDIRS)
.SILENT:

all: TARGET=all
clean: TARGET=clean
clobber: TARGET=clobber
install: TARGET=install
uninstall: TARGET=uninstall

all clean clobber install uninstall : $(SUBDIRS)

$(SUBDIRS):
	@exec $(MAKE) -C $@ $(TARGET)
