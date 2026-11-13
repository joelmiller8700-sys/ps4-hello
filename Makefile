CC      := clang++
LD      := ld.lld
TARGET  := eboot.bin
ODIR    := build

CFLAGS  := --target=x86_64-pc-freebsd12-elf -fPIC -funwind-tables -I$(OO_PS4_TOOLCHAIN)/include -I$(OO_PS4_TOOLCHAIN)/include/c++/v1 -nostdinc -nostdinc++ -O2
LDFLAGS := -m elf_x86_64 -pie --script $(OO_PS4_TOOLCHAIN)/link.x --eh-frame-hdr -L$(OO_PS4_TOOLCHAIN)/lib -lc -lkernel -lc++ -lScePad -lSceUserService

all: $(TARGET)

$(TARGET): $(ODIR)/main.o
	$(LD) $(ODIR)/main.o -o $(ODIR)/homebrew.elf $(LDFLAGS)
	$(OO_PS4_TOOLCHAIN)/bin/linux/create-fself -in=$(ODIR)/homebrew.elf --out=$(TARGET) --eboot=$(TARGET) --paid 0x3800000000000011

$(ODIR)/main.o: main.cpp | $(ODIR)
	$(CC) $(CFLAGS) -c main.cpp -o $(ODIR)/main.o

$(ODIR):
	mkdir -p $(ODIR)
