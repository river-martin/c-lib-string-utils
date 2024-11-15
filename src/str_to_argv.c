#include "../include/str_to_argv.h"

#include "c-lib-stack/src/stack.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void push_field(vp_stack *stack, const char *start, const char *stop)
{
  unsigned long field_len = stop - start;
  char *field = malloc((field_len + 1) * sizeof(char));
  strncpy(field, start, field_len);
  field[field_len] = '\0';
  vp_stack_push(stack, (void *)field);
}

char **str_to_argv(const char *s, int *argc)
{
  vp_stack *str_stack = vp_stack_new();
  const char *field_start = s;
  const char *p;
  char *field;
  // clang-format off
  #define NONE '\0'
  // clang-format on
  char quote_delim = NONE; // \0 if not in quote
  for (p = s; *p; p++)
  {
    if (quote_delim != NONE)
    {
      if (*p == quote_delim)
      {
        const char *field_stop = ++p;
        push_field(str_stack, field_start, field_stop);
        while (*p == ' ')
          p++;
        field_start = p;
        p = p - 1; // to cancel out the increment in the for loop
        quote_delim = NONE;
        continue;
      }
      else if (*p == ' ')
      {
        continue;
      }
    }
    if (quote_delim == NONE)
    {
      if (*p == '\\')
      {
        // Skip the next character
        p++;
        continue;
      }
      if (*p == '\'' || *p == '"')
      {
        quote_delim = *p;
        field_start = p + 1;
      }
      else if (*p == ' ')
      {
        const char *field_stop = p;
        push_field(str_stack, field_start, field_stop);
        while (*p == ' ')
        {
          p++;
        }
        // Set the start of the next field
        field_start = p + 1;
      }
    }
  }
  *argc = str_stack->size;
  char **argv = malloc(*argc * sizeof(char *));
  int i = *argc;
  while ((field = vp_stack_pop(str_stack)))
  {
    argv[--i] = field;
  }

  assert(i == 0);

  // clang-format off
  #undef NONE
  // clang-format on

  return argv;
}
