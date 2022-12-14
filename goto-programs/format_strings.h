#ifndef CPROVER_GOTO_PROGRAMS_FORMAT_STRINGS_H_
#define CPROVER_GOTO_PROGRAMS_FORMAT_STRINGS_H_

#include <list>
#include <string>
#include <util/expr.h>
#include <util/mp_arith.h>

class format_tokent
{
public:
  typedef enum
  {
    UNKNOWN,
    TEXT,
    SIGNED_DEC,   // d, i
    UNSIGNED_OCT, // o
    UNSIGNED_DEC, // u
    UNSIGNED_HEX, // x, X
    DOUBLE_ENG,   // e, E
    DOUBLE,       // f, F
    DOUBLE_G,     // g, G
    DOUBLE_HEX,   // a, A
    CHAR,         // c
    STRING,       // s
    POINTER,      // p
    PERCENT       // %
  } token_typet;

  typedef enum
  {
    ALTERNATE,
    ZERO_PAD,
    LEFT_ADJUST,
    SIGNED_SPACE,
    SIGN,
    ASTERISK
  } flag_typet;

  typedef enum
  {
    LEN_h,
    LEN_l,
    LEN_L,
    LEN_j,
    LEN_t
  } length_modifierst;

  format_tokent(token_typet _type) : type(_type)
  {
  }
  format_tokent() : type(UNKNOWN)
  {
  }

  token_typet type;
  std::list<flag_typet> flags;
  BigInt field_width;
  BigInt precision;
  length_modifierst length_modifier;
  irep_idt value; // for text and pattern matching
};

typedef std::list<format_tokent> format_token_listt;

bool parse_format_string(
  const exprt &format_arg,
  format_token_listt &token_list);

#endif /*CPROVER_GOTO_PROGRAMS_FORMAT_STRINGS_H_*/
