#include <iostream>
#include "stb_image.h"

#ifdef _WIN32
#include <windows.h>
#endif

const std::string RESET_ATTRIBUTES = "\x1b[0m";

void set_fg_color(std::ostream &stream, const unsigned char *pixel) {
    const int red = pixel[0], green = pixel[1], blue = pixel[2];
    stream << "\x1b[38;2;" << red << ";" << green << ";" << blue << "m";
}

void set_bg_color(std::ostream &stream, const unsigned char *pixel) {
    const int red = pixel[0], green = pixel[1], blue = pixel[2];
    stream << "\x1b[48;2;" << red << ";" << green << ";" << blue << "m";
}

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    int w, h, channels;
    unsigned char *data = stbi_load(argv[1], &w, &h, &channels, 3);

    if (!data) {
        std::cerr << "Loading image failed: " << stbi_failure_reason() << std::endl;
        return 1;
    }

#ifdef _WIN32
    HANDLE std_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD console_mode;
    GetConsoleMode(std_output_handle, &console_mode);
    SetConsoleMode(std_output_handle, console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
    UINT encoding = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);
#endif

    const unsigned char row_size = w * 3;
    int i = 0;
    for (int y = 0; y < h - 1; y += 2) {
        // draw two rows at a time
        for (int x = 0; x < w; x++) {
            // foreground color; upper row
            set_fg_color(std::cout, data + i);
            // background color; bottom row
            set_bg_color(std::cout, data + i + row_size);
            // draw pixel character (▀)
            std::cout << "▀";

            i += 3;
        }
        std::cout << RESET_ATTRIBUTES;
        if (y < h - 3) {
            std::cout << std::endl;
        }
        // skip bottom row as it's already drawn
        i += row_size;
    }
    // if odd number of rows
    if (h & 0x1) {
        std::cout << std::endl;
        // draw last row
        for (int x = 0; x < w; x++) {
            // foreground color; only upper row
            set_fg_color(std::cout, data + i);
            // draw pixel character (▀)
            std::cout << "▀";

            i += 3;
        }
    }

    stbi_image_free(data);

    // reset colors
    std::cout << RESET_ATTRIBUTES;

#ifdef _WIN32
    SetConsoleMode(std_output_handle, console_mode);
    SetConsoleCP(encoding);
#endif

    return 0;
}
