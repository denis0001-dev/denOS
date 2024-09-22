/**
 * denOS kernel
 * Created by denis0001-dev on https://gitverse.ru/denis0001-dev/denOS/content/master/src/main/kernel.cpp
 * Version 0.7.7
 * Compiling, linking, and building commands from https://wiki.osdev.org/Bare_Bones
 * DO NOT EDIT OR REMOVE THIS HEADER.
 */

// ReSharper disable CppUnusedIncludeDirective
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

/* This kernel will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

typedef const char* string;

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
	size_t length(const string str) {
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

	string concat(const string first, const string second) {
		const size_t len1 = length(first);
		const size_t len2 = length(second);
		const size_t total = len1 + len2 + 1;
		char new_data[total];

		for (size_t i = 0; i < len1; i++) {
			new_data[i] = first[i];
		}

		for (size_t i = len1; i < len2; i++) {
			new_data[i + len1] = second[i];
		}

		new_data[len1 + len2] = '\0';
		return new_data;
	}

	string concat(const string first, const char second) {
		char data[2];
		data[0] = second;
		data[1] = '\0';
		return concat(first, data);
	}

	bool equals(const string first, const string second) {
		const size_t len1 = length(first);
		if (len1 != length(second)) return false;
		for (size_t i = 0; i < len1; i++) {
			if (first[i] != second[i]) return false;
		}
		return true;
	}

	string substring(const string str, const size_t start, const size_t end) {
		const size_t len = end - start;
		char new_data[len + 1];

		for (size_t i = start; i < end; i++) {
			new_data[i - start] = str[i];
		}

		new_data[len] = '\0';
		return new_data;
	}

	string substring(const string str, const size_t start) {
		return substring(str, start, length(str));
	}

	string reverse(const string str) {
		const size_t length = strings::length(str);
		const auto temp = new char[length];

		for (size_t i = 0; i < length; i++) {
			temp[i] = str[length - i - 1];
		}
		return temp;
	}

	string toString(int num) {
		char buffer[10];
		int i = 0;

		while (num > 0) {
			buffer[i] = num % 10 + '0';
			num /= 10;
			i++;
		}
		buffer[i] = '\0';
		return reverse(buffer);
	}

	string toLowerCase(const string str) {
		const size_t len = length(str);
        char new_data[len + 1];

        for (size_t i = 0; i < len; i++) {
	        const char c = str[i];
            if (c >= 'A' && c <= 'Z') {
                new_data[i] = c + ('a' - 'A');
            } else {
                new_data[i] = c;
            }
        }

        new_data[len] = '\0';
        return new_data;
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
		video::terminal_color = vga_entry_color(video::DEFAULT_FG, video::DEFAULT_BG);
		video::terminal_buffer = reinterpret_cast<uint16_t *>(0xB8000);
		clear();
	}

	void putentryat(const char c, const uint8_t color, const size_t x, const size_t y) {
		const size_t index = y * video::VGA_WIDTH + x;
		video::terminal_buffer[index] = video::vga_entry(c, color);
	}

	void scroll(const size_t lines) {
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
			putentryat(' ', video::terminal_color, video::terminal_column, video::terminal_row);
			if (++video::terminal_row == video::VGA_HEIGHT) {
				scroll(1);
				--video::terminal_row;
			}
			video::terminal_column = 0;
		}
		putentryat(' ', vga_entry_color(video::BLACK, video::WHITE), video::terminal_column, video::terminal_row);
	}

	void print(const string data) {
		for (size_t i = 0; i < strings::length(data); i++) printChar(data[i]);
	}

	void printColor(const string data, const video::vga_color bg, const video::vga_color fg) {
		video::terminal_color = vga_entry_color(fg, bg);
		print(data);
		video::terminal_color = vga_entry_color(video::DEFAULT_FG, video::DEFAULT_BG);
	}
	void printColor(const char data, const video::vga_color bg, const video::vga_color fg) {
		char str[2];
		str[0] = data;
		str[1] = '\0';
		printColor(str, bg, fg);
	}

	void println(const string data) {
		print(data);
		print("\n");
	}

	void printlnColor(const string data, const video::vga_color bg, const video::vga_color fg) {
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

	void move_cursor(const size_t x, const size_t y) {
	    // The cursor position is stored in the first two bytes of the video memory.
	    // The first byte is the high nibble and the second byte is the low nibble.
	    // The high nibble is the page number and the low nibble is the row number.
	    // The page number is the y coordinate divided by 80 and the row number is the x coordinate.

	    // Calculate the high nibble (page number)
	    const uint8_t high_nibble = y / 80;

	    // Calculate the low nibble (row number)
	    const uint8_t low_nibble = x;

	    // Write the high nibble to the first byte of the video memory
	    IO::outb(0x3D4, high_nibble);

	    // Write the low nibble to the second byte of the video memory
	    IO::outb(0x3D5, low_nibble);
	}
}

namespace console {
	struct help_entry {
		string command;
		string description;
	};
	constexpr help_entry HELP_TABLE[] = {
		{"shutdown", "Shut down the system"},
		{"help", "Display this help message"},
		{"version", "Display version information"},
		{"changelog", "Display changes recently made"}
	};
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
	enum key_chars {
		// Arrow keys
		UP = '\101',
		LEFT = '\102',
		DOWN = '\103',
		RIGHT = '\104',
		// Text management
		ENTER = '\n',
		BACKSPACE = '\b',
		DELETE = '\0',
		INSERT = '\50',
		SPACE = ' ',
		TAB = '\t',

		// Navigation
		END = '\200',
		ESC = '\300',
		HOME = '\40',
		PAGE_DOWN = '\62',
		PAGE_UP = '\61',
		PRINT_SCREEN = '\70'
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
			case KEY_FORESLHASH: return '/';
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
	struct changelog_entry {
		string version;
		string codename;
		string date;
		string changes;
	};

	constexpr changelog_entry CHANGELOG[] {
		{
			"0.1",
			nullptr,
			"11.09.2024",
			"The first version. There's no features, only the \"Hello, kernel World!\" message."
		},
		{
			"0.4",
			nullptr,
			"11.09.2024",
			"Print functions, simple colorful project description"
		},
		{
			"0.6",
			"Keyboard Monster",
			"15.09.2024",
			"Keyboard input!!"
		},
		{
			"0.6.1",
			"Keyboard Monster++",
			"15.09.2024",
			"Switched to C++"
		},
		{
			"0.7",
			"Colorful Commander",
			"16.09.2024",
			"Finally, I've made a working shell. Now, you can enter commands.\nThe only command that exists here is shutdown. It halts the kernel."
		},
		{
			"0.7.1",
			"Colorful Commander",
			"17.09.2024",
			"Added some more commands and help."
		},
		{
			"0.7.2",
			"Colorful Commander",
			"17.09.2024",
			"First line animation"
		},
		{
			"0.7.3",
			"Colorful Commander",
			"19.09.2024",
			"Started working on the mouse driver, fixed some bugs, restructured the code.\nFun fact: the first 1000 lines of code have been written!"
		},
		{
			"0.7.4",
			"Colorful Commander",
			"19.09.2024",
			"Automated the system codename & version to the latest release, added release date to 'version' command."
		},
		{
			"0.7.5",
			"Colorful Commander",
			"19.09.2024",
			"Fixed some bugs"
		},
		{
			"0.7.6",
			"Colorful Commander",
			"21.09.2024",
			"Started working on number to string conversion, fixed some bugs"
		}
	};

	/* changelog_entry LATEST_RELEASE = {
		CHANGELOG[LATEST_RELEASE_INDEX].version,
		CHANGELOG[LATEST_RELEASE_INDEX].codename,
		CHANGELOG[LATEST_RELEASE_INDEX].date,
        CHANGELOG[LATEST_RELEASE_INDEX].changes
	}; */

	string VERSION = CHANGELOG[9].version;
	string CODENAME = CHANGELOG[9].codename;
	string RELEASE_DATE = CHANGELOG[9].date;

	int ACPI_CONTROL_REGISTER = 0x400;
	uint8_t ACPI_SHUTDOWN_SIGNAL = 0xFE;

	void sleep(uint32_t timer_count) {
		for (uint32_t i = 0; i < timer_count; i++) {
            asm volatile("nop");
        }
	}
}

// TODO mouse driver
namespace mouse {
	// Define the interrupt number for the PS/2 mouse
	constexpr int IRQ = 12;

	// Define the data port and command port for the PS/2 mouse
	constexpr int DATA_PORT = 0x60;
	constexpr int COMMAND_PORT = 0x64;

	// Define a structure to hold the state of the PS/2 mouse
	struct mouse_state {
	    uint8_t data;
	    uint8_t command;
	};

	// Define a function to read data from the PS/2 mouse
	uint8_t read_data() {
	    // Read the data from the PS/2 mouse
	    const uint8_t data = IO::inb(DATA_PORT);
	    return data;
	}

	// Define a function to write data to the PS/2 mouse
	void write_data(const uint8_t data) {
	    // Write the data to the PS/2 mouse
	    IO::outb(DATA_PORT, data);
	}

	// Define a function to initialize the PS/2 mouse
	void init() {
	    // Write the command to enable data transfer to the PS/2 mouse
	    write_data(0x20);
	}

	// Define a function to process the data from the PS/2 mouse
	void process_data(const uint8_t data) {
		static mouse_state state;

		switch (data) {
			case 0xFE: // Mouse button pressed NOLINT(*-branch-clone)
				break;
			case 0xFF: // Mouse button released
				break;
			case 0xF0: // Data packet
				state.data = data;
				// Process the movement data from the mouse
				if (state.data & 0x80) {
					const size_t x = (state.data & 0x0F) * 8;
					const size_t y = (state.data & 0x70) >> 4;
					// Prsize_t the movement information
					// terminal::prsize_tln(strings::size_t_to_string(x) + ", " + strings::size_t_to_string(y));
					if (!(x > video::VGA_WIDTH || x < 1 || y > video::VGA_HEIGHT || y < 1)) {
						terminal::putentryat(' ', video::WHITE, x, y);
					}
				}
				break;
			default:
				break;
		}
	}

	// Define a function to handle size_terrupts from the PS/2 mouse
	void size_terrupt_handler() {
	    // Read the data from the PS/2 mouse
	    const uint8_t data = read_data();

	    // Process the data from the PS/2 mouse
		process_data(data);

	    // Acknowledge the interrupt
	    IO::outb(COMMAND_PORT, 0x20);
	}

	void register_interrupt_handler() {
	    // TODO Register the interrupt handler for the PS/2 mouse
	}

	// Define a function to clean up the PS/2 mouse driver
	void cleanup() {
	    // TODO Unregister the interrupt handler for the PS/2 mouse
	}

	// Define a function to initialize the PS/2 mouse driver
	void ps2_mouse_init_driver() {
	    init();
	    register_interrupt_handler();
	}

	// Define a function to clean up the PS/2 mouse driver
	void cleanup_driver() {
	    cleanup();
	}
}

void inputLoop() {
	char ch = 0;
	char keycode = 0;
	size_t timer_count = keyboard::INPUT_DELAY;
	terminal::input.buffer[0] = '\0';
	while (true) {
		using namespace terminal;
		using namespace IO;
		using namespace strings;
		using namespace video;
		using namespace keyboard;
		asm volatile("nop"); // A little bit of delay
		if (timer_count-- <= 0) {
			keycode = get_input_keycode();
			// Newline
			if (keycode == KEY_ENTER) {
				print("\n");

				// Commands logic
				if (equals(input.buffer, "shutdown")) {
					uint8_t shutdown_signal = 0xFE;
					// asm volatile("outb %0, %1" :: "a"(shutdown_signal), "Nd"(system::ACPI_CONTROL_REGISTER));
					outb(system::ACPI_CONTROL_REGISTER, shutdown_signal);
					println("ACPI shutdown failed, halting the system");
					return; // Halt the system
				}
				else if (equals(input.buffer, "help")) {
					println("Available commands: ");
					for (size_t i = 0; i < sizeof(console::HELP_TABLE) / sizeof(console::HELP_TABLE[0]); i++) {
						console::help_entry entry = console::HELP_TABLE[i];
						printColor(entry.command, DEFAULT_BG, LIGHT_BROWN);
						print(" - ");
						println(entry.description);
					}
				}
				else if (equals(input.buffer, "version")) {
					printlnColor("denOS", DARK_GREY, WHITE);
					println("");
					print("Kernel version: ");
					printlnColor(system::VERSION, DEFAULT_BG, LIGHT_GREEN);
					print("Codename: ");
					printlnColor(system::CODENAME == nullptr ? "No codename" : system::CODENAME, DEFAULT_BG, LIGHT_BLUE);
					print("Release date: ");
					printlnColor(system::RELEASE_DATE, DEFAULT_BG, LIGHT_MAGENTA);
					println("Compiled using the C++ and asm cross-compiler for i686-elf.");
					println("Type 'help' for more information.");
				}
				else if (equals(input.buffer, "changelog")) {
					println("Changelog:");
					size_t len = sizeof(system::CHANGELOG) / sizeof(system::CHANGELOG[0]);
					for (size_t i = 0; i < len; i++) {
						system::changelog_entry entry = system::CHANGELOG[i];
						printColor(entry.version, DEFAULT_BG, LIGHT_GREEN);
						print(" - ");
						printlnColor(entry.codename == nullptr ? "No codename" : entry.codename, DEFAULT_BG, LIGHT_BLUE);
						print("Released at ");
						printlnColor(entry.date, DEFAULT_BG, LIGHT_MAGENTA);
						println("--------");
						println(entry.changes);
						println("");
					}
					printlnColor("Warning: the dates can be unprecise.", DEFAULT_BG, LIGHT_RED);
					println("The most recent version displays last.");
				}
				else if (equals(input.buffer, "test")) {
					print(toString(100));
				}
				// No valid command is entered
				else if (!equals(input.buffer, "")) {
					print("Unknown command \"");
					printColor(input.buffer, DEFAULT_BG, LIGHT_BROWN);
					print("\". Type '");
					printColor("help", DEFAULT_BG, LIGHT_BROWN);
					println("' for more information.");
				}

				// Command finished, erase the input and update the positions
				print("> ");
				for (size_t i = 0; i < sizeof(input.buffer) / sizeof(input.buffer[0]); i++) {
					input.buffer[i] = '\0';
				}
				input.start.row = terminal_row;
				input.start.col = terminal_column - 1;
				input.end.row = terminal_row;
				input.end.col = terminal_column;
			}
			// Erase
			else if (keycode == KEY_BACKSPACE) {
				if (input.end.col != input.start.col + 1) {
					const size_t index = input.end.row * VGA_WIDTH + input.end.col - 1;
					const size_t index_caret = input.end.row * VGA_WIDTH + input.end.col;
					terminal_buffer[index] = vga_entry(' ', vga_entry_color(BLACK, WHITE));
					terminal_buffer[index_caret] = vga_entry(' ', terminal_color);
					terminal_column--;
					input.end.col--;
					input.buffer[length(input.buffer) - 1] = '\0';
				}
			}
			// Type character
			else if (keycode != 0) {
				ch = get_ascii_char(keycode);
				if (input.start.col == 0 &&
					input.start.row == 0 &&
					input.end.col == 0 &&
					input.end.row == 0
					) {
					input.start = {terminal_row, terminal_column - 1};
					input.end = {terminal_row, terminal_column};
					}
				printColor(ch, DEFAULT_BG, LIGHT_BROWN);
				input.buffer[length(input.buffer)] = ch;
				input.buffer[length(input.buffer) + 1] = '\0';
				input.end.row = input.start.row;
				input.end.col++;
			}
			timer_count = INPUT_DELAY;
		}
	}
}

/**
 *
 */
extern "C" void kernel_main(void) {
	/* Initialize terminal interface */
	terminal::initialize();
	constexpr video::vga_color bg = video::DARK_GREY;

	const auto os = "denOS ";
	const auto kernel = " kernel";

	for (size_t i = 0; i < strings::length(os); i++) {
		system::sleep(100000000);
		terminal::printColor(os[i], bg, video::WHITE);
	}

	for (size_t i = 0; i < strings::length(system::VERSION); i++) {
		system::sleep(100000000);
		terminal::printColor(system::VERSION[i], bg, video::LIGHT_GREEN);
	}

	for (size_t i = 0; i < strings::length(kernel); i++) {
		system::sleep(100000000);
		char data[2];
		data[0] = kernel[i];
		data[1] = '\0';

		terminal::printColor(data, bg, video::WHITE);
	}
	// mouse::ps2_mouse_init_driver();

	// terminal::printColor("denOS ", bg, video::WHITE);
	// terminal::printColor(system::VERSION, bg, video::LIGHT_GREEN);
	// terminal::printlnColor(" kernel", bg, video::WHITE);
	terminal::println("\n");
	terminal::printColor("Features:", video::DEFAULT_BG, video::LIGHT_CYAN);
	terminal::println(" text on the screen, newlines, colors, keyboard (!!!), commands");
	terminal::println("Reset your computer to exit.");
	terminal::println("");
	terminal::printlnColor("WARNING: Doesn't work on x64.", video::DEFAULT_BG, video::LIGHT_RED);
	// terminal::print("Testing number to string: ");
	// terminal::println(strings::int_to_string(5));
	terminal::println("");
	terminal::print("> ");
	inputLoop();
	terminal::println("System halted.");
}