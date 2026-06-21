TARGET		:= boot
BUILD		:= build
SOURCES		:= source

all:
	/c/devkitPro/devkitPPC/bin/powerpc-eabi-gcc -g -O2 -DGEKKO -I/c/devkitPro/libogc/include source/main.c -o boot.elf -L/c/devkitPro/libogc/lib/wii -L/c/devkitPro/devkitPPC/powerpc-eabi/lib -lfat -logc -lm -lnetwork -lwiiuse -lbte
	/c/devkitPro/devkitPPC/bin/powerpc-eabi-elf2dol boot.elf boot.dol
	@echo "BUILD COMPLETE!"

clean:
	rm -f boot.dol boot.elf
