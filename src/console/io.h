#ifndef _MRT_CONSOLE_IO_H_
#define _MRT_CONSOLE_IO_H_ 1

#include <string>
#include <vector>
#include <cstdio>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <termios.h>

namespace mrt {
namespace io {

// Character Flags
constexpr int CTRL   = 1;
constexpr int SHIFT  = 2;
constexpr int ALT    = 4;
constexpr int OPTION = ALT;

// Character Codes
constexpr int ESC         = 27;
constexpr int DEL         = 127;
constexpr int BELL        = 7;
constexpr int UP_ARROW    = 512;
constexpr int DOWN_ARROW  = 513;
constexpr int RIGHT_ARROW = 514;
constexpr int LEFT_ARROW  = 515;

struct Character {
  int code;
  int flags;
};

Character getChar();

namespace impl {

inline Character decodeArrows(int ch, int flags) {
  switch (ch) {
    case 'A': return {UP_ARROW, flags};
    case 'B': return {DOWN_ARROW, flags};
    case 'C': return {RIGHT_ARROW, flags};
    case 'D': return {LEFT_ARROW, flags};
    default:  return {ch, flags};
  }
}

inline Character decodeEscapeSequence(int ch) {
  switch (ch) {
    default:  return decodeArrows(ch, 0);
    case 'Z': return {'\t', SHIFT};
    case '1': {
      ch = getchar();
      if (ch == ';') {
        ch = getchar();
        switch (ch) {
          case '2': return decodeArrows(getchar(), SHIFT);
          case '3': return decodeArrows(getchar(), ALT);
          case '4': return decodeArrows(getchar(), SHIFT | ALT);
          case '5': return decodeArrows(getchar(), CTRL);
          case '6': return decodeArrows(getchar(), CTRL | SHIFT);
          case '7': return decodeArrows(getchar(), CTRL | ALT);
          case '8': return decodeArrows(getchar(), CTRL | SHIFT | ALT);
          default:  return {ch}; // TODO: Find list of combinations
        }
      }
    }
  }
  return {ch};
}

} /* namespace impl */

inline termios getTerminalSettings() {
  termios settings;
  tcgetattr(STDIN_FILENO, &settings);
  return settings;
}

inline void setTerminalSettings(termios settings) {
  tcsetattr(STDIN_FILENO, TCSANOW, &settings);
}

inline void setOneCharRead(bool flag) {
  termios settings = getTerminalSettings();

  if (flag) {
    settings.c_lflag &= ~ICANON;
  } else {
    settings.c_lflag |= ICANON;
  }
  tcsetattr(STDIN_FILENO, TCSANOW, &settings);
}

inline void setEcho(bool flag) {
  termios settings = getTerminalSettings();

  if (flag) {
    settings.c_lflag |= ECHO;
  } else {
    settings.c_lflag &= ~ECHO;
  }
  tcsetattr(STDIN_FILENO, TCSANOW, &settings);
}

inline void showCursor(bool flag) {
  if (flag) {
    fputs("\e[?25h", stdout);
  } else {
    fputs("\e[?25l", stdout);
  }
}

inline void clearLine() {
  printf("\e[2K");
}

inline void clearScreen() {
  printf("\e[2K");
}

inline void moveCursorInLine(int offset) {
  printf("\e[%dG", offset);
}

inline void moveCursor(int rows, int cols) {
  printf("\e[%d;%dH", rows, cols);
}

inline void moveCursorUp(int n = 1) {
  printf("\e[%dA", n);
}

inline void moveCursorDown(int n = 1) {
  printf("\e[%dB", n);
}

inline void moveCursorRight(int n = 1) {
  printf("\e[%dC", n);
}

inline void moveCursorLeft(int n = 1) {
  printf("\e[%dD", n);
}

inline Character getChar() {
  int ch = 0;
  ch = getchar();
  if (ch == ESC) {
    // FIXME: Doesn't work for some reason, for ESC to be returned, it must be pressed twice
    // Check if there is actually an escape sequence
    /*pollfd fd = {.fd = STDIN_FILENO, .events = POLLIN};
    int ret = poll(&fd, 1, 0);
    if (!ret || !(fd.revents & POLLIN)) {
      printf("\n%x %x %x\n", ret, fd.revents, fd.revents & POLLIN);
      return {ch};
    }*/

    ch = getchar();
    if (ch == '[') {
      ch = getchar();
      return impl::decodeEscapeSequence(ch);
    }
  }
  return {ch};
}

} /* namespace io */
} /* namespace mrt */

#endif /* _MRT_CONSOLE_IO_H_ */