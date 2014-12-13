#
# Top-level makefile
#
# At this level, we perform a completely clean build.
#

TOPDIR		:= $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
BUILDDIR	:= $(TOPDIR)/build

.PHONY: release
release:
	rm -rf $(BUILDDIR)
	cd common && ./ncf_parser nodes.ncf
	make -C nodes
	make -C lintool	

.PHONY: program
program: release
	build/lintool/lintool update build/*/*.fw
