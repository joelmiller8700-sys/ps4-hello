TARGET := hello
CFLAGS := --target=x86_64-pc-freebsd12-elf -fPIC -funwind-tables -fuse-init-array -isysroot $(OO_PS4_TOOLCHAIN) -isystem $(OO_PS4_TOOLCHAIN)/include
LDFLAGS := -m elf_x86_64 -pie --script $(OO_PS4_TOOLCHAIN)/link.x --eh-frame-hdr -L$(OO_PS4_TOOLCHAIN)/lib
LIBS := -lkernel -lc

all:
	clang $(CFLAGS) -c -o main.o main.c
	ld.lld $(LDFLAGS) main.o -o $(TARGET).elf $(LIBS) $(OO_PS4_TOOLCHAIN)/lib/crt1.o
