/* Interpreter for Malbolge.                                          */
/* '98 Ben Olmstead.                                                  */
/*                                                                    */
/* Malbolge is the name of Dante's Eighth circle of Hell.  This       */
/* interpreter isn't even Copylefted; I hereby place it in the public */
/* domain.  Have fun...                                               */
/*                                                                    */
/* Note: in keeping with the idea that programming in Malbolge is     */
/* meant to be hell, there is no debugger.                            */
/*                                                                    */
/* By the way, this code assumes that short is 16 bits.  I haven't    */
/* seen any case where it isn't, but it might happen.  If short is    */
/* longer than 16 bits, it will still work, though it will take up    */
/* considerably more memory.                                          */
/*                                                                    */
/* If you are compiling with a 16-bit Intel compiler, you will need   */
/* >64K data arrays; this means using the HUGE memory model on most   */
/* compilers, but MS C, as of 8.00, possibly earlier as well, allows  */
/* you to specify a custom memory-model; the best model to choose in  */
/* this case is /Ashd (near code, huge data), I think.                */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
#include <string.h>

#ifdef __GNUC__
static inline
#endif
void exec(unsigned short *mem);

#ifdef __GNUC__
static inline
#endif
unsigned short op(unsigned short x, unsigned short y);

const char xlat1[] =
  "+b(29e*j1VMEKLyC})8&m#~W>qxdRp0wkrUo[D7,XTcA\"lI"
  ".v%{gJh4G\\-=O@5`_3i<?Z';FNQuY]szf$!BS/|t:Pn6^Ha";

const char xlat2[] =
  "5z]&gqtyfr$(we4{WP)H-Zn,[%\\3dL+Q;>U!pJS72FhOA1C"
  "B6v^=I_0/8|jsb9m<.TVac`uY*MK'X~xDl}REokN:#?G\"i@";

int main(int argc, char **argv) {
  FILE *f;
  unsigned short i = 0, j = 0;
  int x;
  unsigned short *mem;
  if (argc != 2) {
    fputs("Usage : ./malv program.mal\n", stderr);
    return (1);
  }
  if ((f = fopen(argv[1], "r")) == NULL) {
    fputs("\x1b[1m[\x1b[91mwarn\x1b[0m\x1b[1m]\x1b[0m Can't open file\n", stderr);
    return (1);
  }
#ifdef _MSC_VER
  mem = (unsigned short *)_halloc(59049, sizeof(unsigned short));
#else
  mem = (unsigned short *)malloc(sizeof(unsigned short) * 59049);
#endif
  if (mem == NULL) {
    fclose(f);
    fputs("\x1b[1m[\x1b[91mwarn\x1b[0m\x1b[1m]\x1b[0m Can't allocate memory\n", stderr);
    return (1);
  }
  printf("[LOG] Loading program in memory\n");
  while ((x = getc(f))!= EOF) {
    if (isspace(x)) continue;
    if (x < 127 && x > 32) {
      if (strchr("ji*p</vo", xlat1[(x - 33 + i) % 94]) == NULL) {
        //fputs("Invalid character encountered in source file\n", stderr);
        printf("\x1b[1m[\x1b[91mwarn\x1b[0m\x1b[1m]\x1b[0m Invalid character encountered in source file\n char position : %d\n", i);
        free(mem);
        fclose(f);
        return (1);
      }
    }

    if (i == 59049) {
      fputs("\x1b[1m[\x1b[91mwarn\x1b[0m\x1b[1m]\x1b[0m Input file too long\n", stderr);
      free(mem);
      fclose(f);
      return (1);
    }
    //printf("\x1b[1mSET\x1b[0m 0d%05d : %c\n", i, x);
    printf("SET 0d%05d : %c\n", i, x);
    mem[i++] = x;
  }
  //printf("\x1b[1m[\x1b[93mLOG\x1b[0m\x1b[1m] Program loaded\x1b[0m\n");
  printf("[LOG] Program loaded.\n\n");

  fclose(f);
  while (i < 59049) mem[i] = op(mem[i - 1], mem[i - 2]), i++;
  //printf("\x1b[1m[\x1b[93mLOG\x1b[0m\x1b[1m] Execution started\x1b[0m\n");
  printf("[LOG] Execution started\n");
  exec(mem);
  //printf("\n\x1b[1m[\x1b[93mLOG\x1b[0m\x1b[1m] Execution ended\x1b[0m\n");
  printf("\n[LOG] Execution ended\n");
  free(mem);
  return (0);
}

#ifdef __GNUC__
static inline
#endif
void exec(unsigned short *mem) {
  unsigned short a = 0, c = 0, d = 0;
  int x;
  for (;;) {
    //printf("\x1b[0m[REG]\x1b[0m [a] : %5d  |  [c] : %5d  |  [d] : %5d  \n", a, c, d);
    printf("[REG] [a] : %5d  |  [c] : %5d  |  [d] : %5d  \n", a, c, d);

    if (mem[c] < 33 || mem[c] > 126) continue;
    switch (xlat1[(mem[c] - 33 + c) % 94]) {
      case 'j': d = mem[d]; break;
      case 'i': c = mem[d]; break;
      case '*': a = mem[d] = mem[d] / 3 + mem[d] % 3 * 19683; break;
      case 'p': a = mem[d] = op(a, mem[d]); break;
      case '<':
#if '\n' != 10
        if (x == 10) putc('\n', stdout); else
#endif
        //printf("\x1b[1m[OUTPUT=>]\x1b[0m %c\n", a);
        printf("[OUTPUT=>] %c\n", a);
        break;
      case '/':
        //printf("\x1b[1m[INPUT]");
        printf("[INPUT]");
        x = getc(stdin);
        printf("\n");
#if '\n' != 10
        if (x == '\n') a = 10; else
#endif
        if (x == EOF) a = 59048; else a = x;
        break;
      case 'v': return;
    }
    mem[c] = xlat2[mem[c] - 33];
    if (c == 59048) c = 0; else c++;
    if (d == 59048) d = 0; else d++;
  }
}

#ifdef __GNUC__
static inline
#endif
unsigned short op(unsigned short x, unsigned short y) {
  unsigned short i = 0;
  unsigned short j = 0;
  static const unsigned short p9[5]   = { 1, 9, 81, 729, 6561 };
  static const unsigned short o[9][9] = {
      { 4, 3, 3, 1, 0, 0, 1, 0, 0 },
      { 4, 3, 5, 1, 0, 2, 1, 0, 2 },
      { 5, 5, 4, 2, 2, 1, 2, 2, 1 },
      { 4, 3, 3, 1, 0, 0, 7, 6, 6 },
      { 4, 3, 5, 1, 0, 2, 7, 6, 8 },
      { 5, 5, 4, 2, 2, 1, 8, 8, 7 },
      { 7, 6, 6, 7, 6, 6, 4, 3, 3 },
      { 7, 6, 8, 7, 6, 8, 4, 3, 5 },
      { 8, 8, 7, 8, 8, 7, 5, 5, 4 },
    };
  for (j = 0; j < 5; j++)
    i += o[y / p9[j] % 9][x / p9[j] % 9] * p9[j];
  return (i);
}
