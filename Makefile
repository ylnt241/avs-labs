CC = gcc
CFLAGS = -std=c89 -g -pedantic -Wall -Wextra

SRCDIR = src
BINDIR = bin

LABS = $(wildcard $(SRCDIR)/lab-*)

.PHONY: all clean
all: $(patsubst $(SRCDIR)/%, $(BINDIR)/%, $(LABS))

$(BINDIR):
	mkdir -p $@

define LAB_rule
$(BINDIR)/$(notdir $(1)): $(wildcard $(1)/*.c) | $(BINDIR)
	$$(CC) $$(CFLAGS) -o $$@ $$^
endef

$(foreach lab,$(LABS),$(eval $(call LAB_rule,$(lab))))

clean:
	rm -rf $(BINDIR)
