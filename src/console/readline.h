#ifndef _MRT_CONSOLE_READLINE_H_
#define _MRT_CONSOLE_READLINE_H_ 1

#include <mrt/console/io.h>

namespace mrt {
namespace io {

inline std::string readLine(const std::string& prompt, std::vector<std::string>& history) {
  auto termSettings = getTerminalSettings();
  setOneCharRead(true);
  setEcho(false);

  history.push_back("");

  int hIndex = history.size() - 1; // history index - index into history
  int sIndex = 0; // string index - index into curent line

  bool read = true;

  printf("%s", prompt.c_str());
  while (read) {
    Character ch = getChar();
    switch (ch.code) {
      case UP_ARROW: {
        if (hIndex >= 0) {
          printf("\r%s%s", prompt.c_str(), history[--hIndex].c_str());
          sIndex = history[hIndex].size();
          moveCursorInLine(prompt.size() + sIndex + 1);
        } else {
          putchar(BELL);
        }
        break;
      }
      case DOWN_ARROW: {
        if (hIndex < history.size()) {
          printf("\r%s%s", prompt.c_str(), history[--hIndex].c_str());
          sIndex = history[hIndex].size();
          moveCursorInLine(prompt.size() + sIndex + 1);
        } else {
          putchar(BELL);
        }
        break;
      }
      case LEFT_ARROW: {
        if (hIndex != history.size()-1) {
          history[history.size()-1] = history[hIndex];
          hIndex = history.size()-1;
        }
        // if (ch.flags & CTRL) {}
        if (sIndex > 0) {
          sIndex--;
          moveCursorLeft();
        } else {
          putchar(BELL);
        }
        break;
      }
      case RIGHT_ARROW: {
        if (hIndex != history.size()-1) {
          history[history.size()-1] = history[hIndex];
          hIndex = history.size()-1;
        }
        // if (ch.flags & CTRL) {}
        if (sIndex < history[hIndex].size()) {
          sIndex++;
          moveCursorRight();
        } else {
          putchar(BELL);
        }
        break;
      }
      case '\b':
      case DEL: {
        if (hIndex != history.size()-1) {
          history[history.size()-1] = history[hIndex];
          hIndex = history.size()-1;
        }
        if (sIndex == 0) {
          putchar(BELL);
        } else {
          clearLine();
          history[hIndex].erase(history[hIndex].begin() + --sIndex);
          printf("\r%s%s", prompt.c_str(), history[hIndex].c_str());
          moveCursorInLine(prompt.size() + sIndex + 1); // Put cursor to sIndex
        }
        break;
      }
      case '\n': {
        moveCursorInLine(history[hIndex].size());
        putchar('\n');
        goto end;
      }
      default: {
        if (hIndex != history.size()-1) {
          history[history.size()-1] = history[hIndex];
          hIndex = history.size()-1;
        }
        history[hIndex].insert(history[hIndex].begin() + sIndex++, ch.code);
        printf("\r%s%s", prompt.c_str(), history[hIndex].c_str());
      }
    }
  }

end:
  setEcho(true);
  setTerminalSettings(termSettings);
  if (hIndex != history.size() - 1) {
    history.push_back(history[hIndex]);
  }
  return history.back();
}

} /* namespace io */
} /* namespace mrt */

#endif /* _MRT_CONSOLE_READLINE_H_ */