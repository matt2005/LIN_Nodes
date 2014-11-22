SRCROOT		:= $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
DIRS		 = $(dir $(wildcard $(SRCROOT)/*/Makefile))
MAKECMDGOALS	?= build

PSEUDOGOALS	 = reformat bundle

DIR_GOALS	:= $(filter-out $(PSEUDOGOALS),$(MAKECMDGOALS))
TARGETS		:= $(foreach dir,$(DIRS),$(addsuffix -$(dir),$(DIR_GOALS)))

.NOTPARALLEL:

.PHONY: $(MAKECMDGOALS) $(TARGETS)
$(MAKECMDGOALS): $(TARGETS)
$(TARGETS): $(MAKEFILE_LIST)

$(TARGETS):	goal = $(word 1,$(subst -, ,$@))
$(TARGETS):	dir  = $(word 2,$(subst -, ,$@))
$(TARGETS):
	@echo MAKE $(goal) in $(dir)
	@$(MAKE) -C $(dir) $(goal)

EXTRADIRS	 = $(SRCROOT)/lib/
ALLSRC		 = $(foreach dir,$(DIRS) $(EXTRADIRS),$(wildcard $(dir)*.c $(dir)*.cpp $(dir)*.h))

NO_FORMAT_SRCS	 = %Jzon.cpp %Jzon.h

.PHONY: reformat
reformat:
	@$(SRCROOT)/../etc/reformat.sh $(filter-out $(NO_FORMAT_SRCS),$(ALLSRC))

.PHONY: bundle
bundle:
	@$(SRCROOT)/../etc/mkfw.py --dir $(SRCROOT)/../build --git_identity .. > $(SRCROOT)/../build/nodes.fw
