#ifndef _MRT_ENUM_H_
#define _MRT_ENUM_H_ 1

#include <iostream>

/* Declares pseudo-enum and some useful member functions in it
 * Usage: in header file: call macro with 1 argument as the name of the enum,
 * and the rest if the arguments as the names of the enum fields. You can
 * define additional members by writing their definitios after the macro call,
 * but regarless, don't forget to close the brace ('};').
 */
#define MRT_DEF_ENUM(_name, ...) \
  struct _name { \
    static const OptionType __VA_ARGS__; \
    constexpr operator int() const { return m_value; } \
    constexpr bool operator==(const _name& rhs) const { \
      return m_value == rhs.m_value; \
    } \
    constexpr bool operator!=(const _name& rhs) const { \
      return m_value != rhs.m_value; \
    } \
    friend inline std::ostream& operator<<(std::ostream& os, const _name& rhs) { \
      os << rhs.m_value; \
      return os; \
    } \
   private: \
    int m_value; \
    constexpr OptionType(int value) : m_value(value) {} \
   public:

/* Defines enum's fields 
 * Should be called in source file
 */
#define MRT_DEF_ENUM_FIELDS(_name, ...) \
  constexpr const _name __VA_ARGS__

#endif
