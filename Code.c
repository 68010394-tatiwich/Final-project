#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILENAME "employees.csv"
#define MAX_LINE 1024


void output_alldata() {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        perror("Error opening file");
        return ;
    }

    char line[MAX_LINE];

   
    
        printf("=== Employee List ===\n");
    

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        char *name = strtok(line, ",");
        char *start_date = strtok(NULL, ",");
        char *position = strtok(NULL, ",");
        char *status = strtok(NULL, ",");

        if (name && start_date && position && status) {
            printf("Name: %s | Start Date: %s | Position: %s | Status: %s\n",
                   name, start_date, position, status);
        }
    }
    fclose(fp);
}


void addemployeename() {
    char input[MAX_LINE];

    // Flush stdin to remove leftover newline from previous input
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter new employee (format: name,date,position,status): ");
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error.\n");
        return;
    }

    input[strcspn(input, "\n")] = 0;  // Remove trailing newline

    char *name = strtok(input, ",");
    char *start_date = strtok(NULL, ",");
    char *position = strtok(NULL, ",");
    char *status = strtok(NULL, ",");

    if (!name || !start_date || !position || !status) {
        printf("Invalid input format.\n");
        return;
    }

    FILE *fp = fopen(FILENAME, "a");
    if (!fp) {
        printf("Error: Could not open file %s\n", FILENAME);
        return;
    }
    fprintf(fp, "%s,%s,%s,%s\n", name, start_date, position, status);
    fclose(fp);

    printf("Employee added successfully.\n");
}

void Searchbyname() {
    char name[MAX_LINE];
    printf("Enter name to search: ");
    scanf("%s", name);

    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        perror("Error opening file");
        return ;
    }

    char line[MAX_LINE];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        char *emp_name = strtok(line, ",");
        char *start_date = strtok(NULL, ",");
        char *position = strtok(NULL, ",");
        char *status = strtok(NULL, ",");

        if (emp_name && strcmp(emp_name, name) == 0) {
            printf("Found: Name: %s | Start Date: %s | Position: %s | Status: %s\n",
                   emp_name, start_date, position, status);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Employee not found.\n");
    }

    fclose(fp);
}

void Searchbyposition() {
    char position[MAX_LINE];
    printf("Enter position to search: ");
    scanf("%s", position);

    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        perror("Error opening file");
        return ;
    }

    char line[MAX_LINE];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        char *name = strtok(line, ",");
        char *start_date = strtok(NULL, ",");
        char *emp_position = strtok(NULL, ",");
        char *status = strtok(NULL, ",");

        if (emp_position && strcmp(emp_position, position) == 0) {
            printf("Found: Name: %s | Start Date: %s | Position: %s | Status: %s\n", name, start_date, emp_position, status);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Employee not found.\n");
    }

    fclose(fp);
}

void Updateemployeedata() {
    char name[MAX_LINE];
    printf("Enter name of employee to update: ");
    scanf("%s", name);

    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        perror("Error opening file");
        return;
    }

    FILE *temp_fp = fopen("data.csv", "w");
    if (!temp_fp) {
        perror("Error opening temporary file");
        fclose(fp);
        return;
    }

    char line[MAX_LINE];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        char *emp_name = strtok(line, ",");
        char *start_date = strtok(NULL, ",");
        char *position = strtok(NULL, ",");
        char *status = strtok(NULL, ",");

        if (emp_name && strcmp(emp_name, name) == 0) {
            found = 1;
            char new_start_date[MAX_LINE];
            char new_position[MAX_LINE];
            char new_status[MAX_LINE];

            strncpy(new_start_date, start_date, MAX_LINE - 1);
            strncpy(new_position, position, MAX_LINE - 1);
            strncpy(new_status, status, MAX_LINE - 1);
            new_start_date[MAX_LINE - 1] = '\0';
            new_position[MAX_LINE - 1] = '\0';  
            new_status[MAX_LINE - 1] = '\0';


            int choice;
            while (1) {
                printf("\n===== Update Employee Data? =====\n");
                printf("1. Change start date\n");
                printf("2. Change position\n");
                printf("3. Change status\n");
                printf("4. EXIT\n");
                printf("Choose an option: ");

                scanf("%d", &choice);
                getchar(); // à¹à¸à¸¥à¸µà¸¢à¸£à¹ newline à¸à¸µà¹à¸à¹à¸²à¸à¹à¸ buffer

                switch (choice) {
                    case 1:
                        printf("Enter new start date: ");
                        scanf("%s", new_start_date);
                        break;
                    case 2:
                        printf("Enter new position: ");
                        scanf("%s", new_position);
                        break;
                    case 3:
                        printf("Enter new status: ");
                        scanf(" %[^\n]", new_status);  // à¸­à¹à¸²à¸à¸à¸±à¹à¸à¸à¸£à¸£à¸à¸±à¸
                        break;
                    case 4:
                        printf("EXIT\n");
                        goto WRITE_BACK;   // à¸à¸£à¸°à¹à¸à¸à¸­à¸­à¸à¹à¸à¹à¸à¸µà¸¢à¸à¹à¸à¸¥à¹
                        break;
                    default:
                        printf("Invalid choice\n");
                        break;
                }
            }

            WRITE_BACK:
                fprintf(temp_fp, "%s,%s,%s,%s\n", emp_name, new_start_date, new_position, new_status);
        }else {
            fprintf(temp_fp, "%s,%s,%s,%s\n", emp_name, start_date, position, status);
        }
        
    }
        

    fclose(fp);
    fclose(temp_fp);

    if (found) {
        remove(FILENAME);
        rename("data.csv", FILENAME);
        printf("Employee updated successfully.\n");
    } else {
        remove("data.csv");
        printf("Employee not found.\n");
    }
}

void Deleteemployeedata() {
    char name[MAX_LINE];
    printf("Enter name of employee to delete: ");
    scanf("%s", name);

    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        perror("Error opening file");
        return ;
    }

    FILE *temp_fp = fopen("data.csv", "w");
    if (!temp_fp) {
        perror("Error opening temporary file");
        fclose(fp);
        return ;
    }

    char line[MAX_LINE];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        char *emp_name = strtok(line, ",");
        char *start_date = strtok(NULL, ",");
        char *position = strtok(NULL, ",");
        char *status = strtok(NULL, ",");

        if (emp_name && strcmp(emp_name, name) == 0) {
            found = 1; 
            continue;
        }
        fprintf(temp_fp, "%s,%s,%s,%s\n", emp_name, start_date, position, status);
    }

    fclose(fp);
    fclose(temp_fp);

    if (found) {
        remove(FILENAME);
        rename("data.csv", FILENAME);
        printf("Employee deleted successfully.\n");
    } else {
        remove("data.csv");
        printf("Employee not found.\n");
    }
}

int main() {
    int choice;

    while (1) {
        printf("\n===== Employee System =====\n");
        printf("1. Add employee name\n");
        printf("2. Show employee data\n");
        printf("3. Search by name\n");
        printf("4. Search by position\n");
        printf("5. Update employee data\n");
        printf("6. Delete employee data\n");
        printf("7. EXIT\n");
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while(getchar() != '\n'); 
            continue;
        }

        switch (choice) {
            case 1:
                addemployeename();
                break;
            case 2:
                output_alldata();
                break;
            case 3:
                Searchbyname();
                break;
            case 4:
                Searchbyposition();
                break;
            case 5:
                Updateemployeedata();
                break;
            case 6:
                Deleteemployeedata();
                break;
            case 7:
                printf("EXIT\n");
                return 0;
            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}
