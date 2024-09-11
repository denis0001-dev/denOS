#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static uint8_t vga_entry_color(const enum vga_color fg, const enum vga_color bg) {
	return fg | bg << 4;
}

static uint16_t vga_entry(const unsigned char uc, const uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len]) len++;
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static const size_t DEFAULT_BG = VGA_COLOR_BLACK;
static const size_t DEFAULT_FG = VGA_COLOR_LIGHT_GREY;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(DEFAULT_FG, DEFAULT_BG);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_putentryat(const char c, const uint8_t color, const size_t x, const size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void print(const char* data) {
	for (size_t i = 0; i < strlen(data); i++)
		if (data[i] != '\n') {
			terminal_putentryat(data[i], terminal_color, terminal_column, terminal_row);
			if (++terminal_column == VGA_WIDTH) {
				terminal_column = 0;
				if (++terminal_row == VGA_HEIGHT)
					terminal_row = 0;
			}
		} else {
			if (++terminal_row == VGA_HEIGHT) terminal_row = 0;
			terminal_column = 0;
		}
}

void printColor(const char* data, const size_t bg, const size_t fg) {
	terminal_color = vga_entry_color(fg, bg);
	print(data);
	terminal_color = vga_entry_color(DEFAULT_FG, DEFAULT_BG);
}

void println(const char* data) {
	print(data);
	print("\n");
}

void printlnColor(const char* data, const size_t bg, const size_t fg) {
	printColor(data, bg, fg);
	print("\n");
}

// ENTRY POINT
void kernel_main(void) {
	/* Initialize terminal interface */
	terminal_initialize();

	const size_t bg = VGA_COLOR_DARK_GREY;

	printColor("denOS ", bg, VGA_COLOR_WHITE);
	printColor("0.4", bg, VGA_COLOR_LIGHT_GREEN);
	printlnColor(" kernel", bg, VGA_COLOR_WHITE);
	println("");
	printColor("Features:", DEFAULT_BG, VGA_COLOR_LIGHT_CYAN);
	println(" text on the screen, newlines, colors.");
	println("Soon there will be keyboard input.");
	println("Reset your computer to exit.");
	println("");
	printlnColor("WARNING: Doesn't work on x64.", DEFAULT_BG, VGA_COLOR_LIGHT_RED);
	println("This text is incredibly long because i'm testing word wrapping that's not implemented yet...");
}