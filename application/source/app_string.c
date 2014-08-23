

#include "app_string.h"

/* reverse:  reverse string s in place
  */
static void reverse(
    char *                s,
    size_t                len)
{
    size_t                i;
    size_t                j;
    char                  c;

    for (i = 0u, j = len - 1u; i < j; i++, j--)
    {
        c    = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

size_t nstrcpy(char * dst, const char * src) {
    size_t                      retval;

    retval = 0u;

    while (*src) {
        *dst++ = *src++;
        retval++;
    }

    return (retval);
}


size_t sprintUint32(
  char *                buff,
  uint32_t              num)
{
  size_t                i;

  i = 0;

  do
  {                                                                             /* generate digits in reverse order                         */
    buff[i++] = num % 10 + '0';                                                 /* get next digit                                           */
  }
  while ((num /= 10) > 0);                                                      /* delete it                                                */

  reverse(
    buff,
    i);

  return (i);
}
