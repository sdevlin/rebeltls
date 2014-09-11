#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "bdlang.h"
#include "types.h"

static struct bdprog *prog_list;

static int accept(const char **srcp, int c)
{
  if (**srcp == c) {
    *srcp += 1;
    return 1;
  } else {
    return 0;
  }
}

static void expect(const char **srcp, int c)
{
  assert(**srcp == c);
  *srcp += 1;
}

static void eatspace(const char **srcp)
{
  while (accept(srcp, ' ') == 1) {
    /* loop */
  }
}

static int isinttype(int c)
{
  switch (c) {
  case 'b':
  case 'B':
  case 'h':
  case 'H':
  case 'l':
  case 'L':
  case 'q':
  case 'Q':
    return 1;
  default:
    return 0;
  }
}

static uint64 readint(const char **srcp)
{
  const char *src;
  uint64 val;
  int c;

  src = *srcp;
  val = 0;
  for (c = *src; isdigit(c); src += 1, c = *src) {
    val *= 10;
    val += c - '0';
  }

  *srcp = src;
  return val;
}

static void readarray(const char **srcp, struct bdcmd *cmdp)
{
  cmdp->cmdtype = TYPE_ARRAY;
  eatspace(srcp);
  expect(srcp, '[');
  eatspace(srcp);
  cmdp->coef.val = readint(srcp);
  eatspace(srcp);
  expect(srcp, ']');
}

static void readvector(const char **srcp, struct bdcmd *cmdp)
{
  cmdp->cmdtype = TYPE_VECTOR;
  eatspace(srcp);
  expect(srcp, '<');
  eatspace(srcp);
  cmdp->coef.rng.min = readint(srcp);
  eatspace(srcp);
  expect(srcp, '.');
  expect(srcp, '.');
  eatspace(srcp);
  cmdp->coef.rng.max = readint(srcp);
  eatspace(srcp);
  expect(srcp, '>');
}

static void readcmd(const char **srcp, struct bdcmd *cmdp)
{
  const char *src;
  int c;

  src = *srcp;
  eatspace(&src);

  c = *src;
  if (!isinttype(c)) {
    /* TODO print error message */
    exit(1);
  }

  cmdp->inttype = c;
  src += 1;
  eatspace(&src);

  c = *src;
  if (c == '[') {
    readarray(&src, cmdp);
  } else if (c == '<') {
    readvector(&src, cmdp);
  } else {
    cmdp->cmdtype = TYPE_REGISTER;
    if (isdigit(c)) {
      cmdp->coef.val = readint(&src);
    } else {
      cmdp->coef.val = 1;
    }
  }

  *srcp = src;
}

static struct bdprog *lookup(const char *src)
{
  struct bdprog *prog;

  for (prog = prog_list; prog != NULL; prog = prog->next) {
    if (prog->src == src) {
      return prog;
    }
  }

  return NULL;
}

const struct bdprog *bdlang_compile(const char *src)
{
  struct bdprog *prog;
  struct bdcmd *cmd;

  prog = lookup(src);
  if (prog != NULL) {
    return prog;
  }

  prog = malloc(sizeof *prog);
  prog->src = src;
  prog->next = prog_list;
  prog_list = prog;

  eatspace(&src);

  if (accept(&src, '<')) {
    prog->endian = ENDIAN_LITTLE;
  } else if (accept(&src, '>') || accept(&src, '!')) {
    prog->endian = ENDIAN_BIG;
  } else if (accept(&src, '=') || 1) {
    prog->endian = ENDIAN_NATIVE;
  }

  prog->ncmds = 0;
  prog->cmds = malloc(strlen(src) * (sizeof (struct bdcmd)));

  for (cmd = prog->cmds; *src != '\0'; cmd += 1, prog->ncmds += 1) {
    readcmd(&src, cmd);
  }

  prog->cmds = realloc(prog->cmds, prog->ncmds * (sizeof (struct bdcmd)));

  return prog;
}
