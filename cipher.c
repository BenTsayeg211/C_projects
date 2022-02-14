#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define ARGERR "Usage: cipher <encode|decode> <k> "\
"<source path file> <output path file>\n"
#define CHECK_ERROR "Usage: cipher <check> <source path file>"\
" <output path file>\n"
#define COMERR "The given command is invalid\n"
#define KERR "The given shifts value is invalid\n"
#define FILEERR "The given file is invalid\n"
#define WRONG_CHECK "Invalid encrypting\n"
#define VALID_CHECK_RET "Valid encrypting with k = %d\n"
#define DECODE "decode"
#define ENCODE "encode"
#define CHECK "check"
#define READ_MODE "r"
#define WRITE_MODE "w"
#define ARGAMOUNT (5)
#define MODULOVAL (26)
#define LITTLEA (97)
#define BIGA (65)

/**
 *
 * @param ch value to "move"
 * @param command -encode to decode
 * @param k parameter to move
 * @return make a cipher move as asked
 */
int shift_char (int ch, char command[], int k)
{
  /**
   * this function make the change on every char in the input file
   */
  int d;
  if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')))
    {
      return ch;
    }
  if (strcmp (command, ENCODE) == 0)
    {
      if ((ch >= 'A' && ch <= 'Z'))
        {
          ch = (((((ch) - BIGA) + k) % MODULOVAL) + BIGA);
        }
      else
        {
          ch = (((((ch) - LITTLEA) + k) % MODULOVAL) + LITTLEA);
        }
    }
  else if ((ch >= 'A' && ch <= 'Z'))
    { ;
      d = -k;
      while (d < 0)
        {
          d += MODULOVAL;
        }
      ch = (((((ch) - BIGA) + d) % MODULOVAL) + BIGA);
    }
  else
    {
      d = -k;
      while (d < 0)
        {
          d += MODULOVAL;
        }
      ch = (((((ch) - LITTLEA) + d) % MODULOVAL) + LITTLEA);
    }
  return ch;
}
/**
 *
 * @param one -path to file1
 * @param two -path to file 2
 * @return check is this file have a f-zikli movement
 */
int check_exec (char one[], char two[])
{
  FILE *first;
  first = fopen (one, READ_MODE);
  if (first == NULL)
    {
      fprintf (stderr, FILEERR);
      return EXIT_FAILURE;
    }
  FILE *second;
  second = fopen (two, "r");
  if (second == NULL)
    {
      fprintf (stderr, FILEERR);
      fclose (first);
      return EXIT_FAILURE;
    }
  char cur_ch1 = (char) (fgetc (first));
  char cur_ch2 = (char) (fgetc (second));
  while (cur_ch1 == cur_ch2 && isalpha(cur_ch1) == 0)
    {
      cur_ch1 = (char) (fgetc (first));
      cur_ch2 = (char) (fgetc (second));
    }
  if (cur_ch1 != cur_ch2 && isalpha(cur_ch1) == 0)
    {
      printf (WRONG_CHECK);
      return EXIT_SUCCESS;
    }
  int difference = (cur_ch2 - cur_ch1) % MODULOVAL;
  int flag = 0;
  cur_ch1 = (fgetc (first));
  cur_ch2 = (fgetc (second));
  while (cur_ch1 != -1 && cur_ch2 != -1)
    {
      if (isalpha(cur_ch1) != 0)
        {
          if (shift_char (cur_ch1, ENCODE, difference) != cur_ch2)
            {
              flag = 1;
              break;
            }
        }
      cur_ch1 = (char) (fgetc (first));
      cur_ch2 = (char) (fgetc (second));
    }
  if (cur_ch1 != EOF || cur_ch2 != EOF || flag == 1)
    {
      printf (WRONG_CHECK);
    }
  else
    {
      printf (VALID_CHECK_RET, difference);
    }
  fclose (first);
  fclose (second);
  return (EXIT_SUCCESS);
}
/**
 *
 * @param argc
 * @param argv
 * @return gets a command a file and run the cipher.c as asked.
 */
int main (int argc, char *argv[])
{
  /**
   * valid check on arguments
   */
  if (argc == 1)
    {
      fprintf (stderr, COMERR);
      return EXIT_FAILURE;
    }
  if (strcmp (argv[1], CHECK) == 0)
    {
      if (argc != ARGAMOUNT - 1)
        {
          fprintf (stderr, CHECK_ERROR);
          return EXIT_FAILURE;
        }
      return (check_exec (argv[2], argv[3]));
    }

  if ((strcmp (argv[1], ENCODE) == 0) || (strcmp (argv[1], DECODE) == 0))
    {
      if (argc != ARGAMOUNT)
        {
          fprintf (stderr, ARGERR);
          return EXIT_FAILURE;
        }
      int k = atoi (argv[2]);
      FILE *input;
      input = fopen (argv[3], READ_MODE);
      if (input == NULL)
        {
          fprintf (stderr, FILEERR);
          return EXIT_FAILURE;
        }
      FILE *output;
      output = fopen (argv[4], WRITE_MODE);
      if (output == NULL)
        {
          fprintf (stderr, FILEERR);
          return EXIT_FAILURE;
        }
      char cur_ch;
      while ((cur_ch = (char) (fgetc (input))) != EOF)
        {
          int ch = ((int) (cur_ch));
          if (k < 0)
            {
              if (strcmp (argv[1], ENCODE) == 0)
                {
                  ch = shift_char (ch, DECODE, (-1 * k));
                }
              else
                {
                  ch = shift_char (ch, ENCODE, (-1 * k));
                }
            }
          else
            {
              ch = shift_char (ch, argv[1], k);
            }
          cur_ch = ((char) (ch));
          fputc (cur_ch, output);
        }
      fclose (input);
      fclose (output);
      return EXIT_SUCCESS;
    }
  else
    {
      fprintf (stderr, COMERR);
      return EXIT_FAILURE;
    }
}


