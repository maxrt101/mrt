#ifndef _MRT_CONSOLE_IO_H_
#define _MRT_CONSOLE_IO_H_ 1

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

namespace mrt {
namespace io {

constexpr int ESC = 27;
constexpr int LEFT_ARROW = 512;
constexpr int UP_ARROW = 513;
constexpr int RIGHT_ARROW = 514;
constexpr int DOWN_ARROW = 515;

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

void showCursor(bool flag) {
  if (flag) {
    fputs("\e[?25h", stdout);
  } else {
    fputs("\e[?25l", stdout);
  }
}

inline int getChar() {
  int ch = 0;
  ch = getchar();
  if (ch == ESC) {
    ch = getchar();
    if (ch == '[') {
      ch = getchar();
      switch (ch) {
        case 'A': return UP_ARROW;
        case 'B': return DOWN_ARROW;
        case 'C': return RIGHT_ARROW;
        case 'D': return LEFT_ARROW;
        default:  return ch;
      }
    }
  }
  return ch;
}

} /* namespace io */
} /* namespace mrt */

#endif /* _MRT_CONSOLE_IO_H_ */