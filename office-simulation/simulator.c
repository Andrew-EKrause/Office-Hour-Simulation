/*
 * Authors: Samantha Foley and Andrew Krause
 * Date:   12/11/2020
 * ==============================================================================
 * General Description:
 *
 * Program to simulate office hours. The program runs a simulation of office
 * hours using two types of algorithms. The first algorithm is a FIFO implementation
 * of the attributes related to scheduling office hours as well as the reasons for
 * visiting the professor. The second implementation, a Priority algorithm, uses
 * the same components in the program as FIFO. However, the Priority program takes
 * a certain visit type (advising, help, or a project demo) and places that visit
 * before all others. The program first reads in data from a text file with
 * information relating to an office hour schedule for the week. The file includes
 * professor availability, time needed for student visits, student availability,
 * and the visit type. The program then creates a list of students through the
 * init_day function, which is then utilized in the simulate_day function. The
 * simulate_day function uses the list created in the init_day function to simulate
 * the weekdays where the professor meets with students for advising, help, or
 * a project demo. If the instructor does not have a sufficient amount of time to
 * help a student on a given day, that student uses the rest of their visiting time
 * with the professor on their next available day. A summary of the visits and
 * final statistics are printed out at the completion of the program.
 * ==============================================================================
 * FIFO and Priority Descriptions:
 *
 * The FIFO algorithm ueses the programming principle of first-in-first out. The
 * first student to sign up for an office hour visit has priority. In the case of
 * the FIFO implementation, the reasons for visiting the professor are disregarded.
 * In the execution of the program, students are added to a linked list regardless
 * of the reasons for visiting the instructor, which are for advising, help, or
 * completing a project demo. On the other hand, The Priority algorithm enables
 * the professor to select or prioritize which students will visit first based on
 * the reason for that visit. For example, if the instructor prioritizes project
 * demos, then students who are presenting projects will be placed at the start of
 * the linked list for calculating office hour visits before all other types of
 * student visits. The Priority operates on a daily basis. Both algorithms have
 * advantages and disadvantages in this implementation.
 * ==============================================================================
 * Advantages and Disadvantages of Priority-Based Algorithm:
 *
 * The priority-based algorithm is useful for certain aspects of scheduling office
 * hours in this program. Depending on the situation, the process is advantageous
 * in that the professor can decide which visit type is first. For example, if
 * the situation is finals week and certain students need help with a part of the
 * course curriculum before their exam, the instructor can prioritize questions.
 * By prioritizating questions, the professor improves the likelihood that those
 * students who need help will receive assistance before deadlines or other
 * potential complications arise. However, the priority-based algorithm also
 * has disadvantages. After conducting research, I found a website called
 * https://www.geeksforgeeks.org/starvation-and-aging-in-operating-systems/. On
 * the site, the problem of starvation was discussed in detail. In the office hour
 * scheduling program, depending on the situation, students can potentially wait
 * for a significant amount of time if their visit type has a low priority. This
 * could prove to be a major inconveniences for students if the priority consists
 * of a larger number of students with a specific visit type. Overall, the
 * advantages and disadvantages of the priority-based algorithm are contingent upon
 * the situation or scenario that the algorithm is applied to in implementation.
 * ==============================================================================
 * Algorithm Preference:
 *
 * After conducting research and tests on the two different algorithms, FIFO and
 * Priority, I concluded that the FIFO process is best for this particular proble
 * of office hour scheduling. In my research, which I mention part of in the above
 * paragraph, I found that FIFO has less of a risk causing a process called
 * starvation in algorithmic operations. An example of FIFO is in tracking certain
 * manufacturing operations. For cost flow assumptions, a created item that
 * continues on through stages of development before being sold is recognized with
 * an expense. The cost of inventory that is bought first is recognzied first.
 * Therefore, FIFO is used in asset-management to efficiently track the profit of
 * the product being sold by immediately factoring in the cost to produce the
 * product. I also conducted tests on the algorithms in order to observe the total
 * time that students spent waiting to visit with the professor.
 *
 * The tests were conducted for the different scenarios: FIFO, Priority Question,
 * Priority Help, and Priority Advising. I gave eight students the same amount of
 * availability for the tests as well as made the availability of the time of the
 * professor constant. I varied the reasons the students were visiting as well as
 * the time they needed to visit. After completing the tests, I found that FIFO
 * outperformed Priority for the majority of cases. When FIFO was used, there was
 * a total waiting time of 649 minutes. When the priority was used for questions,
 * the total waiting time 603 minutes. When the priority was used for advising,
 * the total waiting time was 731 minutes. When the priority was used for demos,
 * the total waiting time was 672 minutes. In this case, the majority of students
 * had questions for the week. I concluded that the FIFO algorithm works best for
 * this program of office hour scheduling given that, overall, the process was more
 * effiecient in mitigating the amount of time that students need to wait for a
 * visit with the professor compared to the priority-based algorithm.
 * ==============================================================================
 *
 */
#include <string.h>
#include <ctype.h>
#include "stu_list.h"

/* The definition reserves space for the strings entered into the program */
#define LINELEN 1024

/* globals */
int num_students;
professor prof;         /* struct containing information about the professor */
student * all_students; /* dynamically allocated array of all of the students */
sched_t priority_mode;
int rr_quantum;

/* counters for final stats */
int tot_waiting = 0;
int tot_done = 0;
int tot_visit_min = 0;

/* function prototypes */
void read_data(FILE * infile);
stu_list * init_day(days_t today);
void cleanup_list(stu_list * today);
void simulate(stu_list * today, days_t day);
int is_all_done();

/* given print and helper functions -- DON'T CHANGE THESE*/
void print_final_stats();
void print_init_stats();
void print_state();
void print_daily_stats(days_t d, int visits, int v_mins, int waiting, int wait_mins);
void correct_usage_message();
char * day_to_string(days_t d);
char * type_to_string(visit_t t);

/*
 * The main method runs the functions within the
 * program to complete the office hour simulation.
 */
int main(int argc, char** argv)
{
  /* 3 functions to write; call in main method */
  /* init lists and other data structures */
  FILE * infile = NULL;  /* file descriptor for file from user */
  #ifdef FINAL
  stu_list * daily = NULL; /* list of students that are visiting today */
  days_t day; /* current day */
  #endif

  /* parse command-line arguments
   *    -f : FIFO (no priority)
   *    -a : advising takes priority
   *    -q : questions have priority
   *    -d : demos have priority
   *    file : the file to read in
   */

  /* if number of arguments is less than three, print out error message */
  if(argc != 3)
  {
    printf("Incorrect number of arguments.\n");
    correct_usage_message();
    return -1;
  }

  /* if the first argument is -f, the algorithm to use is a FIFO */
  if(strcmp(argv[1], "-f") == 0)
  {
    priority_mode = FIFO;
  }
  /* if first argument is -q, algorithm to use prioritizes questions */
  else if(strcmp(argv[1], "-q") == 0)
  {
    priority_mode = PRI_Q;
  }
  /* if first argument is -a, algorithm to use prioritizes appointments */
  else if(strcmp(argv[1], "-a") == 0)
  {
    priority_mode = PRI_A;
  }
  /* if first argument is -d, algorithm to use prioritizes demos */
  else if(strcmp(argv[1], "-d") == 0)
  {
    priority_mode = PRI_D;
  }
  /* if the argument flag is incorrect, print out error message */
  else
  {
    printf("Incorrect flag for algorithm.\n");
    correct_usage_message();
    return -1;
  }

  /* open the file in order to read in contents for scheduling */
  infile = fopen(argv[2], "r");

  /* if the file is empty or corrupted, print out error message */
  if(infile == NULL)
  {
    printf("Unable to open file.\n");
    correct_usage_message();
    return -1;
  }
  else
  {
    /* othewise, read in the data */
    read_data(infile);
  }

  /* close the file that was opened to read in the schedule data */
  fclose(infile);

  /* print initial stats */
  print_init_stats();

  /******* Deliverable 1 ends here *******/

  #ifdef FINAL
  /* Deliverable 2: remainder of code should also work to simulate the office hours.
   * You will also need to write about the different algorithms. */

  /* Initialize the day to Monday */
  day = MONDAY;

  /* while there are still students who want to visit... */
  while(is_all_done() != 0)
  {
    /* for each day, init day with the students who can visit today */
    daily = init_day(day);

    /* simulate day */
    simulate(daily, day);

    /* get ready for the next day */
    cleanup_list(daily);
    free(daily);
    daily = NULL;
    day = (day + 1) % 5;
  }

  /* print final stats */
  print_final_stats();

  /* free all data structures */
  free(all_students);
  /*********** End Deliverable 2 *********/
  #endif

  /* clean exit of the program */
  return 0;
}

/*
 * Read in the data from the file infile.  Assume that infile has been opened
 * for reading and is not NULL.
 */
void read_data(FILE * infile)
{
  /* counter variables and a buffer for analyzing file content are created */
  int i = 0;
  int j = 0;
  char buffer[ LINELEN ];

  /* first read the data (up to LINELEN chars) into buffer from infile */
  fgets( buffer, LINELEN, infile );

  /* read in data from a line (program moves down one line automatically) */
  if( '\n' == buffer[ strlen(buffer) - 1 ])
  {
    buffer[ strlen(buffer) - 1 ] = '\0';
  }

  /* assign the first line in the text file to the professor struct */
  prof.name = strdup(buffer);

  /* read in the days that the professor is available (weekdays so 5) */
  for(i = 0; i < 5; i++)
  {
    /* call function to obtain file info; set end of string to null */
    fgets(buffer, LINELEN, infile);
    if('\n' == buffer[ strlen(buffer) - 1 ])
    {
      buffer[ strlen(buffer) - 1 ] = '\0';
    }
    /* put integer value within file into professor struct (schedule) */
    prof.schedule[i] = strtol(buffer, NULL, 10);
  }

  /* read the next line of data in from the file and capture the text */
  fgets( buffer, LINELEN, infile );
  if( '\n' == buffer[ strlen(buffer) - 1 ] )
  {
      /* if at newline, set the end of the string to null */
      buffer[ strlen(buffer) - 1 ] = '\0';
  }

  /* obtain number of students in list; reserve space for array of students */
  num_students = strtol(buffer, NULL, 10);
  all_students = (student*)malloc(sizeof(student) * num_students);

  /* loop to obtain data for each student listed in the file */
  for(i = 0; i < num_students; i++)
  {
      /* add identification of student to the struct */
      all_students[i].id = i;

      /* read the next line of data in from the file and capture the text */
      fgets( buffer, LINELEN, infile );
      if( '\n' == buffer[ strlen(buffer) - 1 ] )
      {
          /* if at newline, set the end of the string to null */
          buffer[ strlen(buffer) - 1 ] = '\0';
      }

      /* add student name to the student struct and increment counter */
      all_students[i].name = strdup(buffer);

      /* read the next line of data in from the file and capture the text */
      fgets( buffer, LINELEN, infile );
      if( '\n' == buffer[ strlen(buffer) - 1 ] )
      {
          /* if at newline, set the end of the string to null */
          buffer[ strlen(buffer) - 1 ] = '\0';
      }

      /* if the visit type is a question, set that visit type in the array */
      if(buffer[0] == 'Q')
      {
          all_students[i].visit_type = QUESTION;
      }
      /* if the visit type is a demo, set that visit type in the array */
      else if(buffer[0] == 'D')
      {
          all_students[i].visit_type = DEMO;
      }
      /* if the visit type is advising, set that visit type in the array */
      else if(buffer[0] == 'A')
      {
          all_students[i].visit_type = ADVISING;
      }
      /* otherwise, do nothing */
      else
      {
          /* blank; do nothing */
      }

      /* read the next line of data in from the file and capture the text */
      fgets( buffer, LINELEN, infile );
      if( '\n' == buffer[ strlen(buffer) - 1 ] )
      {
          /* if at newline, set the end of the string to null */
          buffer[ strlen(buffer) - 1 ] = '\0';
      }

      /* read in the amount of time the student will need in their meeting */
      all_students[i].visit = strtol(buffer, NULL, 10);

      /* read the next line of data in from the file and capture the text */
      fgets( buffer, LINELEN, infile );
      if( '\n' == buffer[ strlen(buffer) - 1 ] )
      {
          /* if at newline, set the end of the string to null */
          buffer[ strlen(buffer) - 1 ] = '\0';
      }

      /* initialize the information in the student array to zero */
      for(j = 0; j < 5; j++)
      {
        all_students[i].available[j] = 0;
      }

      /* loop for checking availability on each day of the week */
      for(j = 0; j < strlen(buffer); j++)
      {
          /* if day is Monday, put that information in the array */
          if(buffer[j] == 'M')
          {
            all_students[i].available[0] = 1;
          }
          /* if day is Tuesday, put that information in the array */
          else if(buffer[j] == 'T')
          {
            all_students[i].available[1] = 1;
          }
          /* if day is Wednesday, put that information in the array */
          else if(buffer[j] == 'W')
          {
            all_students[i].available[2] = 1;
          }
          /* if day is Thursday, put that information in the array */
          else if(buffer[j] == 'R')
          {
            all_students[i].available[3] = 1;
          }
          /* if day is Friday, put that information in the array */
          else if(buffer[j] == 'F')
          {
            all_students[i].available[4] = 1;
          }
          /* otherwise, do nothing */
          else
          {
              /* do not do anything */
          }
        }
      }
    }

/*
 * Simulate the day. For each day, check the availability of the
 * professor and the student, the times of the professor and student,
 * and the visit type of the student. Account for the limited time of
 * the professor and make adjustments as needed.
 */
void simulate(stu_list * today, days_t day)
{
  /* inititalize the variables for tracking the office hour statistics */
  int visits = 0;
  int visit_minutes = 0;
  int waiting = 0;
  int waiting_student_minutes = 0;

  /* loop while professor has time to see students and students still need to visit */
  while(prof.schedule[day] != visit_minutes && today->size != 0)
  {
    /* call pop function to obtain student at start of the array of students */
    prof.curr_student = pop(today);

    /* set state of the visiting student to VISITING */
    prof.curr_student->curr_state = VISITING;

    /* set variable called waiting to size of list; increment visits variable */
    waiting = today->size;
    visits++;

    /* print out the type of office hour visit to the professor on the given day */
    printf("Professor %s is %s %s\n", prof.name, type_to_string(prof.curr_student->visit_type), prof.curr_student->name);

    /* check if visit time of student fits within availability of professor */
    if(prof.curr_student->visit <= prof.schedule[day] - visit_minutes)
    {
      /* if above conditional is true, update the amount of time for visits */
      visit_minutes = visit_minutes + prof.curr_student->visit;

      /* update the waiting time based on the day of visits */
      waiting_student_minutes = waiting_student_minutes + (waiting
        * prof.curr_student->visit);

      /* set visit to zero so no longer need to visit, and set status to DONE */
      prof.curr_student->visit = 0;
      prof.curr_student->curr_state = DONE;

      /* increment the total done counter, which tracks completed visits */
      tot_done++;
    }
    /* otherwise, account for case when student time exceeds professor time */
    else
    {
      /* update the waiting time based on the time student has left to visit */
      waiting_student_minutes = waiting_student_minutes + (waiting * (prof.schedule[day] - visit_minutes));

      /* update amount of time that student has left to visit with professor */
      prof.curr_student->visit = prof.curr_student->visit - (prof.schedule[day] - visit_minutes);

      /* set status of student to waiting and update amount of time for visits */
      prof.curr_student->curr_state = WAITING;
      visit_minutes = prof.schedule[day];
    }
  }
  /* print daily statistics; insert data collected previously into function */
  print_daily_stats(day, visits, visit_minutes, waiting, waiting_student_minutes);
}

/*
 * Create a new list and add all of the students who will be waiting that day
 * to the list according to the priority mode.
 */
stu_list * init_day(days_t today)
{
  /* create counter variable, integer to represent a certain day, and a node */
  int i;
  visit_t chosen_mode;

  /* allocate space for the list struct in order to store nodes in it */
  stu_list * a_list = NULL;
  a_list = (stu_list*)malloc(sizeof(stu_list));

  /* create a new empty list for storing the student information in */
  init_list(a_list);

  /* switch statement to determine the mode based on priority type */
  switch(priority_mode)
  {
    /* if case is advising, change the mode variable to advising */
    case PRI_A:
      chosen_mode = ADVISING;
      break;
    /* if case is question, change the mode variable to question */
    case PRI_Q:
      chosen_mode = QUESTION;
      break;
    /* if case is demo, change the mode variable to demo */
    case PRI_D:
      chosen_mode = DEMO;
      break;
    /* if case is default, break out of switch statment */
    default:
      break;
  }

  /* add students to the list in the order they were read in from the file */
  if(priority_mode == FIFO)
  {
    /* traverse through the array of students */
    for(i = 0; i < num_students; i++)
    {
      /* if student at index has not visited and is available, add to list */
      if(all_students[i].curr_state != DONE && all_students[i].available[today] == 1)
      {
        /* change status of student at index to WAITING, and add to list */
        all_students[i].curr_state = WAITING;
        add(a_list, &all_students[i]);
      }
    }
  }
  /* otherwise populate list based on priority (specified as an argument) */
  else
  {
    /* traverse through the array of students */
    for(i = 0; i < num_students; i++)
    {
      /* check if student at index has not visited, is available, and has mode */
      if(all_students[i].curr_state != DONE && all_students[i].available[today] == 1
        && all_students[i].visit_type == chosen_mode)
      {
        /* if above conditional is true, set status of WAITING and add to list */
        all_students[i].curr_state = WAITING;
        add(a_list, &all_students[i]);
      }
    }
    /* traverse through array of students after inserting priorities in list */
    for(i = 0; i < num_students; i++)
    {
      /* check if student at index has not visited, is available, and not mode */
      if(all_students[i].curr_state != DONE && all_students[i].available[today] == 1
        && all_students[i].visit_type != chosen_mode)
      {
        /* if above conditional is true, set status of WAITING and add to list */
        all_students[i].curr_state = WAITING;
        add(a_list, &all_students[i]);
      }
    }
  }
  /* return the doubly linked list of students waiting that day */
  return a_list;
}

/*
 * Clean the list by calling a function within
 * this function that frees the memory used to
 * store the information in the list.
 */
void cleanup_list(stu_list * a_list)
{
  /* loop runs until the size of the list is zero */
  while(a_list->size != 0)
  {
    /* while list size is not zero, pop element at start of list */
    pop(a_list);
  }
}

/*
 * Return 0 if all of the students have visited the professor.
 */
int is_all_done()
{
  /* if all the students have the status of DONE, return 0 */
  if(tot_done == num_students)
  {
    return 0;
  }
  /* otherwise, if not all students have completed visits, return -1 */
  return -1;
}

/*******************************************************************************
 * Helpers and print functions - DO NOT CHANGE                                 *
 ******************************************************************************/

/*
 * Print the initial statistics about the simulation
 */
void print_init_stats()
{
  printf("Simulating Office Hours for Professor %s\n", prof.name);
  printf("-----------------------------------------------------------\n");
  switch(priority_mode){
    case FIFO:
      printf("Algorithm: FIFO\n");
      break;
    case PRI_A:
      printf("Algorithm: Priority (Advising)\n");
      break;
    case PRI_D:
      printf("Algorithm: Priority (Demos)\n");
      break;
    case PRI_Q:
      printf("Algorithm: Priority (Questions)\n");
      break;
    default:
      printf("Algorithm: Undefined\n");
      break;
  }

  printf("Schedule:\n");
  printf("%10s %10s %10s %10s %10s\n", "M", "T", "W", "R", "F");
  printf("-----------------------------------------------------------\n");
  printf("%10d %10d %10d %10d %10d\n", prof.schedule[MONDAY],
                                       prof.schedule[TUESDAY],
                                       prof.schedule[WEDNESDAY],
                                       prof.schedule[THURSDAY],
                                       prof.schedule[FRIDAY]);
  printf("===========================================================\n");
  printf("Initial list of students:\n");
  print_state();
  printf("===========================================================\n");
}

/*
 * Print the final statistics about the simulation
 */
void print_final_stats()
{
  printf("-----------------------------------------------------------\n");
  printf("Professor %s met with %d students for %d minutes\nwith a total waiting time of %d.\n",
              prof.name, num_students, tot_visit_min, tot_waiting);
  printf("===========================================================\n");
}

/*
 * Print the state of the simulation - used for debugging
 */
void print_state()
{
  int i;
  for(i = 0; i < num_students; i++)
  {
    printf("%d %s %d %d ", all_students[i].id, all_students[i].name, all_students[i].visit, all_students[i].waiting);
    switch(all_students[i].visit_type)
    {
      case QUESTION:
        printf("Q ");
        break;
      case ADVISING:
        printf("A ");
        break;
      case DEMO:
        printf("D ");
        break;
      default:
        printf("uh oh! ");
        break;
    }
    switch(all_students[i].curr_state){
      case INIT:
        printf("-- init\n");
        break;
      case WAITING:
        printf("-- waiting\n");
        break;
      case VISITING:
        printf("-- visiting\n");
        break;
      case DONE:
        printf("-- done\n");
        break;
      default:
        printf("-- uh oh!\n");
        break;
    }
  }
}

void print_daily_stats(days_t d, int visits, int v_mins, int waiting, int wait_mins)
{
  printf("Stats for %s:\n", day_to_string(d));
  printf("\t Visits: %10d   |   Visit Minutes: %10d\n", visits, v_mins);
  printf("\t Waiting: %9d   |   Waiting Minutes: %8d\n", waiting, wait_mins);
  printf("-----------------------------------------------------------\n");
  tot_waiting += wait_mins;
  tot_visit_min += v_mins;
}

char * day_to_string(days_t d)
{
  switch(d){
    case MONDAY:
      return strdup("Monday");
    case TUESDAY:
      return strdup("Tuesday");
    case WEDNESDAY:
      return strdup("Wednesday");
    case THURSDAY:
      return strdup("Thursday");
    case FRIDAY:
      return strdup("Friday");
    default:
      return strdup("Uh oh!");
  }
}

char * type_to_string(visit_t t)
{
  switch(t){
    case QUESTION:
      return strdup("answering a question from");
    case ADVISING:
      return strdup("advising");
    case DEMO:
      return strdup("viewing a demo from");
    default:
      return strdup("Uh oh!");
  }
}

void correct_usage_message()
{
  printf("Please use -f to indicate FIFO or -a, -d, or -q to prioritize a visit type, and provide the name of the file for configuring the simulation.\n");
}
