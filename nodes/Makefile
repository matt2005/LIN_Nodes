SRCROOT		:= $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
#DIRS		 = $(dir $(wildcard $(SRCROOT)/*/Makefile))
DIRS		 = master
MAKECMDGOALS	?= build

DIR_GOALS	:= $(filter-out reformat,$(MAKECMDGOALS))
TARGETS		:= $(foreach dir,$(DIRS),$(addsuffix -$(dir),$(DIR_GOALS)))

.PHONY: $(MAKECMDGOALS) $(TARGETS)
$(MAKECMDGOALS): $(TARGETS)
$(TARGETS): $(MAKEFILE_LIST)

$(TARGETS):	goal = $(word 1,$(subst -, ,$@))
$(TARGETS):	dir  = $(word 2,$(subst -, ,$@))
$(TARGETS):
	@echo MAKE $(goal) in $(dir)
	@$(MAKE) -C $(dir) $(goal)

ALLSRC		 = $(foreach dir,$(DIRS),$(wildcard $(dir)*.c $(dir)*.cpp $(dir)*.h))

.PHONY: reformat
reformat:
	@$(SRCROOT)/../etc/reformat.sh $(ALLSRC)
