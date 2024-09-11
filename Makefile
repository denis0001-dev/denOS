TARGET = i686-elf
PREFIX = ~/opt/cross

CC = $(PREFIX)/bin/$(TARGET)-gcc
AS = $(PREFIX)/bin/$(TARGET)-as
EXTRAPARAMS_CC = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
INDIR = $(shell realpath .)/src/main
OUTDIR = $(shell realpath .)/build

setup:
	echo "Declaring global variables..."
	export TARGET=$(TARGET)
	export PREFIX=$(PREFIX)
	export PATH="$(PREFIX)/bin:$PATH"

clean: clean_all

clean_all:
	rm -rf $(INDIR)/*.o $(OUTDIR)/*

clean_tmp_files:
	rm -rf $(OUTDIR)/boot.o $(OUTDIR)/kernel.o $(OUTDIR)/boot_clean.s $(OUTDIR)/iso/

all: clean setup boot kernel link iso clean_tmp_files

qemu: all
	qemu-system-i386 -cdrom $(OUTDIR)/denOS.iso

boot:
	echo "Compiling boot..."
	sed -E 's/;.*$$//gm;t' < $(INDIR)/boot.s > $(OUTDIR)/boot_clean.s
	$(AS) $(OUTDIR)/boot_clean.s -o $(OUTDIR)/boot.o

kernel:
	echo "Compiling kernel..."
	$(CC) -c $(INDIR)/kernel.c -o $(OUTDIR)/kernel.o $(EXTRAPARAMS_CC)

link:
	echo "Linking..."
	$(CC) -T $(INDIR)/linker.ld -o $(OUTDIR)/denOS.bin -ffreestanding -O2 -nostdlib $(OUTDIR)/boot.o $(OUTDIR)/kernel.o -lgcc

iso:
	mkdir -p $(OUTDIR)/iso/boot/grub
	cp $(OUTDIR)/denOS.bin $(OUTDIR)/iso/boot/denOS.bin
	echo "menuentry \"denOS 1.0\" {\n\tmultiboot /boot/denOS.bin\n}" > $(OUTDIR)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $(OUTDIR)/denOS.iso $(OUTDIR)/iso