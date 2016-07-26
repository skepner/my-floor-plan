# -*- Makefile -*-
# Eugene Skepner 2016

# ----------------------------------------------------------------------

MAKEFLAGS = -w

# ----------------------------------------------------------------------

SOURCES_DIR = src

SOURCES = main.cc room.cc

# ----------------------------------------------------------------------

CLANG = $(shell if g++ --version 2>&1 | grep -i llvm >/dev/null; then echo Y; else echo N; fi)
ifeq ($(CLANG),Y)
  WEVERYTHING = -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded
  WARNINGS = -Wno-weak-vtables # -Wno-padded
  STD = c++14
else
  WEVERYTHING = -Wall -Wextra
  WARNINGS =
  STD = c++14
endif

OPTIMIZATION = # -O3 #-fvisibility=hidden -flto
CXXFLAGS = -MMD -g $(OPTIMIZATION) -fPIC -std=$(STD) $(WEVERYTHING) $(WARNINGS) -I$(BUILD)/include $(PKG_INCLUDES)
LDFLAGS =
LDLIBS = $$(pkg-config --libs cairo)

PKG_INCLUDES = $$(pkg-config --cflags cairo)

# ----------------------------------------------------------------------

BUILD = build
DIST = dist

all: my-floor-plan

-include $(BUILD)/*.d

# ----------------------------------------------------------------------

my-floor-plan: $(DIST)/my-floor-plan
	$(DIST)/my-floor-plan /tmp/my-floor-plan.pdf
	open /tmp/my-floor-plan.pdf

$(DIST)/my-floor-plan: $(patsubst %.cc,$(BUILD)/%.o,$(SOURCES)) | $(DIST)
	g++ $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -rf $(DIST) $(BUILD)/*.o $(BUILD)/*.d

distclean: clean
	rm -rf $(BUILD)

# ----------------------------------------------------------------------

$(BUILD)/%.o: $(SOURCES_DIR)/%.cc | $(BUILD)
	@echo $<
	@g++ $(CXXFLAGS) -c -o $@ $<

# ----------------------------------------------------------------------

$(DIST):
	mkdir -p $(DIST)

$(BUILD):
	mkdir -p $(BUILD)

# ======================================================================
### Local Variables:
### eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
### End:
