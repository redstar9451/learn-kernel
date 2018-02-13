
# wildcard */ lists all the directories
# wildcard * lists all the files and directories 
dirs := $(wildcard */ )

.PHONY: all
all: $(dirs)
	echo sucessfully compile sample modules

.PHONY: $(dirs)
$(dirs):
	make -C $@
