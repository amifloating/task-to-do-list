#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100
#define SAVE_FILE "tasks.txt"

typedef struct {
  char *desc;
  int priority;

} Task;

// functions
void addTask(Task **tasks, int *count);
void viewTasks(Task *tasks, int count);
void removeTask(Task **tasks, int *count);
void saveTasks(Task *tasks, int count);
void loadTasks(Task **tasks, int *count);
void sortTasks(Task *tasks, int count);

int main() {
  Task *tasks = NULL;
  int count = 0;
  int choice;
  
  loadTasks(&tasks, &count);
  
  do {
    printf("\n--- To-Do List ---\n");
    printf("1. Add task\n");
    printf("2. View tasks\n");
    printf("3. Remove task\n");
    printf("4. Save tasks\n");
    printf("5. Sort tasks\n");
    printf("6. Quit\n");
    printf("Choose an option: ");
    scanf("%d", &choice);
    getchar();
  
    switch (choice) {
      case 1: addTask(&tasks, &count); break;
      case 2: viewTasks(tasks, count); break;
      case 3: removeTask(&tasks, &count); break;
      case 4: saveTasks(tasks, count); break;
      case 5: sortTasks(tasks, count); break;
      case 6: break;
      default: printf("error. try again.\n");
    }
  } while (choice != 6);
  
  saveTasks(tasks, count);
  
  for (int i = 0; i < count; i++) {
      free(tasks[i].desc);
  }
  free(tasks);
  
  printf("Goodbye!\n");
  return 0;
}

void addTask(Task **tasks, int *count) {
  char buffer[MAX_LEN];
  int priority;
  
  printf("Enter new task: ");
  fgets(buffer, MAX_LEN, stdin);
  buffer[strcspn(buffer, "\n")] = '\0';
  
  printf("Enter priority (1=low, 5=high): ");
  scanf("%d", &priority);
  getchar();
  
  *tasks = realloc(*tasks, (*count + 1) * sizeof(Task));
  if (*tasks == NULL) {
      perror("realloc failed");
      exit(1);
  }
  
  (*tasks)[*count].desc = malloc(strlen(buffer) + 1);
  if ((*tasks)[*count].desc == NULL) {
      perror("malloc failed");
      exit(1);
  }
  strcpy((*tasks)[*count].desc, buffer);
  (*tasks)[*count].priority = priority;
  
  (*count)++;
  printf("Task added!\n");
}

// view tasks
void viewTasks(Task *tasks, int count) {
  if (count == 0) {
    printf("nothing to see here\n");
    return;
  }
  printf("\n--- your task list ---\n");
  for (int i = 0; i < count; i++) {
      printf("%d. [%d] %s\n", i + 1, tasks[i].priority, tasks[i].desc);
  }
}

// remove a task from the list
void removeTask (Task **tasks, int *count) {
  if (*count == 0) {
      printf("no tasks found to remove!");
      return;
  }
  
  int index;
  viewTasks(*tasks, *count);
  printf("enter the task number that you want removed: ");
  scanf("%d", &index);
  getchar();
  
  if (index < 1 || index > *count) {
      printf("invalid task number.\n");
      return;
  }
  
  free((*tasks)[index - 1].desc);
  
  for (int i = index - 1; i < *count - 1; i++) {
      (*tasks)[i] = (*tasks)[i + 1];
  }
  
  *tasks = realloc(*tasks, (*count - 1) * sizeof(Task));
  (*count)--;
  
  printf("task removed!\n");
}

// save tasks to file
void saveTasks(Task *tasks, int count) {
    FILE *fp = fopen(SAVE_FILE, "w");
    if (fp == NULL) {
        perror("failed to open file");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d|%s\n", tasks[i].priority, tasks[i].desc);
    }
    fclose(fp);
    printf("tasks saved to %s\n", SAVE_FILE);
}

// load tasks from a save file
void loadTasks(Task **tasks, int *count) {
    FILE *fp = fopen(SAVE_FILE, "r");
    if (fp == NULL) {
        return;
}

char buffer[MAX_LEN];
while (fgets(buffer, MAX_LEN, fp)) {
    buffer[strcspn(buffer, "\n")] = '\0';
    
    char *sep = strchr(buffer, '|');
    if (sep == NULL) continue;
    
    int priority = buffer[0] - '0'; //reads the first number
    char *desc = sep + 1; 
    
    *tasks = realloc(*tasks, (*count + 1) * sizeof(Task));
    if (*tasks == NULL) {
      perror("realloc failed");
      exit(1);
    }
    
    (*tasks)[*count].desc = malloc(strlen(desc) + 1);
    strcpy((*tasks)[*count].desc, desc);
    (*tasks)[*count].priority = priority;
    (*count)++;
  }
  fclose(fp);
}

// priority sorting
void sortTasks(Task *tasks, int count) {
  if (count < 2) {
      printf("need more tasks to sort \n");
      return;
  }
  
  for (int i = 0; i < count - 1; i++) {
      for (int j = i + 1; j < count; j++) {
          if (tasks[i].priority < tasks[j].priority || 
             (tasks[i].priority == tasks[j].priority &&
              strcmp(tasks[i].desc, tasks[j].desc) >0)) {
              Task temp = tasks[i];
              tasks[i] = tasks[j];
              tasks[j] = temp;
              }
      }
  }
printf("tasks sorted first by priority, then alphabetical");
}
