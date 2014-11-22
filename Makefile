#
# Top-level makefile
#
# At this level, we perform a completely clean build.
#

TOPDIR		:= $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
BUILDDIR	:= $(TOPDIR)/build

release:
	rm -rf $(BUILDDIR)
	cd common && ./ncf_parser nodes.ncf
	make -C nodes
	make -C lintool	

