 TARGET		:= boot
BUILD		:= build
SOURCES		:= source

all:
	powerpc-eabi-gcc -g -O2 -DGEKKO -I/opt/devkitpro/libogc/include $(SOURCES)/main.c -o boot.elf -L/opt/devkitpro/libogc/lib/wii -L/opt/devkitpro/devkitPPC/powerpc-eabi/lib -lfat -logc -lm -lnetwork -lwiiuse -lbte
	powerpc-eabi-elf2dol boot.elf boot.dol
	@echo "BUILD COMPLETE!"

clean:
	rm -f boot.dol boot.elf
