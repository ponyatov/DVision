# var
OS = $(shell uname -s)

# tool
CURL = curl -L -o

# src
D += $(wildcard           src/*.d*)
D += $(wildcard nafigator/src/*.d*)

# all
.PHONY: all
all: $(D)
	dub build && dub build :nafigator

.PHONY: run
run: $(D)
	dub run :nafigator

# format
format: tmp/format_c tmp/format_d
tmp/format_c: $(C)
	clang-format -style=file -i $? && touch $@
tmp/format_d: $(D)
	dub run dfmt -- -i $? && touch $@

# doc
.PHONY: doc
doc: \
	doc/Turbo_Vision_Version_2.0_Programming_Guide_1992.pdf

doc/Turbo_Vision_Version_2.0_Programming_Guide_1992.pdf:
	$(CURL) $@ http://bitsavers.informatik.uni-stuttgart.de/pdf/borland/Turbo_Vision_Version_2.0_Programming_Guide_1992.pdf

# install
.PHONY: install update gz
install: doc gz
	$(MAKE) update
update:
	sudo apt update
	sudo apt install -yu `cat apt.$(OS)`

gz:
