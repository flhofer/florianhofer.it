VERSION = 0.2
GIT_VERSION := "$(shell git describe --abbrev=7 --always --tags)"
CC?=$(CROSS_COMPILE)gcc
AR?=$(CROSS_COMPILE)ar
#uncomment the line below to create debug versions by default
DEBUG=1

OBJDIR = build
BINDIR = cgi-bin

sources = menu.c \
		  display.c

TARGETS = $(sources:.c=)
LIBS	= -lmycgi #-lsqlite3 #-ljson-c

CFLAGS ?= -Wall -Wno-nonnull -march=native
CPPFLAGS += -D _GNU_SOURCE -I src/include
#LDFLAGS ?= -pthread 
LDFLAGS ?= -lmycgi -L $(OBJDIR) -pthread 

# If debug is defined, disable optimization level
ifndef DEBUG
	CFLAGS	+= -O2
	CPPFLAGS += -D VERSION=\"$(VERSION)\"
else
	CFLAGS	+= -O0 -g
	CPPFLAGS += -D DEBUG -D VERSION=\"$(VERSION)\ Build\ $(GIT_VERSION)\"
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
	chmod 755 www/cgi-bin/*.cgi
	#chown 1000:1000 www/cgi-bin/*.cgi commented for CI

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p www/$(BINDIR)

# Include dependency files, automatically generate them if needed.
-include $(addprefix $(OBJDIR)/,$(sources:.c=.d))

#temp off, linked version does not work
menu: $(addprefix $(OBJDIR)/,menu.o libmycgi.a) | $(BINDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BINDIR)/$@.cgi $< $(LIBS)

display: $(addprefix $(OBJDIR)/,display.o libmycgi.a) | $(BINDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BINDIR)/$@.cgi $< $(LIBS)

# lib containing include lib in one binary file
LIBOBJS =$(addprefix $(OBJDIR)/,decorator.o files.o)
$(OBJDIR)/libmycgi.a: $(LIBOBJS)
	$(AR) rcs $@ $^

CLEANUP  = $(TARGETS) *.o .depend *.*~ *.orig *.rej *.d *.a *.gcno *.gcda *.gcov *.cgi
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

