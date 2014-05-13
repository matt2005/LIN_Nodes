DIRS		 = $(dir $(wildcard */Makefile))
MAKECMDGOALS	?= build
TARGETS		:= $(foreach dir,$(DIRS),$(addsuffix -$(dir),$(MAKECMDGOALS)))

.PHONY: $(MAKECMDGOALS) $(TARGETS)
$(MAKECMDGOALS): $(TARGETS)
$(TARGETS): $(MAKEFILE_LIST)

$(TARGETS):	goal = $(word 1,$(subst -, ,$@))
$(TARGETS):	dir  = $(word 2,$(subst -, ,$@))
$(TARGETS):
	@echo MAKE $(goal) in $(dir)
	@$(MAKE) -C $(dir) $(goal)
