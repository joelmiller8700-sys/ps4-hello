TOOLCHAIN   := $(OO_PS4_TOOLCHAIN)
PROJDIR     := $(shell pwd)
INTDIR      := $(PROJDIR)/build
INCLUDEDIR  := $(TOOLCHAIN)/include
LIBDIR      := $(TOOLCHAIN)/lib

CC          := clang++
LD          := ld.lld
CDIR        := $(TOOLCHAIN)/bin/linux

CFLAGS      := --target=x86_64-pc-freebsd12-elf -fPIC -funwind-tables -I$(INCLUDEDIR) -I$(INCLUDEDIR)/c++/v1 -O2 -Wall
LDFLAGS     := -m elf_x86_64 -pie --script $(TOOLCHAIN)/link.x --eh-frame-hdr -L$(LIBDIR) -lc -lkernel -lc++ -lScePad -lSceUserService

CFILES      := $(wildcard *.cpp)
OBJS        := $(patsubst %.cpp, $(INTDIR)/%.o, $(CFILES))

TARGET      := eboot.bin

all: $(TARGET)

$(TARGET): $(INTDIR) $(OBJS)
	$(LD) $(INTDIR)/*.o -o $(INTDIR)/homebrew.elf $(LDFLAGS)
	$(CDIR)/create-fself -in=$(INTDIR)/homebrew.elf --out=$(TARGET) --eboot=$(TARGET) --paid 0x3800000000000011

$(INTDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(INTDIR):
	mkdir -p $(INTDIR)

clean:
	rm -rf $(INTDIR) $(TARGET)
