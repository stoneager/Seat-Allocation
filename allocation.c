#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHOICES 10
#define NUM_INSTITUTES 6
#define MAX_IITs 3
#define MAX_STUDENTS 100

typedef struct {
  int institute;
  char branch;
} Seat;

typedef struct {
  float percentile_exam1;
  float percentile_exam2;
  Seat choices[MAX_CHOICES];
  int num_choices;
  Seat allocated_seat;
} Student;

void read_students(Student students[], int num_students, const char *file_path);
void allocate_seats(Student students[], int num_students, int seat_limits[]);
void print_allocation(Student students[], int num_students);
int login(const char uname[], const char pass[]);
void printStudentAllocation(Student students[], int linecount, int num_students, char *user);

int main() {
  int num_students = MAX_STUDENTS;

  Student *students = malloc(num_students * sizeof(Student));
  if (students == NULL) {
    printf("Memory allocation failed.\n");
    return 1;
  }

  const char *file_path = "choices.csv";
  read_students(students, num_students, file_path);
  int seat_limits[NUM_INSTITUTES];
  allocate_seats(students, num_students, seat_limits);
  print_allocation(students, num_students);

  free(students);

  return 0;
}

int login(const char uname[], const char pass[]) {
  int line_count = 0;
  char s[500];
  FILE *fp;
  fp = fopen("c data.txt", "r");
  if (fp == NULL) {
    printf("File Not Opened.\n");
    return -1;
  }

  while (fgets(s, sizeof(s), fp) != NULL) {
    char *token;
    char *username = NULL;
    char *password = NULL;
    int count = 0;

    token = strtok(s, ",");
    while (token != NULL) {
      count++;
      if (count == 7) {
        username = token;
      } else if (count == 8) {
        password = token;
      }
      token = strtok(NULL, ",");
    }

    if (username != NULL && password != NULL) {
      if (strcmp(username, uname) == 0 && strcmp(password, pass) == 0) {
        printf("Logged in successfully.\n");
        fclose(fp);
        return line_count;
      }
    }

    line_count++;
  }

  printf("Username and Password combination not found.\n");
  fclose(fp);
  return -1;
}

void read_students(Student students[], int num_students, const char *file_path) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    printf("Failed to open the file.\n");
    return;
  }

  char line[500];
  for (int i = 0; i < num_students; i++) {
    if (fgets(line, sizeof(line), file) != NULL) {
      char *token = strtok(line, ",");
      if (token != NULL) {
        students[i].percentile_exam1 = atof(token);
        token = strtok(NULL, ",");
        if (token != NULL) {
          students[i].percentile_exam2 = atof(token);
          token = strtok(NULL, ",");
          students[i].num_choices = 0;
          students[i].allocated_seat.institute = 0;
          students[i].allocated_seat.branch = '\0';
          while (token != NULL) {
            int institute = atoi(token);
            token = strtok(NULL, ",");
            char branch = *token;

            if (students[i].num_choices < MAX_CHOICES) {
              students[i].choices[students[i].num_choices].institute = institute;
              students[i].choices[students[i].num_choices].branch = branch;
              students[i].num_choices++;
            }

            token = strtok(NULL, ",");
          }
        }
      }
    }
  }

  fclose(file);
}

void allocate_seats(Student students[], int num_students, int seat_limits[]) {
  FILE *fp = fopen("institute.csv", "r");
  char line[500];
  int i = -1;
  while (fgets(line, sizeof(line), fp) != NULL) {
    char *token = strtok(line, ",");
    int count = 1;
    while (token != NULL) {
      token = strtok(NULL, ",");
      count++;
      if (count == 4 && i > -1) {
        int val = atoi(token);
        seat_limits[i] = val;
      }
    }
    i++;
  }

  int seats_available[NUM_INSTITUTES][MAX_CHOICES];
  for (int i = 0; i < NUM_INSTITUTES; i++) {
    for (int j = 0; j < MAX_CHOICES; j++) {
      seats_available[i][j] = seat_limits[i];
    }
  }

  for (int i = 0; i < num_students; i++) {
    if (students[i].percentile_exam1 >= 95.0) {
      for (int j = 0; j < students[i].num_choices; j++) {
        if (students[i].choices[j].institute >= 1 && students[i].choices[j].institute <= MAX_IITs) {
          int institute = students[i].choices[j].institute;
          char branch = students[i].choices[j].branch;
          if (seats_available[institute - 1][branch - 'a'] > 0) {
            students[i].allocated_seat = students[i].choices[j];
            seats_available[institute - 1][branch - 'a']--;
            break;
          }
        }
      }
    } else if (students[i].percentile_exam2 >= 85.0) {
      for (int j = 0; j < students[i].num_choices; j++) {
        if (students[i].choices[j].institute > MAX_IITs && students[i].choices[j].institute <= NUM_INSTITUTES) {
          int institute = students[i].choices[j].institute;
          char branch = students[i].choices[j].branch;
          if (seats_available[institute - 1][branch - 'a'] > 0) {
            students[i].allocated_seat = students[i].choices[j];
            seats_available[institute - 1][branch - 'a']--;
            break;
          }
        }
      }
    }
  }
}

void print_allocation(Student students[], int num_students) {
  printf("\nSeat Allocation:\n");
  for (int i = 0; i < num_students; i++) {
    printf("Student %d - Percentile (Exam 1): %.2f, Percentile (Exam 2): %.2f\n",
           i + 1, students[i].percentile_exam1, students[i].percentile_exam2);
    if (students[i].allocated_seat.institute != 0) {
      printf("Allocated Seat - Institute: %d, Branch: %c\n",
             students[i].allocated_seat.institute,
             students[i].allocated_seat.branch);
    } else {
      printf("No seat allocated.\n");
    }
    printf("\n");
  }
}

void printStudentAllocation(Student students[], int linecount, int num_students, char *user) {
  printf("\nSeat Allocation:\n");
  for (int i = 0; i < num_students; i++) {
    if (i == linecount) {
      printf("Student %s - Percentile (Exam 1): %.2f, Percentile (Exam 2): %.2f\n",
             user, students[i].percentile_exam1, students[i].percentile_exam2);
      if (students[i].allocated_seat.institute != 0) {
        printf("Allocated Seat - Institute: %d, Branch: %c\n",
               students[i].allocated_seat.institute,
               students[i].allocated_seat.branch);
      } else {
        printf("No seat allocated.\n");
      }
    }
  }
  printf("\n");
}
