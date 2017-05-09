#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <utf8proc.h>

#include "../wcwidth9.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "argument required\n");
		return 1;
	}

  for (utf8proc_int32_t c = 0; c < 0x110000; c++) {
    if (wcwidth9_intable(wcwidth9_ambiguous, WCWIDTH9_ARRAY_SIZE(wcwidth9_ambiguous), c)) {
      // skip all known ambiguous
      continue;
    }

    int w = wcwidth9(c);
    int u = utf8proc_charwidth(c);

    if (w == -1 && u == 0) {
      // both agree non-printable
      continue;
    }

    if (w == -1 && u == 2) {
      // 3 of these, all verified
      continue;
    }

    if (w == 1 && u == 0) {
      // 126 of these, all verified
      continue;
    }

    if (w == 1 && u == 2) {
      // 9011 of these, differ from utf8proc
      continue;
    }

		utf8proc_uint8_t cstr[4] = {0, 0, 0, 0};
		utf8proc_encode_char(c, cstr);

    if (w != u) {
      printf("d: '%s' c: 0x%06x, wcwidth9: %d, utf8proc: %d, category: %s\n", cstr, c, w, u, utf8proc_category_string(c));
    }
  }

	char *str = argv[1];
	size_t len = strlen(str);
	while (str < argv[1]+len) {
		utf8proc_int32_t rune;
		const utf8proc_ssize_t size = utf8proc_iterate((utf8proc_uint8_t*)str, (utf8proc_ssize_t)strlen(str), &rune);
		const int uwidth = utf8proc_charwidth(rune);
    const int width = wcwidth9(rune);

		utf8proc_uint8_t cstr[4] = {0, 0, 0, 0};
		// const utf8proc_ssize_t written = utf8proc_encode_char(rune, cstr);
		utf8proc_encode_char(rune, cstr);
		printf("rune '%s', wcwidth9: %d, utf8proc: %d, wcwidth: %2d, category: %s, hex: 0x%x\n",
        cstr,
        width,
        uwidth,
        wcwidth(rune),
        utf8proc_category_string(rune),
        rune);
		str += size;
	}
}
