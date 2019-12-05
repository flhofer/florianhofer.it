VERSION = 0.1
VERSUFF = "-beta"
GIT_VERSION := "$(shell git describe --abbrev=7 --always --tags)"
CC?=$(CROSS_COMPILE)gcc
AR?=$(CROSS_COMPILE)ar
#uncomment the line below to create debug versions by default
#DEBUG=1
#COVERAGE=1

OBJDIR = build
BINDIR = cgi-bin

sources = menu.c

TARGETS = $(sources:.c=)
LIBS	=  #-ljson-c

CFLAGS ?= -Wall -Wno-nonnull 
CPPFLAGS += -D _GNU_SOURCE -I src/include
#LDFLAGS ?= -pthread 
#LDFLAGS ?= -lcgi -L $(OBJDIR) -pthread 

# If debug is defined, disable optimization level
ifndef DEBUG
	CFLAGS	+= -O2
	CPPFLAGS += -D VERSION=\"$(VERSION)\"
else
	CFLAGS	+= -O0 -g
	CPPFLAGS += -D DEBUG -D VERSION=\"$(VERSION)$(VERSUFF)\ $(GIT_VERSION)\"

	ifdef COVERAGE
		CFLAGS += -coverage
	endif
endif

VPATH	= src/site:
VPATH	+= src/lib:

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) -c $< $(CFLAGS) $(CPPFLAGS) $(EXTRA_LIBS) -o $@

# Pattern rule to generate dependency files from .c files
$(OBJDIR)/%.d: %.c | $(OBJDIR)
	@$(CC) -MM $(CFLAGS) $(CPPFLAGS) $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@ || rm -f $@

.PHONY: all
all: $(TARGETS) 

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Include dependency files, automatically generate them if needed.
-include $(addprefix $(OBJDIR)/,$(sources:.c=.d))

menu: $(addprefix $(OBJDIR)/,menu.o) | $(BINDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BINDIR)/$@.cgi $< $(LIBS)

CLEANUP  = $(TARGETS) *.o .depend *.*~ *.orig *.rej *.d *.a *.gcno *.gcda *.gcov
CLEANUP += $(if $(wildcard .git), ChangeLog)

.PHONY: clean
clean:
	for F in $(CLEANUP); do find -type f -name "$$F" | xargs rm -f; done

.PHONY: rebuild
rebuild:
	$(MAKE) clean
	$(MAKE) all

.PHONY: help
help:
	@echo " C Website useful Makefile targets:"
	@echo ""
	@echo "    all       :  build all tests (default)"
	@echo "    check     :  build and run unit tests
	@echo "    clean     :  remove object files"
	@echo "    help      :  print this message"

.PHONY: tags
tags:
	ctags -R --extra=+f --c-kinds=+p --exclude=tmp --exclude=cgi-bin *

