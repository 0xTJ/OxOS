CFLAGS := $(CFLAGS)
ASFLAGS := $(ASFLAGS) -f bin

SRCDIR := src
BINDIR := bin

BINARIES := $(MODULES)
BINARIES := $(BINARIES:%=$(BINDIR)/%)

.PHONY: all clean

all: $(BINARIES)

$(BINDIR)/%: $(SRCDIR)/%.s
	$(if ifndef $(@D), mkdir -p $(@D))
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(BINDIR)
