/********************************
EX2 in C programing 67315
Ben Tsayeg
207014929
***********************************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define ARGMSG "USAGE: <you need to insert one of those: "\
"'best','bubble' or 'quick'>\n"
#define NUMOFSTUDENTSMSG "ERROR: num of studnts "\
"should be a number\n"
#define IDMSG "ERROR: Id must have 10 digits and "\
"can not start with 0.\n"
#define GRADEMSG "ERROR: grade should be only a integer "\
"between 0 to 100 (include)\n"
#define AGEMSG "ERROR: Age should be an integer "\
"between 18 and 120 (includes).\n"
#define ASK_FOR_STUDENT "Enter student info. Then enter\n"
#define ASK_FOR_AMOUNT "Enter number of students. Then enter\n"
#define BEST "best"
#define BUBBLE "bubble"
#define QUICK "quick"
#define SSCANF_FORMAT "%[^,],%[^,],%[^,]"
#define RETURN_STUDENT_FORMAT "best student info is: %ld,%d,%d\n"
#define PRINT_FORMAT "%ld,%d,%d\n"
#define LINE_LENGTH 60
#define FIELD_LENGTH 20
#define ID_LENGTH 10
#define MAX_AGE 120
#define MIN_AGE 18
#define MAX_GRADE 100
#define MIN_GRADE 0
#define STRTOL_FORMAT 10

/**
* struct that represent HUJI student
*/
typedef struct Student {
    long int ID;
    int Grade;
    int Age;
} Student;

///**
//*  this function swap between 2 sturct elements of array
// *  @param:2 struct
//*/
void swap (Student *A, Student *B)
{
  //this function swap between 2 sturct elements of array
  Student temp = *A;
  *A = *B;
  *B = temp;
}
///**
// * this helper for quick sort
// */
Student *partition (Student *start, Student *end)
{
  Student *pivot = end;
  Student *i = start - 1;
  Student *j = start;
  for (; j <= end - 1; j++)
    {
      if (j->Age < pivot->Age)
        {
          i++;
          swap (i, j);
        }
    }
  swap (i + 1, end);
  return (i + 1);
}
/**
 * this function sort a array by alphabetic order
 */
void quick_sort (Student *start, Student *end)
{
  if (start < end)
    {
      Student *pivot = partition (start, end);
      quick_sort (start, pivot - 1);
      quick_sort (pivot + 1, end);
    }
}

void bubble_sort (Student *start, Student *end)
{
  int length = (int) (end - start);
  Student *j;
  for (int i = 0; i < length; i++)
    // Last i elements are already in place
    for (j = start + 1; j < end - i; j++)
      {
        if ((j - 1)->Grade > (j->Grade))
          {
            swap (j - 1, j);
          }
      }
}
void best_student (Student *start, Student *end)
{
  int best_index = 0;
  float max_val = (float) (start->Grade) /
                  (float) (start->Age);
  float temp_val;
  Student *p = start;
  int index = 0;
  for (; p < end; p++)
    {
      temp_val = (float) (p->Grade) /
                 (float) (p->Age);
      if (temp_val > max_val)
        {
          max_val = temp_val;
          best_index = index;
        }
      index++;
    }
  printf (RETURN_STUDENT_FORMAT,
          (start + best_index)->ID,
          (start + best_index)->Grade,
          (start + best_index)->Age);
}
void print_student (Student *start, Student *end)
{
  Student *p = start;
  for (; p < end; p++)
    {
      printf (PRINT_FORMAT, p->ID, p->Grade, p->Age);
    }
}
/**
 *
 * @param argc
 * @param argv
 * @return manage information about HUJI students
 */
int main (int argc, char *argv[])
{
  //argument valid check
  if (argc != 2)
    {
      printf (ARGMSG);
      return EXIT_FAILURE;
    }
  if (strcmp (argv[1], BEST) != 0 &&
      strcmp (argv[1], BUBBLE) != 0
      && strcmp (argv[1], QUICK) != 0)
    {
      printf (ARGMSG);
      return EXIT_FAILURE;
    }

  char temp[LINE_LENGTH];
  long int studnts_amount;
  int flag = 1;
  while (flag == 1)
    {
      flag = 0;
      printf (ASK_FOR_AMOUNT);
      if (fgets (temp, LINE_LENGTH + 1, stdin) == NULL)
        {
          return EXIT_FAILURE;
        }

      int size_temp = (int) (strlen (temp));
      if ((temp[0] == '0' && size_temp == 2))
        {
          printf (NUMOFSTUDENTSMSG);
          flag = 1;
          continue;
        }
      for (int i = 0; i < size_temp - 1; i++)
        {
          if (!((temp[i] >= '0') && (temp[i] <= '9')))
            {
              printf (NUMOFSTUDENTSMSG);
              flag = 1;
              break;
            }
        }
    }
  char *eptr = NULL;
  studnts_amount = (long int) strtol
      (temp, (char **) eptr, STRTOL_FORMAT);
  Student *students_array = malloc
      (studnts_amount * sizeof (Student));
  if (students_array == NULL)
    {
      return EXIT_FAILURE;
    }
  char personal_info[LINE_LENGTH];
  char id_string[FIELD_LENGTH];
  char grade_string[FIELD_LENGTH];
  char age_string[FIELD_LENGTH];
  long int the_id;
  int the_grade, the_age;
  int counter = 0;
  int helper_flag;
  while (counter < studnts_amount)
    {
      helper_flag = 0;
      printf (ASK_FOR_STUDENT);
      if (fgets (personal_info,
                 LINE_LENGTH + 1, stdin) == NULL)
        {
          free (students_array);
          students_array = NULL;
          return EXIT_FAILURE;
        }
      if (sscanf (personal_info, SSCANF_FORMAT, id_string,
                  grade_string, age_string) == EOF)
        {
          free (students_array);
          students_array = NULL;
          return EXIT_FAILURE;
        }
      //Checking the correctneass of the input
      //ID
      int id_string_size = (int) (strlen (id_string));
      for (int i = 0; i < id_string_size; i++)
        {
          if (!((id_string[i] >= '0') && (id_string[i] <= '9')))
            {
              helper_flag = 1;
              break;
            }
        }
      if (helper_flag == 1 || id_string_size != ID_LENGTH ||
          id_string[0] == '0')
        {
          printf (IDMSG);
          continue;
        }
      the_id = (long int) strtol (id_string,
                                  (char **) eptr, STRTOL_FORMAT);
      //GRADE
      int grade_string_size = (int) (strlen (grade_string));
      for (int i = 0; i < grade_string_size; i++)
        {
          if (!((grade_string[i] >= '0') &&
                (grade_string[i] <= '9')))
            {
              helper_flag = 1;
              break;
            }
        }
      the_grade = (int) strtol (grade_string,
                                (char **) eptr, STRTOL_FORMAT);
      if (helper_flag == 1 || the_grade < MIN_GRADE ||
          the_grade > MAX_GRADE)
        {
          printf (GRADEMSG);
          continue;
        }
      //AGE
      int age_string_size = (int) (strlen (age_string));
      for (int i = 0; i < age_string_size - 1; i++)
        {
          if (!((age_string[i] >= '0') && (age_string[i] <= '9')))
            {
              helper_flag = 1;
              break;
            }
        }
      the_age = (int) strtol (age_string,
                              (char **) eptr, STRTOL_FORMAT);
      if (helper_flag == 1 || the_age < MIN_AGE || the_age > MAX_AGE)
        {
          printf (AGEMSG);
          continue;
        }
      //put the new student in the array
      Student new_one = {the_id, the_grade, the_age};
      students_array[counter] = new_one;
      counter++;
    }
  Student *end = students_array + studnts_amount;
  if (strcmp (argv[1], BEST) == 0)
    {
      best_student (students_array, end);
    }
  else if (strcmp (argv[1], QUICK) == 0)
    {
      quick_sort (students_array, end - 1);
      print_student (students_array, end);
    }
  else if (strcmp (argv[1], BUBBLE) == 0)
    {
      bubble_sort (students_array, end);
      print_student (students_array, end);
    }

  free (students_array);
  students_array = NULL;
  return 0;
  //free all the memory
}