// ReSharper disable once CppUnusedIncludeDirective
// ReSharper disable CppUnusedIncludeDirective
// ReSharper disable CppStringLiteralToCharPointerConversion
// ReSharper disable CppUseStructuredBinding
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

namespace IO {
	uint8_t inb(uint16_t port) {
		uint8_t ret;
		asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
		return ret;
	}

	void outb(uint16_t port, uint8_t data) {
		asm volatile("outb %0, %1" : "=a"(data) : "d"(port));
	}
}

namespace strings {
	size_t length(const char* str) {
		size_t len = 0;
		while (str[len]) len++;
		return len;
	}

	/* class string {
	private:
		char* data = new char[1];
	public:
		explicit string() {
			data[0] = '\0';
		}

		explicit string(const char data[]) {
			data = new char[strlen(data) + 1];
			for (int i = 0; i < strlen(data); i++) {
				this->data[i] = data[i];
			}
			this->data[strlen(data)] = '\0';
		}

		explicit string(const char data[], const int buffer) {
			data = new char[buffer];
			for (int i = 0; i < strlen(data); i++) {
				this->data[i] = data[i];
			}
		}

		int length() const {
			int len = 0;
			for (int i = 0; true; i++) {
				if (this->data[i] != '\0') {
					len++;
				} else break;
			}
			return len;
		}

		string concat(const string& second) const {
			return this->concat(second.getChars());
		}

		string concat(const char second[]) const {
			const int len1 = length();
			const int len2 = strlen(second);
			const int total = len1 + len2 + 1;
			char new_data[total];

			for (int i = 0; i < len1; i++) {
				new_data[i] = this->getChars()[i];
			}

			for (int i = len1; i < len2; i++) {
				new_data[i + len1] = second[i];
			}

			new_data[len1 + len2] = '\0';
			return string(new_data, total);
		}

		string concat(const char second) const {
			char data[1];
			data[0] = second;
			return concat(data);
		}

		string substring(const int start, const int end) const{
			const int len = end - start;
			char new_data[len + 1];

			for (int i = start; i < end; i++) {
				new_data[i - start] = this->data[i];
			}

			new_data[len] = '\0';
			return string(new_data, len);
		}

		string substring(const int start) const {
			return substring(start, length());
		}

		bool equals(const string other) const {
			const int len1 = other.length();
			if (len1 != length()) return false;
			for (int i = 0; i < len1; i++) {
				if (this->data[i] != other.data[i]) return false;
			}
			return true;
		}

		bool equals(const char* other) const {
			return equals(string(other));
		}

		static bool equals(const string first, const string second) {
			return first.equals(second);
		}

		static bool equals(const string first, const char* second) {
			return first.equals(second);
		}

		static bool equals(const char* first, const char* second) {
			return string(first).equals(second);
		}

		static string concat(const string& first, const string& second) {
			return first.concat(second);
		}

		static string concat(const char first[], const char second[]) {
			return string(first).concat(second);
		}

		static string concat(const string& first, const char second) {
			return first.concat(second);
		}

		static string concat(const char first[], const char second) {
			return string(first).concat(second);
		}

		char* getChars() const {
			char chars[length()];
			for (int i = 0; i < length(); i++) {
				chars[i] = this->data[i];
			}
			return chars;
		}
	}; *


	/* struct string {
		char* data = new char[1];

		string() {
			data[0] = '\0';
		}

		explicit string(const char data[]) {
			data = new char[strlen(data) + 1];
			for (int i = 0; i < strlen(data); i++) {
				this->data[i] = data[i];
			}
			this->data[strlen(data)] = '\0';
		}

		string(const char data[], const int buffer) {
			data = new char[buffer];
			for (int i = 0; i < strlen(data); i++) {
				this->data[i] = data[i];
			}
		}

		int length() const {
			int len = 0;
			for (int i = 0; true; i++) {
				if (this->data[i] != '\0') {
					len++;
				} else break;
			}
			return len;
		}

		string concat(const string& second) const {
			return this->concat(second.getChars());
		}

		string concat(const char second[]) const {
			const int len1 = length();
			const int len2 = strlen(second);
			const int total = len1 + len2 + 1;
			char new_data[total];

			for (int i = 0; i < len1; i++) {
				new_data[i] = this->getChars()[i];
			}

			for (int i = len1; i < len2; i++) {
				new_data[i + len1] = second[i];
			}

			new_data[len1 + len2] = '\0';
			return string(new_data, total);
		}

		string concat(const char second) const {
			char data[1];
			data[0] = second;
			return concat(data);
		}

		string substring(const int start, const int end) const{
			const int len = end - start;
			char new_data[len + 1];

			for (int i = start; i < end; i++) {
				new_data[i - start] = this->data[i];
			}

			new_data[len] = '\0';
			return string(new_data, len);
		}

		string substring(const int start) const {
			return substring(start, length());
		}
	}; */

	char *concat(const char* first, const char* second) {
		const int len1 = length(first);
		const int len2 = length(second);
		const int total = len1 + len2 + 1;
		char new_data[total];

		for (int i = 0; i < len1; i++) {
			new_data[i] = first[i];
		}

		for (int i = len1; i < len2; i++) {
			new_data[i + len1] = second[i];
		}

		new_data[len1 + len2] = '\0';
		return new_data;
	}

	char *concat(const char* first, const char second) {
		char data[2];
		data[0] = second;
		data[1] = '\0';
		return concat(first, data);
	}

	bool equals(const char* first, const char* second) {
		const int len1 = length(first);
		if (len1 != length(second)) return false;
		for (int i = 0; i < len1; i++) {
			if (first[i] != second[i]) return false;
		}
		return true;
	}

	char *substring(const char* str, const int start, const int end) {
		const int len = end - start;
		char new_data[len + 1];

		for (int i = start; i < end; i++) {
			new_data[i - start] = str[i];
		}

		new_data[len] = '\0';
		return new_data;
	}

	char *substring(const char* str, const int start) {
		return substring(str, start, length(str));
	}

	char* int_to_string(int num) {
		static char str[11]; // Assuming the maximum number to be converted is 2147483647
		int i = 0;
		bool negative = false;

		// Handle negative numbers
		if (num < 0) {
			negative = true;
			num = -num;
		}

		// Convert the integer to a string
		while (num > 0) {
			const int digit = num % 10;
			str[i++] = digit + '0';  // Convert the digit to ASCII
			num /= 10;
		}

		// Add a null terminator to the end of the string
		str[i] = '\0';

		// If the number was negative, add a negative sign to the string
		if (negative) {
			str[0] = '-';
		}

		// Reverse the string, since we added digits from left to right
		int j = 0;
		int k = i - 1;
		while (j < k) {
			const char temp = str[j];
			str[j] = str[k];
			str[k] = temp;
			j++;
			k--;
		}

		return &str[i - negative];
	}
}

namespace video {
	enum vga_color {
		BLACK = 0,
		BLUE = 1,
		GREEN = 2,
		CYAN = 3,
		RED = 4,
		MAGENTA = 5,
		BROWN = 6,
		LIGHT_GREY = 7,
		DARK_GREY = 8,
		LIGHT_BLUE = 9,
		LIGHT_GREEN = 10,
		LIGHT_CYAN = 11,
		LIGHT_RED = 12,
		LIGHT_MAGENTA = 13,
		LIGHT_BROWN = 14,
		WHITE = 15,
	};

	static constexpr size_t VGA_WIDTH = 80;
	static constexpr size_t VGA_HEIGHT = 25;

	static vga_color DEFAULT_BG = BLACK;
	static vga_color DEFAULT_FG = LIGHT_GREY;

	size_t terminal_row;
	size_t terminal_column;
	uint8_t terminal_color;
	uint16_t* terminal_buffer;

	static uint8_t vga_entry_color(const vga_color fg, const vga_color bg) {
		return fg | bg << 4;
	}

	static uint16_t vga_entry(const uint16_t uc, const uint8_t color) {
		return static_cast<uint16_t>(uc) | static_cast<uint16_t>(color) << 8;
	}

	struct VGAEntry {
		uint16_t uc;
		uint8_t color;
	};

	VGAEntry vgaEntryFromResult(const uint16_t result) {
		// Получаем младший байт результата для цвета
		VGAEntry entry = {};

		entry.color = static_cast<uint8_t>(result & 0xFF);

		// Получаем старший байт результата для uc
		entry.uc = static_cast<uint16_t>(result >> 8 & 0xFFFF);
		return entry;
	}
}

namespace terminal {
	void clear() {
		video::terminal_row = 0;
		video::terminal_column = 0;
		for (size_t y = 0; y < video::VGA_HEIGHT; y++) {
			for (size_t x = 0; x < video::VGA_WIDTH; x++) {
				const size_t index = y * video::VGA_WIDTH + x;
				video::terminal_buffer[index] = video::vga_entry(' ', video::terminal_color);
			}
		}
	}

	void initialize() {
		video::terminal_row = 0;
		video::terminal_column = 0;
		video::terminal_color = video::vga_entry_color(video::DEFAULT_FG, video::DEFAULT_BG);
		video::terminal_buffer = reinterpret_cast<uint16_t *>(0xB8000);
		clear();
	}

	void putentryat(const char c, const uint8_t color, const size_t x, const size_t y) {
		const size_t index = y * video::VGA_WIDTH + x;
		video::terminal_buffer[index] = video::vga_entry(c, color);
	}

	void scroll(const int lines) {
		const size_t new_row = (video::terminal_row + lines) % video::VGA_HEIGHT;

		for (size_t y = new_row; y < video::VGA_HEIGHT; y++) {
			for (size_t x = 0; x < video::VGA_WIDTH; x++) {
				const size_t index = y * video::VGA_WIDTH + x;
				video::terminal_buffer[index] = video::terminal_buffer[(y + lines) * video::VGA_WIDTH + x];
			}
		}

		for (size_t y = 0; y < lines; y++) {
			for (size_t x = 0; x < video::VGA_WIDTH; x++) {
				const size_t index = y * video::VGA_WIDTH + x;
				video::terminal_buffer[index] = video::vga_entry(' ', video::terminal_color);
			}
		}
	}

	void printChar(const char c) {
		if (c != '\n') {
			putentryat(c, video::terminal_color, video::terminal_column, video::terminal_row);
			if (++video::terminal_column == video::VGA_WIDTH) {
				video::terminal_column = 0;
				if (++video::terminal_row == video::VGA_HEIGHT) {
					scroll(1);
					--video::terminal_row;
				}
			}
		} else {
			if (++video::terminal_row == video::VGA_HEIGHT) {
				scroll(1);
				--video::terminal_row;
			}
			video::terminal_column = 0;
		}
	}

	void print(const char* data) {
		for (size_t i = 0; i < strings::length(data); i++) printChar(data[i]);
	}

	void printColor(const char *data, const video::vga_color bg, const video::vga_color fg) {
		video::terminal_color = vga_entry_color(fg, bg);
		print(data);
		video::terminal_color = vga_entry_color(video::DEFAULT_FG, video::DEFAULT_BG);
	}

	void println(const char* data) {
		print(data);
		print("\n");
	}

	void printlnColor(const char *data, const video::vga_color bg, const video::vga_color fg) {
		printColor(data, bg, fg);
		print("\n");
	}

	struct pos {
		size_t row;
		size_t col;
	};

	struct current_input {
		size_t size;
		pos start;
		pos end;
		char buffer[video::VGA_WIDTH];
	};

	constexpr current_input default_input = {0, {0, 0}, {0, 0}, ""};

	current_input input = default_input;

	void enableCursor(const uint8_t cursor_start, const uint8_t cursor_end) {
		IO::outb(0x3D4, 0x0A);
		IO::outb(0x3D5, IO::inb(0x3D5) & 0xC0 | cursor_start);

		IO::outb(0x3D4, 0x0B);
		IO::outb(0x3D5, IO::inb(0x3D5) & 0xE0 | cursor_end);
	}

	void disableCursor() {
		IO::outb(0x3D4, 0x0A);
		IO::outb(0x3D5, 0x20);
	}

	void updateCursor(const size_t x, const size_t y) {
		const uint16_t pos = y * video::VGA_WIDTH + x;

		IO::outb(0x3D4, 0x0F);
		IO::outb(0x3D5, static_cast<uint8_t>(pos & 0xFF));
		IO::outb(0x3D4, 0x0E);
		IO::outb(0x3D5, static_cast<uint8_t>(pos >> 8 & 0xFF));
	}

	pos getCursorPosition() {
		uint16_t pos = 0;
		IO::outb(0x3D4, 0x0F);
		pos |= IO::inb(0x3D5);
		IO::outb(0x3D4, 0x0E);
		pos |= static_cast<uint16_t>(IO::inb(0x3D5)) << 8;

		const size_t x = pos % video::VGA_WIDTH;
		const size_t y = pos / video::VGA_WIDTH;
		return {x, y};
	}
}

namespace keyboard {
	constexpr int PORT = 0x60;
	constexpr int INPUT_DELAY = 500000000;
	enum keys {
		KEY_A = 0x1E,
		KEY_B = 0x30,
		KEY_C = 0x2E,
		KEY_D = 0x20,
		KEY_E = 0x12,
		KEY_F = 0x21,
		KEY_G = 0x22,
		KEY_H = 0x23,
		KEY_I = 0x17,
		KEY_J = 0x24,
		KEY_K = 0x25,
		KEY_L = 0x26,
		KEY_M = 0x32,
		KEY_N = 0x31,
		KEY_O = 0x18,
		KEY_P = 0x19,
		KEY_Q = 0x10,
		KEY_R = 0x13,
		KEY_S = 0x1F,
		KEY_T = 0x14,
		KEY_U = 0x16,
		KEY_V = 0x2F,
		KEY_W = 0x11,
		KEY_X = 0x2D,
		KEY_Y = 0x15,
		KEY_Z = 0x2C,
		KEY_1 = 0x02,
		KEY_2 = 0x03,
		KEY_3 = 0x04,
		KEY_4 = 0x05,
		KEY_5 = 0x06,
		KEY_6 = 0x07,
		KEY_7 = 0x08,
		KEY_8 = 0x09,
		KEY_9 = 0x0A,
		KEY_0 = 0x0B,
		KEY_MINUS = 0x0C,
		KEY_EQUAL = 0x0D,
		KEY_SQUARE_OPEN_BRACKET = 0x1A,
		KEY_SQUARE_CLOSE_BRACKET = 0x1B,
		KEY_SEMICOLON = 0x27,
		KEY_BACKSLASH = 0x2B,
		KEY_COMMA = 0x33,
		KEY_DOT = 0x34,
		KEY_FORESLHASH = 0x35,
		KEY_F1 = 0x3B,
		KEY_F2 = 0x3C,
		KEY_F3 = 0x3D,
		KEY_F4 = 0x3E,
		KEY_F5 = 0x3F,
		KEY_F6 = 0x40,
		KEY_F7 = 0x41,
		KEY_F8 = 0x42,
		KEY_F9 = 0x43,
		KEY_F10 = 0x44,
		KEY_F11 = 0x85,
		KEY_F12 = 0x86,
		KEY_BACKSPACE = 0x0E,
		KEY_DELETE = 0x53,
		KEY_DOWN = 0x50,
		KEY_END = 0x4F,
		KEY_ENTER = 0x1C,
		KEY_ESC = 0x01,
		KEY_HOME = 0x47,
		KEY_INSERT = 0x52,
		KEY_KEYPAD_5 = 0x4C,
		KEY_KEYPAD_MUL = 0x37,
		KEY_KEYPAD_Minus = 0x4A,
		KEY_KEYPAD_PLUS = 0x4E,
		KEY_KEYPAD_DIV = 0x35,
		KEY_LEFT = 0x4B,
		KEY_PAGE_DOWN = 0x51,
		KEY_PAGE_UP = 0x49,
		KEY_PRINT_SCREEN = 0x37,
		KEY_RIGHT = 0x4D,
		KEY_SPACE = 0x39,
		KEY_TAB = 0x0F,
		KEY_UP = 0x48,
	};

	char get_input_keycode() {
		char ch = 0;
		while((ch = IO::inb(PORT)) != 0) { // NOLINT(*-narrowing-conversions)
			if (ch > 0) return ch;
		}
		return ch;
	}

	char get_ascii_char(const char keycode) {
		switch(keycode) {
			// Letters
			case KEY_A: return 'a';
			case KEY_B: return 'b';
			case KEY_C: return 'c';
			case KEY_D: return 'd';
			case KEY_E: return 'e';
			case KEY_F: return 'f';
			case KEY_G: return 'g';
			case KEY_H: return 'h';
			case KEY_I: return 'i';
			case KEY_J: return 'j';
			case KEY_K: return 'k';
			case KEY_L: return 'l';
			case KEY_M: return 'm';
			case KEY_N: return 'n';
			case KEY_O: return 'o';
			case KEY_P: return 'p';
			case KEY_Q: return 'q';
			case KEY_R: return 'r';
			case KEY_S: return 's';
			case KEY_T: return 't';
			case KEY_U: return 'u';
			case KEY_V: return 'v';
			case KEY_W: return 'w';
			case KEY_X: return 'x';
			case KEY_Y: return 'y';
			case KEY_Z: return 'z';
			// Numbers
			case KEY_0: return '0';
			case KEY_1: return '1';
			case KEY_2: return '2';
			case KEY_3: return '3';
			case KEY_4: return '4';
			case KEY_5: return '5';
			case KEY_6: return '6';
			case KEY_7: return '7';
			case KEY_8: return '8';
			case KEY_9: return '9';
			// Special characters
			case KEY_MINUS: return '-';
			case KEY_EQUAL: return '=';
			case KEY_SQUARE_OPEN_BRACKET: return '[';
			case KEY_SQUARE_CLOSE_BRACKET: return ']';
			case KEY_SEMICOLON: return ';';
			case KEY_BACKSLASH: return '\\';
			case KEY_COMMA: return ',';
			case KEY_DOT: return '.';
			case KEY_FORESLHASH: return '*';
			// F keys
			case KEY_F1: return '1';
			case KEY_F2: return '2';
			case KEY_F3: return '3';
			case KEY_F4: return '4';
			case KEY_F5: return '5';
			case KEY_F6: return '6';
			case KEY_F7: return '7';
			case KEY_F8: return '8';
			case KEY_F9: return '9';
			case KEY_F10: return '0';
			case KEY_F11: return '1';
			case KEY_F12: return '2';
			// Arrow keys
			case KEY_UP: return '\101';
			case KEY_LEFT: return '\102';
			case KEY_DOWN: return '\103';
			case KEY_RIGHT: return '\104';
			// Text management
			case KEY_ENTER: return '\n';
			case KEY_BACKSPACE: return '\b';
			case KEY_DELETE: return '\0';
			case KEY_INSERT: return '\50';
			case KEY_SPACE: return ' ';
			case KEY_TAB: return '\t';

			// Navigation
			case KEY_END: return '\200';
			case KEY_ESC: return '\300';
			case KEY_HOME: return '\40';
			case KEY_PAGE_DOWN: return '\62';
			case KEY_PAGE_UP: return '\61';
			case KEY_PRINT_SCREEN: return '\70';

			default: return 0;
		}
	}
}

namespace system {
	auto VERSION = "0.7.1";

	void sleep(uint32_t timer_count) {
		for (uint32_t i = 0; i < timer_count; i++) {
            asm volatile("nop");
        }
	}
}

void inputLoop() {
	char ch = 0;
	char keycode = 0;
	int timer_count = keyboard::INPUT_DELAY;
	terminal::input.buffer[0] = '\0';
	while (true) {
		asm volatile("nop"); // A little bit of delay
		if (timer_count-- <= 0) {
			keycode = keyboard::get_input_keycode();
			// Newline
			if (keycode == keyboard::KEY_ENTER) {
				terminal::print("\n");

				// Commands logic
				if (strings::equals(terminal::input.buffer, "shutdown")) {
					return; // Shut down the kernel
				} else if (strings::equals(terminal::input.buffer, "help")) {
					terminal::println("Available commands: ");
					terminal::println("shutdown - Shut down the kernel.");
					terminal::println("help - Display this help message.");
					terminal::println("version - Display the version.");
				} else if (strings::equals(terminal::input.buffer, "version")) {
					terminal::printlnColor("denOS", video::DARK_GREY, video::WHITE);
					terminal::println("");
					terminal::print("Kernel version: ");
					terminal::printlnColor(system::VERSION, video::DEFAULT_BG, video::LIGHT_GREEN);
					terminal::println("Compiled using the C++ and asm cross-compiler for i686-elf.");
					terminal::println("Type 'help' for more information.");
				}
				// No valid command is entered
				else if (!strings::equals(terminal::input.buffer, "")) {
					terminal::print("Unknown command \"");
					terminal::print(terminal::input.buffer);
					terminal::println("\". Type 'help' for more information.");
				}

				terminal::print("> ");
				for (int i = 0; i < sizeof(terminal::input.buffer) / sizeof(terminal::input.buffer[0]); i++) {
					terminal::input.buffer[i] = '\0';
				}
				terminal::input.start.row = video::terminal_row;
				terminal::input.start.col = video::terminal_column - 1;
				terminal::input.end.row = video::terminal_row;
				terminal::input.end.col = video::terminal_column;
			}
			// Erase
			else if (keycode == keyboard::KEY_BACKSPACE) {
				if (terminal::input.end.col != terminal::input.start.col + 1) {
					const size_t index = terminal::input.end.row * video::VGA_WIDTH + terminal::input.end.col - 1;
					video::terminal_buffer[index] = video::vga_entry(' ', video::terminal_color);
					video::terminal_column--;
					terminal::input.end.col--;
					terminal::input.buffer[strings::length(terminal::input.buffer) - 1] = '\0';
				}
			}
			// Type character
			else if (keycode != 0) {
				ch = keyboard::get_ascii_char(keycode);
				if (terminal::input.start.col == 0 &&
					terminal::input.start.row == 0 &&
					terminal::input.end.col == 0 &&
					terminal::input.end.row == 0
					) {
					const terminal::pos pos = {video::terminal_row, video::terminal_column - 1};
					terminal::input.start = pos;
					const terminal::pos end = {video::terminal_row, video::terminal_column};
					terminal::input.end = end;
					}
				terminal::printChar(ch);
				terminal::input.buffer[strings::length(terminal::input.buffer)] = ch;
				terminal::input.buffer[strings::length(terminal::input.buffer) + 1] = '\0';
				terminal::input.end.row = terminal::input.start.row;
				terminal::input.end.col++;
			}
			timer_count = keyboard::INPUT_DELAY;
		}
	}
}

// ENTRY POINT
extern "C" void kernel_main(void) {
	/* Initialize terminal interface */
	terminal::initialize();
	constexpr video::vga_color bg = video::DARK_GREY;

	terminal::printColor("denOS ", bg, video::WHITE);
	terminal::printColor(system::VERSION, bg, video::LIGHT_GREEN);
	terminal::printlnColor(" kernel", bg, video::WHITE);
	terminal::println("");
	terminal::printColor("Features:", video::DEFAULT_BG, video::LIGHT_CYAN);
	terminal::println(" text on the screen, newlines, colors, keyboard (!!!), commands");
	terminal::println("Reset your computer to exit.");
	terminal::println("");
	terminal::printlnColor("WARNING: Doesn't work on x64.", video::DEFAULT_BG, video::LIGHT_RED);
	terminal::println("");
	terminal::print("> ");
	terminal::print(strings::int_to_string(5));
	// system::sleep(1000);
	/* update_cursor(1, 4);
	disable_cursor(); */
	inputLoop();
	terminal::println("System halted.");
}