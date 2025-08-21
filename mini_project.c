#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char student_id[20];
    char name[50];
    char course[50];
    int total_classes;
    int present;
    int absent;
    int late;
    char password[20];
} Student;

typedef struct {
    char student_id[20];
    char date[20];
    char status;
} Attendance;

void adminLogin();
void adminMenu();
void studentLogin();
void studentMenu(char student_id[]);
void addStudent();
void markAttendance();
void viewStudentAttendance();
void generateReports();
void viewOwnAttendance(char student_id[]);
void saveStudentsToFile(Student students[], int count);
void saveAttendanceToFile(Attendance records[], int count);
int loadStudentsFromFile(Student students[]);
int loadAttendanceFromFile(Attendance records[]);
int findStudentById(Student students[], int count, char id[]);
void changeStudentPassword(char student_id[]);
void deleteStudent();
void modifyAttendance();

int main() {
    int choice;
    
    do {
        printf("\n\nSTUDENT ATTENDANCE MANAGEMENT SYSTEM\n");
        printf("------------------------------------\n");
        printf("1. Admin Login\n");
        printf("2. Student Login\n");
        printf("3. Exit\n");
        printf("------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                adminLogin();
                break;
            case 2:
                studentLogin();
                break;
            case 3:
                printf("\nExiting system...\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    } while(choice != 3);
    
    return 0;
}

void adminLogin() {
    char username[20], password[20];
    const char admin_username[] = "admin";
    const char admin_password[] = "admin123";
    
    printf("\nADMIN LOGIN\n");
    printf("-----------\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);
    
    if(strcmp(username, admin_username) == 0 && strcmp(password, admin_password) == 0) {
        printf("\nLogin successful!\n");
        adminMenu();
    } else {
        printf("\nInvalid username or password.\n");
    }
}

void adminMenu() {
    int choice;
    
    do {
        printf("\nADMIN MENU\n");
        printf("----------\n");
        printf("1. Add New Student\n");
        printf("2. Mark Attendance\n");
        printf("3. View Student Attendance\n");
        printf("4. Generate Reports\n");
        printf("5. Delete Student\n");
        printf("6. Modify Attendance\n");
        printf("7. Back to Main Menu\n");
        printf("-------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                addStudent();
                break;
            case 2:
                markAttendance();
                break;
            case 3:
                viewStudentAttendance();
                break;
            case 4:
                generateReports();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                modifyAttendance();
                break;
            case 7:
                printf("\nReturning to main menu...\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    } while(choice != 7);
}

void studentLogin() {
    Student students[100];
    int count = loadStudentsFromFile(students);
    char student_id[20], password[20];
    int found = 0;
    
    printf("\nSTUDENT LOGIN\n");
    printf("-------------\n");
    printf("Student ID: ");
    scanf("%s", student_id);
    printf("Password: ");
    scanf("%s", password);
    
    for(int i = 0; i < count; i++) {
        if(strcmp(students[i].student_id, student_id) == 0 && 
           strcmp(students[i].password, password) == 0) {
            found = 1;
            printf("\nLogin successful!\n");
            studentMenu(student_id);
            break;
        }
    }
    
    if(!found) {
        printf("\nInvalid student ID or password.\n");
    }
}

void studentMenu(char student_id[]) {
    int choice;
    
    do {
        printf("\nSTUDENT MENU\n");
        printf("------------\n");
        printf("1. View My Attendance\n");
        printf("2. Change Password\n");
        printf("3. Back to Main Menu\n");
        printf("-------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                viewOwnAttendance(student_id);
                break;
            case 2:
                changeStudentPassword(student_id);
                break;
            case 3:
                printf("\nReturning to main menu...\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    } while(choice != 3);
}

void addStudent() {
    Student students[100];
    int count = loadStudentsFromFile(students);
    Student new_student;
    
    printf("\nADD NEW STUDENT\n");
    printf("---------------\n");
    printf("Student ID: ");
    scanf("%s", new_student.student_id);
    
    for(int i = 0; i < count; i++) {
        if(strcmp(students[i].student_id, new_student.student_id) == 0) {
            printf("\nError: Student ID already exists.\n");
            return;
        }
    }
    
    printf("Name: ");
    scanf(" %[^\n]s", new_student.name);
    printf("Course: ");
    scanf(" %[^\n]s", new_student.course);
    printf("Set password for student: ");
    scanf("%s", new_student.password);
    
    new_student.total_classes = 0;
    new_student.present = 0;
    new_student.absent = 0;
    new_student.late = 0;
    
    students[count] = new_student;
    count++;
    
    saveStudentsToFile(students, count);
    printf("\nStudent added successfully!\n");
}

void deleteStudent() {
    Student students[100];
    Attendance records[1000], new_records[1000];
    int student_count = loadStudentsFromFile(students);
    int attendance_count = loadAttendanceFromFile(records);
    char student_id[20];
    int found = 0, new_count = 0;
    
    if(student_count == 0) {
        printf("\nNo students registered yet.\n");
        return;
    }
    
    printf("\nDELETE STUDENT\n");
    printf("-------------\n");
    printf("Enter Student ID to delete: ");
    scanf("%s", student_id);
    
    // Remove student from students array
    for(int i = 0; i < student_count; i++) {
        if(strcmp(students[i].student_id, student_id) == 0) {
            found = 1;
            printf("\nDeleting student: %s (%s)\n", students[i].name, students[i].student_id);
            
            // Shift remaining elements
            for(int j = i; j < student_count - 1; j++) {
                students[j] = students[j + 1];
            }
            student_count--;
            break;
        }
    }
    
    if(!found) {
        printf("\nStudent ID not found.\n");
        return;
    }
    
    // Remove student's attendance records
    for(int i = 0; i < attendance_count; i++) {
        if(strcmp(records[i].student_id, student_id) != 0) {
            new_records[new_count++] = records[i];
        }
    }
    
    saveStudentsToFile(students, student_count);
    saveAttendanceToFile(new_records, new_count);
    printf("\nStudent deleted successfully!\n");
}

void modifyAttendance() {
    Student students[100];
    Attendance records[1000];
    int student_count = loadStudentsFromFile(students);
    int attendance_count = loadAttendanceFromFile(records);
    char student_id[20], date[20];
    char new_status;
    int found = 0;
    
    if(student_count == 0) {
        printf("\nNo students registered yet.\n");
        return;
    }
    
    printf("\nMODIFY ATTENDANCE\n");
    printf("----------------\n");
    printf("Enter Student ID: ");
    scanf("%s", student_id);
    printf("Enter Date (DD-MM-YYYY): ");
    scanf("%s", date);
    
    // Find the student
    int student_index = -1;
    for(int i = 0; i < student_count; i++) {
        if(strcmp(students[i].student_id, student_id) == 0) {
            student_index = i;
            break;
        }
    }
    
    if(student_index == -1) {
        printf("\nStudent ID not found.\n");
        return;
    }
    
    // Find the attendance record
    for(int i = 0; i < attendance_count; i++) {
        if(strcmp(records[i].student_id, student_id) == 0 && 
           strcmp(records[i].date, date) == 0) {
            found = 1;
            
            // Decrement the old status count
            if(records[i].status == 'P') {
                students[student_index].present--;
            } else if(records[i].status == 'A') {
                students[student_index].absent--;
            } else if(records[i].status == 'L') {
                students[student_index].late--;
            }
            
            printf("\nCurrent status: ");
            if(records[i].status == 'P') printf("Present");
            else if(records[i].status == 'A') printf("Absent");
            else if(records[i].status == 'L') printf("Late");
            
            printf("\nEnter new status (P for Present, A for Absent, L for Late): ");
            scanf(" %c", &new_status);
            new_status = toupper(new_status);
            
            while(new_status != 'P' && new_status != 'A' && new_status != 'L') {
                printf("Invalid choice. Enter P, A or L: ");
                scanf(" %c", &new_status);
                new_status = toupper(new_status);
            }
            
            // Update the record and increment new status count
            records[i].status = new_status;
            if(new_status == 'P') {
                students[student_index].present++;
            } else if(new_status == 'A') {
                students[student_index].absent++;
            } else if(new_status == 'L') {
                students[student_index].late++;
            }
            
            break;
        }
    }
    
    if(!found) {
        printf("\nAttendance record not found for this student on %s\n", date);
        return;
    }
    
    saveStudentsToFile(students, student_count);
    saveAttendanceToFile(records, attendance_count);
    printf("\nAttendance modified successfully!\n");
}

void markAttendance() {
    Student students[100];
    Attendance records[1000];
    int student_count = loadStudentsFromFile(students);
    int attendance_count = loadAttendanceFromFile(records);
    char date[20];
    char choice;
    
    if(student_count == 0) {
        printf("\nNo students registered yet.\n");
        return;
    }
    
    printf("\nMARK ATTENDANCE\n");
    printf("--------------\n");
    printf("Enter date (DD-MM-YYYY): ");
    scanf("%s", date);
    
    // Check if attendance already marked for this date
    int date_used = 0;
    for(int i = 0; i < attendance_count; i++) {
        if(strcmp(records[i].date, date) == 0) {
            date_used = 1;
            break;
        }
    }
    
    if(date_used) {
        printf("\nAttendance already marked for this date. Use Modify Attendance option instead.\n");
        return;
    }
    
    for(int i = 0; i < student_count; i++) {
        printf("\nStudent: %s (%s)\n", students[i].name, students[i].student_id);
        printf("Status (P for Present, A for Absent, L for Late): ");
        scanf(" %c", &choice);
        
        choice = toupper(choice);
        while(choice != 'P' && choice != 'A' && choice != 'L') {
            printf("Invalid choice. Enter P, A or L: ");
            scanf(" %c", &choice);
            choice = toupper(choice);
        }
        
        students[i].total_classes++;
        if(choice == 'P') students[i].present++;
        else if(choice == 'A') students[i].absent++;
        else if(choice == 'L') students[i].late++;
        
        strcpy(records[attendance_count].student_id, students[i].student_id);
        strcpy(records[attendance_count].date, date);
        records[attendance_count].status = choice;
        attendance_count++;
    }
    
    saveStudentsToFile(students, student_count);
    saveAttendanceToFile(records, attendance_count);
    printf("\nAttendance marked successfully for date %s\n", date);
}

void viewStudentAttendance() {
    Student students[100];
    Attendance records[1000];
    int student_count = loadStudentsFromFile(students);
    int attendance_count = loadAttendanceFromFile(records);
    char student_id[20];
    int found = 0;
    
    if(student_count == 0) {
        printf("\nNo students registered yet.\n");
        return;
    }
    
    printf("\nVIEW STUDENT ATTENDANCE\n");
    printf("-----------------------\n");
    printf("Enter Student ID: ");
    scanf("%s", student_id);
    
    for(int i = 0; i < student_count; i++) {
        if(strcmp(students[i].student_id, student_id) == 0) {
            found = 1;
            printf("\nATTENDANCE RECORD\n");
            printf("----------------------------------------\n");
            printf("%-15s: %s\n", "Name", students[i].name);
            printf("%-15s: %s\n", "Student ID", student_id);
            printf("%-15s: %s\n", "Course", students[i].course);
            printf("%-15s: %d\n", "Total Classes", students[i].total_classes);
            printf("%-15s: %d\n", "Present", students[i].present);
            printf("%-15s: %d\n", "Absent", students[i].absent);
            printf("%-15s: %d\n", "Late", students[i].late);
            
            float percentage = 0;
            if(students[i].total_classes > 0) {
                percentage = ((float)(students[i].present + students[i].late * 0.5) / students[i].total_classes) * 100;
            }
            printf("%-15s: %.2f%%\n", "Percentage", percentage);
            
            if(percentage < 75) {
                printf("\nWARNING: Attendance below 75%%\n");
            }
            
            printf("\nDETAILED ATTENDANCE\n");
            printf("------------------\n");
            printf("%-12s %-10s\n", "Date", "Status");
            printf("--------------------\n");
            
            for(int j = 0; j < attendance_count; j++) {
                if(strcmp(records[j].student_id, student_id) == 0) {
                    char status[10];
                    if(records[j].status == 'P') strcpy(status, "Present");
                    else if(records[j].status == 'A') strcpy(status, "Absent");
                    else if(records[j].status == 'L') strcpy(status, "Late");
                    
                    printf("%-12s %-10s\n", records[j].date, status);
                }
            }
            break;
        }
    }
    
    if(!found) {
        printf("\nStudent ID not found.\n");
    }
}

void generateReports() {
    Student students[100];
    int student_count = loadStudentsFromFile(students);
    float threshold;
    
    if(student_count == 0) {
        printf("\nNo students registered yet.\n");
        return;
    }
    
    printf("\nGENERATE REPORTS\n");
    printf("---------------\n");
    printf("Enter attendance threshold percentage (e.g., 75): ");
    scanf("%f", &threshold);
    
    printf("\nATTENDANCE REPORT\n");
    printf("------------------------------------------------------------------------------------------------\n");
    printf("%-15s %-20s %-15s %-6s %-8s %-8s %-6s %-10s\n", 
           "Student ID", "Name", "Course", "Total", "Present", "Absent", "Late", "Percentage");
    printf("------------------------------------------------------------------------------------------------\n");
    
    for(int i = 0; i < student_count; i++) {
        float percentage = 0;
        if(students[i].total_classes > 0) {
            percentage = ((float)(students[i].present + students[i].late * 0.5) / students[i].total_classes) * 100;
        }
        
        printf("%-15s %-20s %-15s %-6d %-8d %-8d %-6d %-10.2f%%\n", 
               students[i].student_id, 
               students[i].name, 
               students[i].course,
               students[i].total_classes,
               students[i].present,
               students[i].absent,
               students[i].late,
               percentage);
    }
    
    printf("\nDEFAULTER LIST (Below %.2f%% attendance)\n", threshold);
    printf("-------------------------------------------------\n");
    printf("%-15s %-20s %-10s\n", "Student ID", "Name", "Percentage");
    printf("-------------------------------------------------\n");
    
    int defaulters = 0;
    for(int i = 0; i < student_count; i++) {
        float percentage = 0;
        if(students[i].total_classes > 0) {
            percentage = ((float)(students[i].present + students[i].late * 0.5) / students[i].total_classes) * 100;
        }
        
        if(percentage < threshold) {
            printf("%-15s %-20s %-10.2f%%\n", 
                   students[i].student_id, 
                   students[i].name, 
                   percentage);
            defaulters++;
        }
    }
    
    if(defaulters == 0) {
        printf("No defaulters found.\n");
    }
}

void viewOwnAttendance(char student_id[]) {
    Student students[100];
    Attendance records[1000];
    int student_count = loadStudentsFromFile(students);
    int attendance_count = loadAttendanceFromFile(records);
    
    for(int i = 0; i < student_count; i++) {
        if(strcmp(students[i].student_id, student_id) == 0) {
            printf("\nYOUR ATTENDANCE RECORD\n");
            printf("----------------------------------------\n");
            printf("%-15s: %s\n", "Name", students[i].name);
            printf("%-15s: %s\n", "Student ID", students[i].student_id);
            printf("%-15s: %s\n", "Course", students[i].course);
            printf("%-15s: %d\n", "Total Classes", students[i].total_classes);
            printf("%-15s: %d\n", "Present", students[i].present);
            printf("%-15s: %d\n", "Absent", students[i].absent);
            printf("%-15s: %d\n", "Late", students[i].late);
            
            float percentage = 0;
            if(students[i].total_classes > 0) {
                percentage = ((float)(students[i].present + students[i].late * 0.5) / students[i].total_classes) * 100;
            }
            printf("%-15s: %.2f%%\n", "Percentage", percentage);
            
            if(percentage < 75) {
                printf("\nWARNING: Your attendance is below 75%%\n");
            }
            
            printf("\nDETAILED ATTENDANCE\n");
            printf("------------------\n");
            printf("%-12s %-10s\n", "Date", "Status");
            printf("--------------------\n");
            
            for(int j = 0; j < attendance_count; j++) {
                if(strcmp(records[j].student_id, student_id) == 0) {
                    char status[10];
                    if(records[j].status == 'P') strcpy(status, "Present");
                    else if(records[j].status == 'A') strcpy(status, "Absent");
                    else if(records[j].status == 'L') strcpy(status, "Late");
                    
                    printf("%-12s %-10s\n", records[j].date, status);
                }
            }
            return;
        }
    }
    
    printf("\nError: Student record not found.\n");
}

void changeStudentPassword(char student_id[]) {
    Student students[100];
    int count = loadStudentsFromFile(students);
    char new_password[20], confirm_password[20];
    
    for(int i = 0; i < count; i++) {
        if(strcmp(students[i].student_id, student_id) == 0) {
            printf("\nCHANGE PASSWORD\n");
            printf("---------------\n");
            printf("Enter new password: ");
            scanf("%s", new_password);
            printf("Confirm new password: ");
            scanf("%s", confirm_password);
            
            if(strcmp(new_password, confirm_password) == 0) {
                strcpy(students[i].password, new_password);
                saveStudentsToFile(students, count);
                printf("\nPassword changed successfully!\n");
            } else {
                printf("\nPasswords do not match. Password not changed.\n");
            }
            return;
        }
    }
    
    printf("\nError: Student record not found.\n");
}

void saveStudentsToFile(Student students[], int count) {
    FILE *file = fopen("students.dat", "wb");
    if(file == NULL) {
        printf("\nError opening file for writing.\n");
        return;
    }
    
    fwrite(&count, sizeof(int), 1, file);
    fwrite(students, sizeof(Student), count, file);
    fclose(file);
}

void saveAttendanceToFile(Attendance records[], int count) {
    FILE *file = fopen("attendance.dat", "wb");
    if(file == NULL) {
        printf("\nError opening file for writing.\n");
        return;
    }
    
    fwrite(&count, sizeof(int), 1, file);
    fwrite(records, sizeof(Attendance), count, file);
    fclose(file);
}

int loadStudentsFromFile(Student students[]) {
    FILE *file = fopen("students.dat", "rb");
    if(file == NULL) {
        return 0;
    }
    
    int count;
    fread(&count, sizeof(int), 1, file);
    fread(students, sizeof(Student), count, file);
    fclose(file);
    
    return count;
}

int loadAttendanceFromFile(Attendance records[]) {
    FILE *file = fopen("attendance.dat", "rb");
    if(file == NULL) {
        return 0;
    }
    
    int count;
    fread(&count, sizeof(int), 1, file);
    fread(records, sizeof(Attendance), count, file);
    fclose(file);
    
    return count;
}

int findStudentById(Student students[], int count, char id[]) {
    for(int i = 0; i < count; i++) {
        if(strcmp(students[i].student_id, id) == 0) {
            return i;
        }
    }
    return -1;
}