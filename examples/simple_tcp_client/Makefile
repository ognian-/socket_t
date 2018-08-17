mkfile_path := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
SOCKT_BUILD_ROOT := $(shell cd $(shell dirname $(mkfile_path)); pwd)

rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

CC ?= clang
CXX ?= c++

SOCKT_BUILD_PRODUCT := simple_tcp_client
SOCKT_BUILD_CFLAGS := -fPIC -g3
SOCKT_BUILD_CPPFLAGS := -std=c++1z -fexceptions -frtti -Wall -Wextra -pedantic -Werror -Werror=old-style-cast
SOCKT_BUILD_LDFLAGS := -pthread
SOCKT_BUILD_OUTDIR := out

SOCKT_BUILD_HEADERS := $(call rwildcard,,*.h)
SOCKT_BUILD_CSOURCES := $(call rwildcard,,*.c)
SOCKT_BUILD_CPPSOURCES := $(call rwildcard,,*.cpp)
SOCKT_BUILD_OBJECTS := $(patsubst %.c,%.o,$(patsubst %,$(SOCKT_BUILD_OUTDIR)/%,$(SOCKT_BUILD_CSOURCES))) \
	$(patsubst %.cpp,%.o,$(patsubst %,$(SOCKT_BUILD_OUTDIR)/%,$(SOCKT_BUILD_CPPSOURCES)))

SOCKT_BUILD_CFLAGS += -I$(SOCKT_BUILD_ROOT)/../../include \
	-I$(SOCKT_BUILD_ROOT)/../common \
	-I$(SOCKT_BUILD_ROOT)

SOCKT_BUILD_CFLAGS += $(CFLAGS)
SOCKT_BUILD_CPPFLAGS += $(CPPFLAGS)
SOCKT_BUILD_LDFLAGS += $(LDFLAGS)

all: $(SOCKT_BUILD_OUTDIR)/$(SOCKT_BUILD_PRODUCT)
$(SOCKT_BUILD_OUTDIR)/%.o: %.c $(SOCKT_BUILD_HEADERS)
	@mkdir -p $(dir $@)
	@echo Compiling C $<
	$(CC) -c -o $@ $< $(SOCKT_BUILD_CFLAGS)
$(SOCKT_BUILD_OUTDIR)/%.o: %.cpp $(SOCKT_BUILD_HEADERS)
	@mkdir -p $(dir $@)
	@echo Compiling C++ $<
	$(CXX) -c -o $@ $< $(SOCKT_BUILD_CFLAGS) $(SOCKT_BUILD_CPPFLAGS)
$(SOCKT_BUILD_OUTDIR)/$(SOCKT_BUILD_PRODUCT): $(SOCKT_BUILD_OBJECTS)
	@echo Linking $@
	$(CXX) -o $@ $^ $(SOCKT_BUILD_LDFLAGS)
	@echo Success
.PHONY: clean run
clean:
	@rm -rf $(SOCKT_BUILD_OUTDIR)
