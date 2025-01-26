#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define tot_instit 6
typedef struct {
  char institute[75];
  int seats[7];
} Institute;

float percentile(int req_count);
int login();
void add_to_line(const char *file_path, const char *new_values,
                 int line_number);
void append_values_in_middle(const char *file_path, const char *new_values,
                             int line_number);
void choicefill(int linecount);

int main() {
  
  int linecount = login();
  if (linecount != -1){
  choicefill(linecount);
  }
}
void add_to_line(const char *file_path, const char *new_values,
                 int line_number) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    printf("Failed to open the file.\n");
    return;
  }

  FILE *temp_file = tmpfile();
  if (temp_file == NULL) {
    printf("Failed to create temporary file.\n");
    fclose(file);
    return;
  }

  char line[100];
  int current_line = -1;

  while (fgets(line, sizeof(line), file) != NULL) {
    current_line++;

    if (current_line == line_number) {

      line[strcspn(line, "\n")] = '\0';

      strcat(line, new_values);
      strcat(line, ",");
      strcat(line, "\n");
    }

    fputs(line, temp_file);
  }

  fclose(file);

  rewind(temp_file);

  file = fopen(file_path, "w");
  if (file == NULL) {
    printf("Failed to open the file.\n");
    fclose(temp_file);
    return;
  }

  while (fgets(line, sizeof(line), temp_file) != NULL) {
    fputs(line, file);
  }

  fclose(temp_file);

  fclose(file);
}

int login() {
  int sign_or_log = -1;
  char uname[50];
  char pass[25];
  char name[30];
  char regnumber[15];

  int line_count = 1;
  char s[200];
  FILE *fp;
  fp = fopen("login file.csv", "r");
  if (fp == NULL) {
    printf("File Not Opened.\n");
    return -1;
  }

  while (sign_or_log != 0) {
    printf("Enter 1 for Sign up and 2 for Login and 0 to Quit: ");
    scanf("%d", &sign_or_log);
    getchar();

    if (sign_or_log == 1) {
      printf("Enter your name: ");
      fgets(name, sizeof(name), stdin);
      name[strcspn(name, "\n")] = '\0';

      printf("Enter your register number: ");
      scanf("%s", regnumber);

      while (fgets(s, sizeof(s), fp) != NULL) {
        char *token;
        char *tname = NULL;
        char *tregnumber = NULL;
        int count = 0;

        token = strtok(s, ",");
        while (token != NULL) {
          count++;
          if (count == 1) {
            tname = token;
          } else if (count == 2) {
            tregnumber = token;
          }
          if (count > 5 && strcmp(tname, name) == 0 &&
              strcmp(tregnumber, regnumber) == 0) {
            printf("You have already registered. Please use your Username and "
                   "Password\n");
            break;
          }
          token = strtok(NULL, ",");
        }

        if (count == 5 && tname != NULL && tregnumber != NULL) {
          if (strcmp(tname, name) == 0 && strcmp(tregnumber, regnumber) == 0) {
            printf("Set up a username: ");
            scanf("%s", uname);
            printf("Set a password: ");
            scanf("%s", pass);
            strcat(uname, ",");
            strcat(uname, pass);
            add_to_line("login file.csv", uname, line_count);

            printf("Successfully Signed Up. Please Login with your New "
                   "Username and Password\n\n");
            break;
          }
        }
        line_count++;
      }
      fclose(fp);
    } else if (sign_or_log == 2) {
      printf("Enter username: ");
      scanf("%s", uname);
      printf("Enter password: ");
      scanf("%s", pass);

      fp = fopen("login file.csv", "r");
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
          if (count == 5) {
            username = token;
          } else if (count == 6) {
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

      fclose(fp);
      return 0;
    }
  }
}

float percentile(int req_count) {
  FILE *fp;
  fp = fopen("login file.csv", "r");
  if (fp == NULL) {
    printf("File Not Opened.\n");
    return -1;
  }
  fseek(fp, 0, SEEK_END);
  long file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char *line = malloc(file_size + 1);
  if (line == NULL) {
    fclose(fp);
    printf("Memory allocation failed.\n");
    return -1;
  }
  int line_count = 1;
  char *token;
  int count = 0;
  while (fgets(line, file_size, fp) != NULL) {
    if (line_count == req_count) {
      token = strtok(line, ",");
      while (token != NULL) {
        count++;
        if (count == 4) {
          float value = atof(token);
          return value;
        }
        token = strtok(NULL, ",");
      }
      count = 0;
    }
    line_count++;
  }

  fclose(fp);
  free(line);

  return -1;
}

void append_values_in_middle(const char *file_path, const char *new_values,
                             int line_number) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    printf("Failed to open the file.\n");
    return;
  }

  FILE *temp_file = tmpfile();
  if (temp_file == NULL) {
    printf("Failed to create temporary file.\n");
    fclose(file);
    return;
  }

  char line[100];
  int current_line = -1;

  while (fgets(line, sizeof(line), file) != NULL) {
    current_line++;

    if (current_line == line_number) {

      line[strcspn(line, "\n")] = '\0';

      strcat(line, new_values);
      strcat(line, ",");
      strcat(line, "\n");
    }

    fputs(line, temp_file);
  }

  fclose(file);

  rewind(temp_file);

  file = fopen(file_path, "w");
  if (file == NULL) {
    printf("Failed to open the file.\n");
    fclose(temp_file);
    return;
  }

  while (fgets(line, sizeof(line), temp_file) != NULL) {
    fputs(line, file);
  }

  fclose(temp_file);

  fclose(file);
}

void choicefill(int linecount) {
  int n;
  printf("\n");
  printf("*******************Welcome to the Choice Filling "
         "Portal*******************\n");
  printf("Enter no of choices: ");
  scanf("%d", &n);
  char *in = malloc(sizeof(char) * 100);
  char *br = malloc(sizeof(char) * 100);

  if (percentile(linecount) >= 85.00) {
    char instarr[75];
    FILE *inst = fopen("Instit.txt", "r");
    FILE *bran = fopen("Institutes.csv", "r");
    Institute institute[tot_instit];
    int j = 0;
    while (fgets(instarr, sizeof(instarr), inst) != NULL) {
      strcpy(institute[j].institute, instarr);
      j++;
    }
    j = 0;
    for (int i = 0; i < n; i++) {
      while (j < tot_instit) {
        printf("\t\t\t%d.%s\n", j + 1, institute[j].institute);
        j++;
      }
      printf("\n\t\t\tEnter Institute No %d: ", i + 1);
      scanf("%s", in);
      append_values_in_middle("c project.csv", in, linecount);
      printf("a.B.Tech Computer Science & Engineering\n");
      printf("b.B.Tech Mechanical Engineering\n");
      printf("c.B.Tech Civil Engineering\n");
      printf("d.B.Tech Chemical Engineering\n");
      printf("e.B.Tech Aerospace Engineering\n");
      printf("\nEnter a Branch: ");
      scanf(" %s", br);
      append_values_in_middle("c project.csv", br, linecount);
    }
    printf("***All the Choices added Successfully***");
  } else {
    for (int i = 0; i < n; i++) {
      char instarr[75];
      FILE *inst = fopen("Instit.txt", "r");
      FILE *bran = fopen("Institutes.csv", "r");
      Institute institute[tot_instit];
      int j = 0;
      while (fgets(instarr, sizeof(instarr), inst) != NULL) {
        strcpy(institute[j].institute, instarr);
      }
      j = 3;
      for (int i = 0; i < n; i++) {
        while (j < tot_instit) {
          printf("%d.%s\n", j + 1, institute[j].institute);
          j++;
        }
        printf("\n\t\t\tEnter Institute No %d: ", i + 1);
        scanf("%s", in);
        char val = atoi(in);
        append_values_in_middle("c project.csv", in, linecount);
        printf("a.B.Tech Computer Science & Engineering\n");
        printf("b.B.Tech Mechanical Engineering\n");
        printf("c.B.Tech Civil Engineering\n");
        printf("d.B.Tech Chemical Engineering\n");
        printf("e.B.Tech Aerospace Engineering\n");
        printf("\nEnter a Branch: ");
        scanf(" %s", br);
        append_values_in_middle("c project.csv", br, linecount);
      }
      printf("***All the Choices added Successfully***");
    }
    free(in); // Free the allocated memory
    free(br);
  }
}