/** @file
 * denOS kernel
 * Created by denis0001-dev on https://gitverse.ru/denis0001-dev/denOS/content/master/src/main/kernel.cpp
 * Version 0.7.12
 * Compiling, linking, and building commands from https://wiki.osdev.org/Bare_Bones
 * DO NOT EDIT OR REMOVE THIS HEADER.
 */

// ReSharper disable CppUnusedIncludeDirective
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Check if the compiler thinks you are targeting the wrong operating system.
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

// This kernel will only work for the 32-bit ix86 targets.
#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

typedef const char* string; ///< A convenience char pointer @code string@endcode alias.

/// @brief I\\O functions that use inline assembly to interact with devices.
namespace IO {
	/**
	 * @brief Read input from a port, such as keyboard (<code>0x60</code>).
	 * @param port A short unsigned (not negative) integer port number.
	 * @return The @code unsigned char@endcode read from the port.
	 */
	uint8_t inb(uint16_t port) {
		uint8_t ret;
		asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
		return ret;
	}

	/**
	 * @brief Send data to a port.
	 * @param port A short unsigned (not negative) integer port number.
	 * @param data The @code unsigned char@endcode to send to the port.
	 */
	void outb(uint16_t port, uint8_t data) {
		asm volatile("outb %0, %1" : "=a"(data) : "d"(port));
	}
}

/// @brief Functions for string manipulation.
namespace strings {
	/**
	 * @brief Returns the count of characters present in a string
	 * before the null terminator (<code>\0</code>).
	 * @param str The string (char pointer).
	 * @return The length in form of an @code unsigned long@endcode.
	 */
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
	}; */

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

	/**
	 * @brief Concatenates two strings.
	 *
	 * It first calculates the length of the final string
	 * @code (length of first + length of second + 1)@endcode.
	 * Then it creates a new string with this length and copies the characters from the first string.
	 * After this, the characters from the second string are copied.
	 * Finally, it adds @code \0@endcode to the end of the string.
	 * @param first The source string.
	 * @param second The string to add to the end of the source string.
	 * @return The first string + the second string.
	 */
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

	/**
	 * @brief Concatenates the first string with the character.
	 *
	 * It uses the first concat function to concatenate.
	 * Before calling this function, it creates a new string with the
	 * character and the null terminator.
	 * @param first The source string.
	 * @param second The character to add to the end of the source string.
	 * @return The result of @code concat(first, {second, '\0'})@endcode.
	 */
	string concat(const string first, const char second) {
		char data[2];
		data[0] = second;
		data[1] = '\0';
		return concat(first, data);
	}

	/**
	 * @brief Check if two strings match, without counting the characters
	 * after the null terminator.
	 *
	 * If the length of the first string doesn't match the length of the
	 * second string, returns @code false@endcode.
	 * Otherwise, it compares each character of the strings
	 * at the same position. If any character doesn't match, returns @code false@endcode.
	 * If all characters match, returns @code true@endcode.
	 * @param first The first string.
	 * @param second The second string.
	 * @return If the strings match, @code true@endcode, otherwise @code false@endcode.
	 */
	bool equals(const string first, const string second) {
		const size_t len1 = length(first);
		if (len1 != length(second)) return false;
		for (size_t i = 0; i < len1; i++) {
			if (first[i] != second[i]) return false;
		}
		return true;
	}

	bool startsWith(const string str, const string prefix) {
		if (length(str) < length(prefix)) return false;
		size_t matched = 0;

		for (size_t i = 0; i <= length(str); i++) {
			if (matched == length(prefix)) return true;
			if (str[i] != prefix[i]) return false;
            matched++;
		}
		return false;
	}

	/**
	 * @brief Get the substring out of a string between the start & end positions.
	 *
	 * It iterates over the source string starting from the start position,
	 * and adds the character to the final string.
	 * When it reaches the end position, it adds @code \0@endcode to the final string.
	 * @param str The source string.
	 * @param start The start position in the source string.
	 * @param end The end position in the source string.
	 * @return The characters between the start and end positions.
	 */
	string substring(const string str, const size_t start, const size_t end) {
		const size_t len = end - start;
		char new_data[len + 1];

		for (size_t i = start; i < end; i++) {
			new_data[i - start] = str[i];
		}

		new_data[len] = '\0';
		return new_data;
	}

	/**
	 * @brief Get the substring out of a string from the start to the end of the string.
	 * @param str The source string.
	 * @param start The start position in the source string.
	 * @return Result of @code substring(str, start, length(str)@endcode.
	 */
	string substring(const string str, const size_t start) {
		return substring(str, start, length(str));
	}

	/**
	 * @brief Reverses the characters of a string.
	 * @param str The source string.
	 * @return The characters of the source string in reverse order.
	 */
	string reverse(const string str) {
		const size_t length = strings::length(str);
		char temp[length];

		for (size_t i = 0; i < length; i++) {
			temp[i] = str[length - i - 1];
		}
		return temp;
	}

	int numDigits(const int32_t x) { // NOLINT(*-no-recursion)
		if (x == INT32_MIN) return 10 + 1;
		if (x < 0) return numDigits(-x) + 1;

		if (x >= 10000) {
			if (x >= 10000000) {
				if (x >= 100000000) {
					if (x >= 1000000000)
						return 10;
					return 9;
				}
				return 8;
			}
			if (x >= 100000) {
				if (x >= 1000000)
					return 7;
				return 6;
			}
			return 5;
		}
		if (x >= 100) {
			if (x >= 1000)
				return 4;
			return 3;
		}
		if (x >= 10)
			return 2;
		return 1;
	}

	/**
	 * @brief Lowercases each character of the given string if possible.
	 * @param str The string.
	 * @return The string with all the characters converted to lowercase.
	 */
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

	char* itoa(int value, char* str, const int radix) {
		char* ptr;
		// Check for supported base.
		if (radix < 2 || radix > 36 ){
			*str = '\0';
			return str;
		}
		char *rc = ptr = str;
		// Set '-' for negative decimals.
		if (value < 0 && radix == 10 ) {
			*ptr++ = '-';
		}
		// Remember where the numbers start.
		char *low = ptr;
		// The actual conversion.
		while (value) {
			// Modulo is negative for negative value. This trick makes abs() unnecessary.
			*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % radix];
			value /= radix;
		}
		// Terminating the string.
		*ptr-- = '\0';
		// Invert the numbers.
		while (low < ptr) {
			const char tmp = *low;
			*low++ = *ptr;
			*ptr-- = tmp;
		}
		return rc;
	}

	/**
	 * @brief Converts an integer to a string.
	 * @param num The integer to convert.
	 * @return The integer as a string.
	 */
	char* toString(const int num) {
		char str[numDigits(num)];
		itoa(num, str, 10);
		return str;
	}
}

/// @brief Contains functions for managing the video output.
namespace video {
	/// @brief VGA text mode colors.
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

	static constexpr size_t VGA_WIDTH = 80; ///< The VGA screen width.
	static constexpr size_t VGA_HEIGHT = 25; ///< The VGA screen height.

	static vga_color DEFAULT_BG = BLACK; ///< The default background color.
	static vga_color DEFAULT_FG = LIGHT_GREY; ///< The default foreground (text) color.

	size_t terminal_row; ///< The current row to which the next character will be placed.
	size_t terminal_column; ///< The current column to which the next character will be placed.
	uint8_t terminal_color; ///< The current terminal color in which the next character will be printed.
	uint16_t* terminal_buffer; ///< The buffer containing the terminal characters.

	/**
	 * @brief Combines two colors together into a single value.
	 * @param fg The foreground (text) color.
	 * @param bg The background color.
	 * @return The @code char@endcode with the foreground & background color combined into one value.
	 */
	static uint8_t vga_entry_color(const vga_color fg, const vga_color bg) {
		return fg | bg << 4;
	}

	/**
	 * @brief Makes a VGA entry that con be put into @link terminal_buffer@endlink
	 * to make it appear on the screen.
	 * @param uc The character to display.
	 * @param color The color got from @link vga_entry_color(fg, bg)@endlink.
	 * @return The character and the color combined into a single value.
	 */
	static uint16_t vga_entry(const uint16_t uc, const uint8_t color) {
		return static_cast<uint16_t>(uc) | static_cast<uint16_t>(color) << 8;
	}

	/// @brief A structure holding the data from the parameters of @link vga_entry()@endlink.
	struct VGAEntry {
		uint16_t uc;
		uint8_t color;
	};

	/**
	 * @brief Get the parameters of @link vga_entry()@endlink from its result.
	 * @param result The result of the @link vga_entry()@endlink function.
	 * @return The uc and color values in the @link VGAEntry@endlink structure.
	 */
	VGAEntry vgaEntryFromResult(const uint16_t result) {
		// Получаем младший байт результата для цвета
		VGAEntry entry = {};

		entry.color = static_cast<uint8_t>(result & 0xFF);

		// Получаем старший байт результата для uc
		entry.uc = static_cast<uint16_t>(result >> 8 & 0xFFFF);
		return entry;
	}
}

/// @brief Functions for managing the terminal.
namespace terminal {
	/**
	 * @brief Clears the characters on the screen.
	 *
	 * It replaces all characters in @link video::terminal_buffer@endlink with spaces.
	 */
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

	/**
	 * @brief Initializes the terminal by setting the variables to the default values.
	 *
	 * The values cannot be initialized right at declaration, because the kernel needs to
	 * boot first.
	 * So, when the kernel is ready, it calls this function to initialize the terminal and
	 * get it ready to work.
	 */
	void initialize() {
		video::terminal_row = 0;
		video::terminal_column = 0;
		video::terminal_color = vga_entry_color(video::DEFAULT_FG, video::DEFAULT_BG);
		video::terminal_buffer = reinterpret_cast<uint16_t *>(0xB8000);
		clear();
	}

	/**
	 * @brief Displays a character in the specified color at the specified position.
	 *
	 * First, it calculates the position in the @link video::terminal_buffer@endlink.
	 * Then, it sets the result of @link video::vga_entry(c, color)@endlink to the
	 * calculated position.
	 * @param c The character to display.
	 * @param color The color in which the character should be displayed.
	 * @param x The x position on the screen.
	 * @param y The y position on the screen.
	 */
	void putentryat(const char c, const uint8_t color, const size_t x, const size_t y) {
		const size_t index = y * video::VGA_WIDTH + x;
		video::terminal_buffer[index] = video::vga_entry(c, color);
	}

	/**
	 * @brief Scrolls the terminal by the specified amount of lines.
	 *
	 * It discards the number of lines specified from the top of the screen,
	 * and re-displays all the characters.
	 * @bug It erases the top line after scrolling.
	 * @param lines The number of lines to scroll.
	 */
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

	/**
	 * @brief Prints the character after the last character printed in the default terminal color.
	 * The terminal will automatically scroll by 1 line if there's not enough space to display
	 * the character.
	 * <br/>
	 * If the character is @code \n@endcode, the terminal will go on a new line.
	 * @param c The character to print.
	 */
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

	/**
	 * @brief Displays the specified string on the screen.
	 *
	 * It calls @link printChar(c)@endlink for each character in the string.
	 * @param data The string to display.
	 */
	void print(const string data) {
		for (size_t i = 0; i < strings::length(data); i++) printChar(data[i]);
	}

	/**
	 * @brief Display the specified string in the specified colors on the screen.
	 *
	 * It sets the @link video::terminal_color@endlink to the specified color,
	 * calls @link print@endlink, and sets the color back to the default.
	 * @param data The string to display.
	 * @param bg The background color.
	 * @param fg The foreground (text) color.
	 */
	void printColor(const string data, const video::vga_color bg, const video::vga_color fg) {
		video::terminal_color = vga_entry_color(fg, bg);
		print(data);
		video::terminal_color = vga_entry_color(video::DEFAULT_FG, video::DEFAULT_BG);
	}

	/**
	 * @brief Display the specified character in the specified colors on the screen.
	 *
	 * It constructs a string out of the character, and calls
	 * @link printColor(string, video::vga_color, video::vga_color)@endlink.
	 * @param data The character to display.
	 * @param bg The background color.
	 * @param fg The foreground (text) color.
	 */
	void printColor(const char data, const video::vga_color bg, const video::vga_color fg) {
		char str[2];
		str[0] = data;
		str[1] = '\0';
		printColor(str, bg, fg);
	}

	/**
	 * @brief Displays a string on the screen, and goes to a new line after the string.
	 *
	 * It just prints the string, and prints @code \n@endcode.
	 * @param data The string to display.
	 */
	void println(const string data) {
		print(data);
		print("\n");
	}

	/**
	 * @brief Displays a string on the screen in the specified colors, and goes to a new line
	 * after the string.
	 *
	 * It just prints the string in the specified colors, and prints @code\n@endcode.
	 * @param data The string to display
	 * @param bg The background color.
	 * @param fg The foreground (text) color.
	 */
	void printlnColor(const string data, const video::vga_color bg, const video::vga_color fg) {
		printColor(data, bg, fg);
		print("\n");
	}

	/// @brief X, Y coordinates.
	struct pos {
		size_t row; ///< Y coordinate
		size_t col; ///< X coordinate
	};

	/// @brief The current input info.
	struct current_input {
		size_t size; ///< The count of characters in the input
		pos start; ///< The starting position
		pos end; ///< The ending position
		char buffer[video::VGA_WIDTH - 2]; ///< The input buffer
	};

	/// The default values for an empty input.
	constexpr current_input default_input = {0, {0, 0}, {0, 0}, ""};

	current_input input = default_input; ///< The current input.
}

/// @brief Variables used for the console.
namespace console {
	/// @brief An entry for the help command.
	struct help_entry {
		string command; ///< The command.
		string description; ///< The command description.
	};

	/// @brief Helpful description for each command.
	constexpr help_entry HELP_TABLE[] = {
		{"shutdown", "Shut down the system"},
		{"help", "Display this help message"},
		{"version", "Display version information"},
		{"changelog", "Display changes recently made"},
		{"repeat, echo", "Repeat the same text that was typed after this command's name"}
	};
}

/// @brief Functions & variables for handling the keyboard.
namespace keyboard {
	constexpr int PORT = 0x60; ///< The keyboard I\\O port.
	/**
	 * @brief The count of times to repeat the @code nop@endcode instruction.
	 *
	 * This is needed because the processor is very fast, so if you press a key,
	 * the entire screen will be filled with the character matching the key
	 * thousands of times. To prevent this, the @code nop@endcode instruction is
	 * used to delay reading the keyboard.
	 */
	constexpr int INPUT_DELAY =
		#ifndef DEBUG
			500000000
	    #else
			125000000
		#endif
	;

	/// @brief The scancodes for each key that exists on the keyboard.
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

	/// @brief The special characters used to represent some keys.
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

	/**
	 * @brief Gets the current pressed key's code.
	 * @return The keycode read from the keyboard,
	 * or 0 if no key was pressed at the moment of calling this function.
	 */
	char getInputKeycode() {
		char ch = 0;
		while((ch = IO::inb(PORT)) != 0) { // NOLINT(*-narrowing-conversions)
			if (ch > 0) return ch;
		}
		return ch;
	}

	/**
	 * @brief Convert a keycode to a character that can be displayed on
	 * the screen.
	 * @param keycode The keycode got from @link getInputKeycode@endlink.
	 * @return The character representing the key.
	 */
	char keycodeToReadableChar(const char keycode) {
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

/*! @namespace System
 * Contains system-related methods and information about the release.
 */
namespace System {
	/*! @struct ChangelogEntry
	 * A convenience structure for the changelog versions.
	 */
	struct ChangelogEntry {
		/// The version in format @code [MAJOR].[MINOR].[PATCH]@endcode.
		string version;
		/// The codename of the version. Use @code nullptr@endcode if there's none.
		string codename;
		/// The date of the release in format @code [YYYY-MM-DD]@endcode.
		string date;
		/// The list of changes for this version.
		string changes;
	};

	/// The register for sending <a href="https://wiki.osdev.org/ACPI">ACPI</a> commands to the computer.
	int ACPI_CONTROL_REGISTER = 0x400;
	/// The <a href="https://wiki.osdev.org/ACPI">ACPI</a> power off signal.
	uint8_t ACPI_SHUTDOWN_SIGNAL = 0xFE;

	/*!
	 * The history of versions of this OS starting from the oldest.
	 * <br/>
	 * @link System::ChangelogEntry@endlink is used to contain each version.
	 */
	constexpr ChangelogEntry CHANGELOG[] {
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
		},
		{
			"0.7.8",
			"Colorful Commander",
			"22.09.2024",
			"Fixed some bugs, restructured the code"
		},
		{
			"0.7.10",
			"Colorful Commander",
			"22.09.2024",
			"Fixed some bugs"
		},
		{
			"0.7.11",
			"Colorful Commander",
			"26.09.2024",
			"Documented the code"
		},
		{
			"0.7.12",
			"Colorful Commander",
			"26.09.2024",
			"Readme"
		}
	};

	/* changelog_entry LATEST_RELEASE = {
		CHANGELOG[LATEST_RELEASE_INDEX].version,
		CHANGELOG[LATEST_RELEASE_INDEX].codename,
		CHANGELOG[LATEST_RELEASE_INDEX].date,
        CHANGELOG[LATEST_RELEASE_INDEX].changes
	}; */

	string VERSION = CHANGELOG[12].version; ///< The system version.
	string CODENAME = CHANGELOG[12].codename; ///< The system codename. Will be @code nullptr@endcode if none.
	string RELEASE_DATE = CHANGELOG[12].date; ///< @link ChangelogEntry.date@endlink

	/// Exit codes for a command.
	enum exitCodes {
		SUCCESS = 0, ///< Command succeeded.
		CMD_NOT_FOUND = 1, ///< Command wasn't found.
		NO_COMMAND = 3, ///< No command was entered, do nothing.
	};

	/// Special exit codes that indicate a system command such as shutdown.
	enum signals {
		SHUTDOWN = 2, ///< Special code for halting the system.
	};

	/**
	 * Executes the assembly @code nop@endcode instruction for the given amount of times,
	 * delaying execution.
	 * <b>Warning: the time of the delay cannot be predicted, it depends on your computer's speed.</b>
	 */
	void sleep(uint32_t timer_count) {
		for (uint32_t i = 0; i < timer_count; i++) {
            asm volatile("nop");
        }
	}

	enum ports {
		/* keyboard interface IO port: data and control
	READ:   status port
	WRITE:  control register */
		KEYBOARD_INTERFACE = 0x64,
		KEYBOARD_IO = 0x60, /* keyboard IO port */
		KEYBOARD_RESET = 0xFE /* reset CPU command */
	};

	// Define the CPUID instruction
	#define CPUID(func, eax, ebx, ecx, edx) \
		asm volatile("cpuid" : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx) : "0" (func))

	struct CPUID {
		uint64_t frequency;
		char* vendor;
	};

	// Function to get the CPU frequency
	CPUID getCPUID() {
		uint32_t eax, ebx, ecx, edx;

		// Get the CPU vendor string
		CPUID(0, eax, ebx, ecx, edx);
		char vendor[13];
		*reinterpret_cast<uint32_t *>(vendor) = ebx;
		*reinterpret_cast<uint32_t *>(vendor + 4) = edx;
		*reinterpret_cast<uint32_t *>(vendor + 8) = ecx;
		vendor[12] = '\0';

		// Check if the CPU supports the RDTSC instruction
		if (eax >= 1) {
			CPUID(1, eax, ebx, ecx, edx);
			if (edx & 1 << 4) {
				// Get the TSC frequency
				uint64_t tsc_start, tsc_end;
				uint32_t tsc_start_low, tsc_start_high, tsc_end_low, tsc_end_high;

				// Read the TSC before and after a short delay
				asm volatile("rdtsc" : "=a" (tsc_start_low), "=d" (tsc_start_high));
				// Do something that takes some time
				for (volatile int i = 0; i < 1000000; i++) {
					asm volatile("nop");
				}
				asm volatile("rdtsc" : "=a" (tsc_end_low), "=d" (tsc_end_high));

				// Combine the TSC values
				tsc_start = static_cast<uint64_t>(tsc_start_high) << 32 | tsc_start_low;
				tsc_end = static_cast<uint64_t>(tsc_end_high) << 32 | tsc_end_low;

				// Calculate the TSC frequency
				uint64_t tsc_frequency = tsc_end - tsc_start;

				// Return the TSC frequency
				return CPUID { tsc_frequency, vendor };
			}
		}

		// Return 0 if the TSC frequency cannot be determined
		return CPUID { 0, vendor };
	}

	void disableInterrupts() {
		asm volatile("cli");
	}

	[[noreturn]] void halt() {
		loop:
			asm volatile("hlt"); /* if that didn't work, halt the CPU */
		goto loop; /* if a NMI is received, halt again */
	}

	/* keyboard interface bits */
	#define KBRD_BIT_KDATA 0 /* keyboard data is in buffer (output buffer is empty) (bit 0) */
	#define KBRD_BIT_UDATA 1 /* user data is in buffer (command buffer is empty) (bit 1) */

	#define bit(n) (1<<(n)) /* Set bit n to 1 */

	/* Check if bit n in flags is set */
	#define check_flag(flags, n) ((flags) & bit(n))

	void reboot() {
		// Send the reset command to the keyboard controller
		IO::outb(KEYBOARD_INTERFACE, KEYBOARD_RESET);

		// Wait for a while to ensure the command is processed
		for (int i = 0; i < 100000; i++) {
			asm volatile("nop");
		}

		// Reset the CPU
		asm volatile("jmp 0xFFFFFFF0");
	}

	/**
	 * A function that will process the command string and execute the given command.
	 * @param command The command to execute, or @code""@endcode to do nothing.
	 * @return An exit code from @link exitCodes@endlink or @link signals@endlink.
	 */
	int processCommand(const string command) {
		using namespace strings;
		using namespace terminal;
		using namespace video;

		const bool startsWithRepeat = startsWith(command, "repeat");
		const bool startsWithEcho = startsWith(command, "echo");

		if (equals(command, "shutdown")) {
			IO::outb(ACPI_CONTROL_REGISTER, ACPI_SHUTDOWN_SIGNAL);
			println("ACPI shutdown failed, halting the system");
			return SHUTDOWN; // Halt the system
		}
		else if (equals(command, "help")) {
			println("Available commands: ");
			for (size_t i = 0; i < sizeof(console::HELP_TABLE) / sizeof(console::HELP_TABLE[0]); i++) {
				const auto [command, description] = console::HELP_TABLE[i];
				printColor(command, DEFAULT_BG, LIGHT_BROWN);
				print(" - ");
				println(description);
			}
		}
		else if (equals(command, "version")) {
			printlnColor("denOS", DARK_GREY, WHITE);
			println("");
			print("Kernel version: ");
			printlnColor(VERSION, DEFAULT_BG, LIGHT_GREEN);
			print("Codename: ");
			printlnColor(CODENAME == nullptr ? "No codename" : CODENAME, DEFAULT_BG, LIGHT_BLUE);
			print("Release date: ");
			printlnColor(RELEASE_DATE, DEFAULT_BG, LIGHT_MAGENTA);
			print("Build: ");
			#ifdef DEBUG
				printColor("Debug", DEFAULT_BG, MAGENTA);
			#else
				printColor("Release", DEFAULT_BG, MAGENTA);
			#endif
			println("Compiled using the C++ and asm cross-compiler for i686-elf.");
			println("Type 'help' for more information.");
		}
		else if (equals(command, "changelog")) {
			println("Changelog:");
			constexpr size_t len = sizeof(CHANGELOG) / sizeof(CHANGELOG[0]);
			for (size_t i = 0; i < len; i++) {
				const auto [version, codename, date, changes] = CHANGELOG[i];
				printColor(version, DEFAULT_BG, LIGHT_GREEN);
				print(" - ");
				printlnColor(codename == nullptr ? "No codename" : codename, DEFAULT_BG, LIGHT_BLUE);
				print("Released at ");
				printlnColor(date, DEFAULT_BG, LIGHT_MAGENTA);
				println("--------");
				println(changes);
				println("");
			}
			printlnColor("Warning: the dates can be unprecise.", DEFAULT_BG, LIGHT_RED);
			println("The most recent version displays last.");
		}
		else if (startsWithRepeat || startsWithEcho) {
			size_t start = 0;
			if (startsWithRepeat) {
				start = 7;
			} else {
				start = 5;
			}

			char inp[80];
			for (int i = start; i < 78; i++) {
				if (command[i] == '\0') {
					break;
				}
				inp[i - start] = command[i];
			}
			println(inp);
		}
		else if (equals(command, "reboot")) {
			reboot();
		}
		else if (equals(command, "cpuid")) {
			const auto cpuid = getCPUID();
			print("Frequency: ");
			char* freqString = toString(cpuid.frequency);
			println(freqString);
			print("Vendor: ");
			println(cpuid.vendor);
		}
		else if (equals(command, "")) {
			return NO_COMMAND;
		}
		// No valid command is entered
		else if (!equals(command, "")) {
			print("Unknown command \"");
			printColor(input.buffer, DEFAULT_BG, LIGHT_BROWN);
			print("\". Type '");
			printColor("help", DEFAULT_BG, LIGHT_BROWN);
			println("' for more information.");
			return CMD_NOT_FOUND;
		}
		return SUCCESS;
	}
}

/**
 * Not implemented yet.
 */
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

/**
 * This function gets data from the keyboard in an interval,
 * gets the keycode and processes it.
 *   - If @code Enter@endcode was pressed, new line will be inserted, and
 *     the command will be executed.
 *   - If @code Backspace@endcode was pressed, the last character that the
 *     user typed will be deleted, and the cursor position will decrease by 1.
 *   - If any other key was pressed, the appropiate character will be displayed
 *     on the screen and the cursor position will increase by 1. The maximun
 *     amount of characters you can enter is determined by VGA_WIDTH.
 *
 * To make a delay, it uses the @code nop@endcode assembly instruction.
 */
// ReSharper disable CppDFAConstantConditions
// ReSharper disable CppDFAUnreachableCode
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
			keycode = getInputKeycode();
			// Newline
			if (keycode == KEY_ENTER) {
				print("\n");

				// Commands logic
				if (int code = System::processCommand(input.buffer); code == System::SHUTDOWN) {
					return; // Halt the system
				} else if (code == System::SUCCESS) {
					println("\nCommand executed successfully.");
				} else if (code != System::CMD_NOT_FOUND && code != System::NO_COMMAND) {
					println("\nCommand failed.");
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
				ch = keycodeToReadableChar(keycode);
				if (ch != 0) {
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
			}
			timer_count = INPUT_DELAY;
		}
	}
}

/**
 * <b>Entry point</b>
 * <p>
 * This function has to be declared with <code>extern "C"</code>,
 * so this function can be called from the <a href="https://gitverse.ru/denis0001-dev/denOS/content/master/src/main/boot.s">boot.s</a> bootstrap file.
 * Otherwise, this function will compile with a different name, and the linker will
 * not be able to call this function.
 * </p>
 * <b>
 * Note: the code snippets are not colored and the whitespaces are not preserved,
 * so the things you see here are different from the actual OS.
 * </b>
 * <p>
 * The system will "type" this text: @code denOS 0.0.0 kernel@endcode
 * <br/>
 * Then, it will display some features that this system has.
 * Here's the example output you will see:
 * @code
 * denOS 0.0.0 kernel
 * [space]
 * Features: text on the screen, newlines, colors, keyboard (!!!), commands
 * Reset your computer to exit.
 * [space]
 * WARNING: Doesn't work on x64.
 * [space]
 * >
 * @endcode
 * You will see a white box after the > character, which is the cursor.
 * <br/>
 * Currently, you can't type capital letters.
 * <br/>
 * Now, you can type some commands.
 * <br/>
 * For example, type @code help@endcode to see a list of available commands.
 * The help command will look like this:
 * @code
 * > help
 * help - print this help message
 * shutdown - halt the system
 * version - print the version and build information
 * ...
 * @endcode
 * <b>Note: The messages shown in this code snippet may differ from the actual result.</b>
 * <br/>
 * You can look at the changelog for the list of changes recently made:
 * @code
 * > changelog
 * Changelog:
 * 0.0.0 - Example Codename
 * Released at 01.01.2000
 * --------
 * Example changes list, the changes weren't made because this is just an example :)
 * [space]
 * 0.1.0 - Example Codename 2
 * Released at 02.01.2000
 * --------
 * Example changes list, the changes weren't made because this is just a 2nd example :)
 * ...
 * @endcode
 * You can look for more commands in the help table.
 * </p>
 * @see system:HELP_TABLE
 */
extern "C" void kernel_main(void) {
	terminal::initialize();
	constexpr video::vga_color bg = video::DARK_GREY;

	// Animate "typing" "denOS x.x.x kernel"
	const auto os = "denOS ";
	const auto kernel = " kernel";
	#ifndef DEBUG
	for (size_t i = 0; i < strings::length(os); i++) {
		System::sleep(100000000);
		terminal::printColor(os[i], bg, video::WHITE);
	}

	for (size_t i = 0; i < strings::length(System::VERSION); i++) {
		System::sleep(100000000);
		terminal::printColor(System::VERSION[i], bg, video::LIGHT_GREEN);
	}

	for (size_t i = 0; i < strings::length(kernel); i++) {
		System::sleep(100000000);
		char data[2];
		data[0] = kernel[i];
		data[1] = '\0';

		terminal::printColor(data, bg, video::WHITE);
	}
	#else
	terminal::printColor(os, bg, video::WHITE);
	terminal::printColor(System::VERSION, bg, video::LIGHT_GREEN);
	terminal::printColor(kernel, bg, video::WHITE);
	#endif
	// mouse::ps2_mouse_init_driver();

	terminal::println("\n");
	terminal::printColor("Features:", video::DEFAULT_BG, video::LIGHT_CYAN);
	terminal::println(" text on the screen, newlines, colors, keyboard (!!!), commands");
	terminal::println("Reset your computer to exit.");
	terminal::println("");
	terminal::printlnColor("WARNING: Doesn't work on x64.", video::DEFAULT_BG, video::LIGHT_RED);
	terminal::println("");
	terminal::print("> ");
	inputLoop();
	terminal::println("System halted."); // After this the system will be halted
}