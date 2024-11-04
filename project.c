// Hospital Management System
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#define MAX_DOCTORS 50
#define MAX_PATIENTS 100
#define MAX_USERS 100
struct User {
    char username[50];
    char password[50];
};
struct patient {
    int id;
    char patientName[100];
    char patientAddress[100];
    char disease[100];
    char date[52];
    char category[50];
} p;
struct doctor {
    int id;
    char name[50];
    char address[50];
    char specialize[50];
    char date[12];
} d;
struct doctorStats {
    int doctorId;
    int patientCount;
};
struct User users[MAX_USERS];
int userCount = 0;
struct doctorStats doctorStatsArray[MAX_DOCTORS];
FILE *fp;
int isUsernameTaken(const char *username) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return 1;
        }
    }
    return 0;
}
void signUp() {
    printf("Sign Up\n");
    struct User newUser;

    printf("Enter Username: ");
    scanf("%s", newUser.username);

    if (isUsernameTaken(newUser.username)) {
        printf("Username already taken. Please choose a different one.\n");
        return;
    }
    char pass[50] = "";
    printf("Enter Password: ");
    while (1) {
        char c = getch();
        if (c == '\r' || c == '\n') {
            break;
        } else if (newUser.password[strlen(newUser.password) - 1] != '\b') {
            putchar('*');
            pass[strlen(pass)] = c;
        }
    }
    strcpy(newUser.password, pass);

    users[userCount++] = newUser;
    saveUsersToFile();
    printf("\nAccount created successfully. You can now sign in.\n");
}
int signIn() {
    loadUsersFromFile();
    printf("Sign In\n");
    char username[50];
    char password[50] = "";

    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    while (1) {
        char c = getch();
        if (c == '\r' || c == '\n') {
            break;
        } else if (strlen(password) < sizeof(password) - 1) {
            putchar('*');
            password[strlen(password)] = c;
        }
    }
    password[sizeof(password) - 1] = '\0';
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1;
        }
    }
    return 0;
}
void saveUsersToFile() {
    FILE *file = fopen("users.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %s\n", users[i].username, users[i].password);
    }

    fclose(file);
}
void loadUsersFromFile() {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("No user data found.\n");
        return;
    }

    while (fscanf(file, "%s %s", users[userCount].username, users[userCount].password) == 2) {
        userCount++;
    }

    fclose(file);
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void animateLoading() {
    printf("Loading");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf(".");
            fflush(stdout);
            usleep(500000);
        }
        printf("\b\b\b   \b\b\b");
    }
    printf("\n");
}

void displayMenu() {
    clearScreen();
    printf("<== Hospital Management System ==>\n");
    printf("1. Admit Patient\n");
    printf("2. Patient List\n");
    printf("3. Discharge Patient\n");
    printf("4. Add Doctor\n");
    printf("5. Doctors List\n");
    printf("0. Exit\n\n");
    printf("Enter your choice: ");
}

void admitPatient(){
    char myDate[999];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year + 1900);
    strcpy(p.date, myDate);

    fp = fopen("patient.txt", "ab");

    printf("Enter Patient id: ");
    scanf("%d", &p.id);

    printf("Enter Patient name: ");
    fflush(stdin);
    gets(p.patientName);

    printf("Enter Patient Address: ");
    fflush(stdin);
    gets(p.patientAddress);

    printf("Enter Patient Disease: ");
    fflush(stdin);
    gets(p.disease);
    printf("\nPatient Added Successfully");

    fwrite(&p, sizeof(p), 1, fp);
    fclose(fp);
}
void patientList() {
    system("cls");
    printf("<== Patient List ==>\n\n");
    printf("%-10s %-30s %-50s %-20s %s\n", "Id", "Patient Name", "Address", "Disease", "Date");
    printf("---------------------------------------------------------------------------------------------------------------------------------\n");

    fp = fopen("patient.txt", "rb");
    while (fread(&p, sizeof(p), 1, fp) == 1) {
        printf("%-10d %-30s %-50s %-20s %s\n", p.id, p.patientName, p.patientAddress, p.disease, p.date);
    }
    fclose(fp);
}

void dischargePatient(){
    int id, f=0;
    system("cls");
    printf("<== Discharge Patient ==>\n\n");
    printf("Enter Patient id to discharge: ");
    scanf("%d", &id);
    FILE *ft;
    fp = fopen("patient.txt", "rb");
    ft = fopen("temp.txt", "wb");
    while(fread(&p, sizeof(p), 1, fp) == 1){
        if(id == p.id){
            f=1;
        }else{
            fwrite(&p, sizeof(p), 1, ft);
        }
    }
    if(f==1){
        printf("\n\nPatient Discharged Successfully.");
    }else{
        printf("\n\nRecord Not Found !");
    }
    fclose(fp);
    fclose(ft);
    remove("patient.txt");
    rename("temp.txt", "patient.txt");
}
void addDoctor(){
    char myDate[999];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year + 1900);
    strcpy(d.date, myDate);
    int f=0;
    system("cls");
    printf("<== Add Doctor ==>\n\n");

    fp = fopen("doctor.txt", "ab");

    printf("Enter Doctor id: ");
    scanf("%d", &d.id);

    printf("Enter Doctor Name: ");
    fflush(stdin);
    gets(d.name);

    printf("Enter Doctor Address: ");
    fflush(stdin);
    gets(d.address);

    printf("Doctor Specialize in: ");
    fflush(stdin);
    gets(d.specialize);

    printf("Doctor Added Successfully\n\n");

    fwrite(&d, sizeof(d), 1, fp);
    fclose(fp);
}
void doctorList(){
    system("cls");
    printf("<== Doctor List ==>\n\n");

    printf("%-10s %-30s %-30s %-30s %s\n", "id", "Name", "Address", "Specialize","Date");
    printf("-------------------------------------------------------------------------------------------------------------------\n");

    fp = fopen("doctor.txt", "rb");
    while(fread(&d, sizeof(d), 1, fp) == 1){
        printf("%-10d %-30s %-30s %-30s %s\n", d.id, d.name, d.address, d.specialize, d.date);
    }

    fclose(fp);
}
int main() {
    int choice;
    int loggedIn = 0;

    while (1) {
        printf("Hospital Management System\n");
        printf("1. Sign Up\n");
        printf("2. Sign In\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                signUp();
                break;
            case 2:
                if (signIn()) {
                    printf("\nSign in successful. You are now logged in.\n");
                    loggedIn = 1;
                } else {
                    printf("\nSign in failed. Please check your username and password.\n");
                }
                break;
            case 3:
                exit(0);
            default:
                printf("\nInvalid choice. Please try again.\n");
        }

        if (loggedIn) {
            break;
        }
    }
    int ch;
    while (1) {
        displayMenu();
        scanf("%d", &ch);
        switch (ch) {
            case 0:
                exit(0);
            case 1:
                clearScreen();
                printf("Admitting a patient...\n");
                animateLoading();
                admitPatient();
                break;
            case 2:
                clearScreen();
                printf("Fetching patient list...\n");
                animateLoading();
                patientList();
                break;
            case 3:
                clearScreen();
                printf("Discharging a patient...\n");
                animateLoading();
                dischargePatient();
                break;
            case 4:
                clearScreen();
                printf("Adding a doctor...\n");
                animateLoading();
                addDoctor();
                break;
            case 5:
                clearScreen();
                printf("Fetching doctor list...\n");
                animateLoading();
                doctorList();
                break;
            default:
                printf("Invalid Choice...\n\n");
        }
        printf("\nPress Enter To Continue...");
        getchar();
    }

    return 0;
}

