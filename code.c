#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define filename "employees.csv"
#define MAX_LINE 1024
#define Tempfile "temp.csv"


void to_lower_str(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);  // cast for safety
    }
}

bool namecmp(const char* n1,const char* n2){
    char name1[MAX_LINE],name2[MAX_LINE];
    strcpy(name1,n1);
    strcpy(name2,n2);
    to_lower_str(name1);
    to_lower_str(name2);
    if(strstr(name1,name2) != NULL){
        return true;
    }
    return false;
}

void output(char *fname) {
    char line[MAX_LINE] = {};

    FILE *file = fopen(fname, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // skip header if you want: fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == ',') {
                line[i] = ' ';
            }
        }
        printf("%s", line);
    }

    if (fclose(file) != 0) {
        perror("Error closing file");
    }
}

void add_employee(const char *fname,
                  const char *name,
                  const char *start_date,
                  const char *position,
                  const char *status) {
    FILE *file = fopen(fname, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "%s,%s,%s,%s\n", name, start_date, position, status);

    if (fclose(file) != 0) {
        perror("Error closing file");
    }

    printf("Add employee successfully\n");
}

void search_by_name(const char *fname, const char *name) {
    char line[MAX_LINE] = {};
    int found = 0;
    FILE *file = fopen(fname, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fgets(line, sizeof(line), file); // skip header

    while (fgets(line, sizeof(line), file)) {
        char tmp_line[MAX_LINE];
        strcpy(tmp_line, line);
        char *tmp_name = strtok(tmp_line, ",");
        if (tmp_name && namecmp(tmp_name,name)) {
            if (found == 0) {
                printf("Found:\n");
            }
            for (int i = 0; line[i] != '\0'; i++) {
                if (line[i] == ',') {
                    line[i] = ' ';
                }
            }
            printf("%s", line);
            found++;
        }
    }

    if (fclose(file) != 0) {
        perror("Error closing file");
    }

    if (found == 0) {
        printf("%s not found\n", name);
    }
}

void search_by_position(const char *fname, const char *position) {
    char line[MAX_LINE] = {};
    int found = 0;
    FILE *file = fopen(fname, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fgets(line, sizeof(line), file); // skip header

    while (fgets(line, sizeof(line), file)) {
        char tmp_line[MAX_LINE];
        strcpy(tmp_line, line);
        char *tmp_pos = strtok(tmp_line, ",");
        tmp_pos = strtok(NULL, ",");
        tmp_pos = strtok(NULL, ",");
        if (tmp_pos && namecmp(tmp_pos,position)) {
            if (found == 0) {
                printf("Found:\n");
            }
            for (int i = 0; line[i] != '\0'; i++) {
                if (line[i] == ',') {
                    line[i] = ' ';
                }
            }
            printf("%s", line);
            found++;
        }
    }

    if (fclose(file) != 0) {
        perror("Error closing file");
    }

    if (found == 0) {
        printf("position %s not found\n", position);
    }
}

void deleteemployeedata(const char *fname,const char *name) {
    FILE *fp = fopen(fname, "r");
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

        if (emp_name && namecmp(emp_name, name)) {
            found = 1; 
            continue;
        }
        fprintf(temp_fp, "%s,%s,%s,%s\n", emp_name, start_date, position, status);
    }

    fclose(fp);
    fclose(temp_fp);

    if (found) {
        remove(fname);
        rename("data.csv", fname);
        printf("Employee deleted successfully.\n");
    } else {
        remove("data.csv");
        printf("Employee not found.\n");
    }
}

void Updateemployeedata(const char* fname, const char *name) {
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        perror("Error opening file");
        return;
    }

    FILE *temp_fp = fopen("data.csv", "w");  // <-- FIXED: use "w"
    if (!temp_fp) {
        perror("Error opening temporary file");
        fclose(fp);
        return;
    }

    char line[MAX_LINE];
    int found = 0;

    //fgets(line, sizeof(line), fp); // skip header
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        char *emp_name = strtok(line, ",");
        char *start_date = strtok(NULL, ",");
        char *position = strtok(NULL, ",");
        char *status = strtok(NULL, ",");

        if (emp_name && namecmp(emp_name, name)) {
            found = 1;
            char new_start_date[MAX_LINE];
            char new_position[MAX_LINE];
            char new_status[MAX_LINE];

            strcpy(new_start_date, start_date);
            strcpy(new_position, position);
            strcpy(new_status, status);

            int choice;
            while (1) {
                printf("\n===== Update Employee Data =====\n");
                printf("1. Change start date\n");
                printf("2. Change position\n");
                printf("3. Change status\n");
                printf("4. EXIT\n");
                printf("Choose an option: ");
                scanf("%d", &choice);
                getchar();

                if (choice == 1) {
                    printf("Enter new start date: ");
                    scanf("%s", new_start_date);
                } else if (choice == 2) {
                    printf("Enter new position: ");
                    scanf("%s", new_position);
                } else if (choice == 3) {
                    printf("Enter new status: ");
                    scanf(" %[^\n]", new_status);
                } else if (choice == 4) {
                    printf("EXIT\n");
                    break;
                } else {
                    printf("Invalid choice.\n");
                }
            }

            fprintf(temp_fp, "%s,%s,%s,%s\n", emp_name, new_start_date, new_position, new_status);
        } else {
            fprintf(temp_fp, "%s,%s,%s,%s\n", emp_name, start_date, position, status);
        }
    }

    fclose(fp);
    fclose(temp_fp);

    remove(fname);
    rename("data.csv", fname);

    if (!found) {
        printf("Employee '%s' not found.\n", name);
    }
}


void process() {
    int choice;
    while (1) {
        printf("=================MENU===============\n");
        printf("--------------------------------\n1 : output every employees\n");
        printf("2 : add employee\n");
        printf("3 : search employees by name\n");
        printf("4 : search employees by position\n");
        printf("5 : delete employees\n");
        printf("6 : change data\n");
        printf("7 : exit program\n");
        printf("Enter choice : ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                printf("--------------------------------\n");
                output(filename);
                break;
            }

            case 2: {
                char name[MAX_LINE], date[MAX_LINE], position[MAX_LINE], status[MAX_LINE];
                printf("Enter name : ");
                scanf(" %[^\n]", name);
                printf("Enter start date (yyyy-mm-dd) : ");
                scanf(" %[^\n]", date);
                printf("Enter position : ");
                scanf(" %[^\n]", position);
                printf("Enter status : ");
                scanf(" %[^\n]", status);
                add_employee(filename, name, date, position, status);
                break;
            }

            case 3: {
                char name[MAX_LINE];
                printf("Enter name : ");
                scanf(" %[^\n]", name);
                search_by_name(filename, name);
                break;
            }

            case 4: {
                char position[MAX_LINE];
                printf("Enter position : ");
                scanf(" %[^\n]", position);
                search_by_position(filename, position);
                break;
            }

            case 5:
                char name[MAX_LINE];
                printf("Enter name : ");
                scanf(" %[^\n]", name);
                deleteemployeedata(filename, name);
                break;

            case 6:
                char n[MAX_LINE];
                printf("Enter name : ");
                scanf(" %[^\n]", n);
                Updateemployeedata(filename, n);
                break;

            case 7:
                printf("Process end.\n");
                return;

            default:
                printf("Invalid input.\n");
                break;
        }
    }
}

void test_add_employee() {
    
    printf("Running test_add_employee...\n");
    // 🔹 Step 1: สร้างไฟล์ใหม่ (เริ่มจากว่าง)
    FILE *fp = fopen(Tempfile, "w");
    assert(fp != NULL);
    fclose(fp);

    // 🔹 Step 2: เรียกฟังก์ชันจริง
    add_employee(Tempfile, "Alice", "2024-05-01", "Manager", "Active");

    // 🔹 Step 3: เปิดไฟล์อ่านกลับมาตรวจสอบ
    fp = fopen(Tempfile, "r");
    assert(fp != NULL);

    char line[MAX_LINE];
    assert(fgets(line, sizeof(line), fp) != NULL);
    fclose(fp);

    // 🔹 Step 4: ตรวจสอบเนื้อหา
    // ตัด \n ออกก่อน
    line[strcspn(line, "\n")] = 0;
    assert(strcmp(line, "Alice,2024-05-01,Manager,Active") == 0);

    printf(" test_add_employee passed.\n");

    // 🔹 Step 5: ทำความสะอาดไฟล์
    remove(Tempfile);
}

void test_deleteemployeedata() {
    printf("Running test_deleteemployeedata...\n");

    // 🔹 Step 1: สร้างไฟล์ CSV สำหรับทดสอบ
    FILE *fp = fopen(Tempfile, "w");
    assert(fp != NULL);
    fprintf(fp,
        "Alice,2024-01-01,Manager,Active\n"
        "Bob,2023-05-10,Developer,Inactive\n"
        "Charlie,2022-03-22,Tester,Active\n");
    fclose(fp);

    // 🔹 Step 2: ลบ Bob ออก
    deleteemployeedata(Tempfile, "Bob");

    // 🔹 Step 3: เปิดไฟล์กลับมาอ่านเพื่อตรวจสอบผล
    fp = fopen(Tempfile, "r");
    assert(fp != NULL);

    char buf[MAX_LINE];
    int found_bob = 0;
    int total_lines = 0;
    while (fgets(buf, sizeof(buf), fp)) {
        total_lines++;
        if (strstr(buf, "Bob") != NULL) {
            found_bob = 1; // เจอ Bob (ไม่ควรจะมี)
        }
    }
    fclose(fp);

    // 🔹 Step 4: ตรวจสอบผลลัพธ์
    assert(found_bob == 0);  // ไม่ควรเจอ Bob แล้ว
    assert(total_lines == 2); // ควรเหลือ 2 คน

    printf(" test_deleteemployeedata passed!\n");

    // 🔹 Step 5: ล้างไฟล์ทดสอบ
    remove(Tempfile);
}


int main() {
    test_deleteemployeedata();
    test_add_employee(); 
    process();
    return 0;
}
