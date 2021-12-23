#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BIT(x)	1 << x
#define SIZE(x) (sizeof(x) / sizeof(x[0]))

#ifndef NDEBUG
#define LOGV(x, fmt)                                                           \
	printf("%s:%d | %s = " fmt "\n", __PRETTY_FUNCTION__, __LINE__, #x, x);
#else
#define LOGV(x, fmt)
#endif

#define INPUT_BUF_LEN 1024
#define REPORT_LENGTH 8

using byte = unsigned char;

struct opt {
	const char* key;
	byte		code;
};

namespace keyboard {
	opt modifiers[] = {
		{"left-ctrl", BIT(0)},
		{"left-shift", BIT(1)},
		{"left-alt", BIT(2)},
		{"left-meta", BIT(3)},

		{"right-ctrl", BIT(4)},
		{"right-shift", BIT(5)},
		{"right-alt", BIT(6)},
		{"right-meta", BIT(7)},
	};

	opt keycodes[] = {
		{"a", 4},
		{"b", 5},
		{"c", 6},
		{"d", 7},
		{"e", 8},
		{"f", 9},
		{"g", 10},
		{"h", 11},
		{"i", 12},
		{"j", 13},
		{"k", 14},
		{"l", 15},
		{"m", 16},
		{"n", 17},
		{"o", 18},
		{"p", 19},
		{"q", 20},
		{"r", 21},
		{"s", 22},
		{"t", 23},
		{"u", 24},
		{"v", 25},
		{"w", 26},
		{"x", 27},
		{"y", 28},
		{"z", 29},
		{"1", 30},
		{"2", 31},
		{"3", 32},
		{"4", 33},
		{"5", 34},
		{"6", 35},
		{"7", 36},
		{"8", 37},
		{"9", 38},
		{"0", 39},
		{"enter", 40},
		{"return", 40},
		{"esc", 41},
		{"escape", 41},
		{"bksp", 42},
		{"backspace", 42},
		{"tab", 43},
		{"space", 44},
		{"-", 45},
		{"dash", 45},
		{"minus", 45},
		{"=", 46},
		{"equal", 46},
		{"equals", 46},
		{"[", 47},
		{"lbracket", 47},
		{"left-bracket", 47},
		{"]", 48},
		{"rbracket", 48},
		{"right-bracket", 48},
		{"\\", 49},
		{"bslash", 49},
		{"backslash", 49},
		{"back-slash", 49},
		{";", 51},
		{"semicolon", 51},
		{"'", 52},
		{"quote", 52},
		{"~", 53},
		{"tilde", 53},
		{",", 54},
		{"comma", 54},
		{".", 55},
		{"period", 55},
		{"/", 56},
		{"slash", 56},
		{"fslash", 56},
		{"forward-slash", 56},
		{"caps-lock", 57},
		{"f1", 58},
		{"f2", 59},
		{"f3", 60},
		{"f4", 61},
		{"f5", 62},
		{"f6", 63},
		{"f7", 64},
		{"f8", 65},
		{"f9", 66},
		{"f10", 67},
		{"f11", 68},
		{"f12", 69},
		{"print-screen", 70},
		{"prt-scr", 70},
		{"prtscr", 70},
		{"scroll-lock", 71},
		{"pause", 72},
		{"insert", 73},
		{"home", 74},
		{"page-up", 75},
		{"pg-up", 75},
		{"delete", 76},
		{"del", 76},
		{"end", 77},
		{"page-down", 78},
		{"pg-down", 78},
		{"right", 79},
		{"left", 80},
		{"down", 81},
		{"up", 82},
		{"num-lock", 83},
		{"kp-slash", 84},
		{"kp-multiply", 85},
		{"kp-minus", 86},
		{"kp-dash", 86},
		{"kp-plus", 87},
		{"kp-enter", 88},
		{"kp-return", 88},
		{"kp-1", 89},
		{"kp-2", 90},
		{"kp-3", 91},
		{"kp-4", 92},
		{"kp-5", 93},
		{"kp-6", 94},
		{"kp-7", 95},
		{"kp-8", 96},
		{"kp-9", 97},
		{"kp-0", 98},
		{"kp-delete", 99},
		{"kp-del", 99},
		{"applic", 101},
		{"power", 102},
		{"kp-equal", 103},
		{"kp-equals", 103},
		{"f13", 104},
		{"f14", 105},
		{"f15", 106},
		{"f16", 107},
		{"f17", 108},
		{"f18", 109},
		{"f19", 110},
		{"f20", 111},
		{"f21", 112},
		{"f22", 113},
		{"f23", 114},
		{"f24", 115},
		{"execute", 116},
		{"exec", 116},
		{"help", 117},
		{"menu", 118},
		{"select", 119},
		{"stop", 120},
		{"again", 121},
		{"undo", 122},
		{"cut", 123},
		{"copy", 124},
		{"paste", 125},
		{"find", 126},
		{"mute", 127},
		{"volume-up", 128},
		{"vol-up", 128},
		{"volume-down", 129},
		{"vol-down", 129},
		{"kp-comma", 133},
	};

	static int write_to_keyboard(const char* kdev, char report[REPORT_LENGTH]) {
		int kfd = open(kdev, O_WRONLY);
		if (kfd < 0) {
			perror("open");
			LOGV(kfd, "%d")
			return kfd;
		}
		int ret = write(kfd, report, REPORT_LENGTH);

		char empty[REPORT_LENGTH] = {0};
		write(kfd, empty, REPORT_LENGTH);
		close(kfd);

		return ret;
	}

	void parse(const char* kdev, char* buf) {
		// keypress index in report
		int	 kp_index			   = 0;
		char report[REPORT_LENGTH] = {0};

		for (char* pos = strtok(buf, " "); pos != NULL;) {
			// add modifiers
			for (size_t i = 0; i < SIZE(modifiers); i++)
				if (strcmp(modifiers[i].key, pos) == 0)
					report[0] |= modifiers[i].code;

			// add keypresses
			if (kp_index < REPORT_LENGTH - 2) {
				for (size_t i = 0; i < SIZE(keycodes); i++)
					if (strcmp(keycodes[i].key, pos) == 0) {
						// check if this key has already been added to the
						// report, if yes, write this one and start making
						// another one
						for (size_t j = 0; j < REPORT_LENGTH - 2; j++) {
							if (report[2 + j] == keycodes[i].code) {
								write_to_keyboard(kdev, report);

								// reset only the keypresses part of the report
								// (byte [2:])
								memset(report + 2, 0, REPORT_LENGTH - 2);
								kp_index = 0;
								continue;
							}
						}
						LOGV(keycodes[i].key, "%s")
						LOGV(keycodes[i].code, "%d")
						report[2 + kp_index++] = keycodes[i].code;
					}
			} else { // we have reached the end of the report but there are more
					 // keys to press, write this one and make a new one with
					 // the same modifiers so there is no weird behavior
				write_to_keyboard(kdev, report);

				// reset only the keypresses part of the report (bytes [2:])
				memset(report + 2, 0, REPORT_LENGTH - 2);
				kp_index = 0;
				continue;
			}

			// move to next input
			pos = strtok(NULL, " ");
		}
		write_to_keyboard(kdev, report);
	}

} // namespace keyboard

int main(int argc, char* argv[]) {
	// input buffer
	char* input;

	if (argc < 2) {
		printf("Usage: %s [hidgX] {data}\n", argv[0]);
		return 0;
	}

	// read from stdin, unless argv[2] was passed
	if (argc < 3) {
		input = (char*)calloc(INPUT_BUF_LEN, sizeof(char));
		read(STDIN_FILENO, input, INPUT_BUF_LEN);
	} else {
		input = argv[2];
	}

	keyboard::parse(argv[1], input);

	if (argc < 3)
		free(input);
}