#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

COORD coord = {0, 0};

void gotoxy(int x, int y)
{
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetColor(int textColor, int bgColor)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgColor << 4) | textColor);
}

void hideCursor()
{
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void displayM(int selected)
{
    char arr[6][20] = // Updated to 6 for the new "Display File Contents" option
    {
        "Add_New_Student",
        "Update_Student",
        "Calculate_Grades",
        "Display_Results",
        "Display_File_Contents", // New option
        "Exit"
    };
    int n = 6; // Updated to 6 for the new option
    int boxWidth = 30;
    int startX = 45;
    int startY = 5;

    gotoxy(startX, startY);
    SetColor(12, 0);
    for (int i = 0; i < boxWidth; i++)
        printf("-");

    for (int i = 0; i < n; i++)
    {
        gotoxy(startX, startY + i + 1);
        printf("|");
        gotoxy(startX + boxWidth - 1, startY + i + 1);
        printf("|");

        gotoxy(startX + 2, startY + i + 1);
        if (i == selected)
        {
            SetColor(12, 0);
            printf(" %s ", arr[i]);
        }
        else
        {
            SetColor(9, 0);
            printf(" %s ", arr[i]);
        }
    }

    gotoxy(startX, startY + n + 1);
    SetColor(12, 0);
    for (int i = 0; i < boxWidth; i++)
        printf("-");
}

void displayWelcome()
{
    system("cls");
    gotoxy(60, 3);

    SetColor(9, 0);
    printf(" \n\n\n\n\n\t\t\t\t\t     *  *  *  *  *  *  *  *");
    printf("\n\n\t\t\t\t\t     *                    *");
    printf("\n\n\t\t\t\t\t     *       Welcome      *");
    printf("\n\n\t\t\t\t\t     *                    *");
    printf("\n\n\t\t\t\t\t     *  *  *  *  *  *  *  *");
    SetColor(7, 0);
    getch();
}

void displayHeader()
{
    SetColor(9, 0);
    gotoxy(30, 2);
    printf("\t\t\tStudent management system\t\t");
    SetColor(7, 0);
}

struct Students
{
    int id;
    char name[10];
    double *scores;
    char **subjects;
    int numSubject;
};
typedef struct Students student;

#define FILENAME "students.dat"

void saveStudentsToFile(student *s1, int studentCount)
{
    FILE *file = fopen(FILENAME, "wb"); // Open file in write-binary mode
    if (file == NULL)
    {
        printf("Error opening file for writing!\n");
        return;
    }

    // Write the number of students first
    fwrite(&studentCount, sizeof(int), 1, file);

    // Write each student's data
    for (int i = 0; i < studentCount; i++)
    {
        fwrite(&s1[i].id, sizeof(int), 1, file);
        fwrite(s1[i].name, sizeof(char), 10, file);
        fwrite(&s1[i].numSubject, sizeof(int), 1, file);

        // Write subjects and scores
        for (int j = 0; j < s1[i].numSubject; j++)
        {
            fwrite(s1[i].subjects[j], sizeof(char), 30, file);
            fwrite(&s1[i].scores[j], sizeof(double), 1, file);
        }
    }

    fclose(file); // Close the file
    printf("Data saved to file successfully!\n");
}

void loadStudentsFromFile(student **s1, int *studentCount)
{
    FILE *file = fopen(FILENAME, "rb"); // Open file in read-binary mode
    if (file == NULL)
    {
        printf("No existing data found. Starting with an empty list.\n");
        return;
    }

    // Read the number of students first
    fread(studentCount, sizeof(int), 1, file);

    // Allocate memory for students
    *s1 = (student *)malloc(*studentCount * sizeof(student));

    // Read each student's data
    for (int i = 0; i < *studentCount; i++)
    {
        fread(&(*s1)[i].id, sizeof(int), 1, file);
        fread((*s1)[i].name, sizeof(char), 10, file);
        fread(&(*s1)[i].numSubject, sizeof(int), 1, file);

        // Allocate memory for subjects and scores
        (*s1)[i].subjects = (char **)malloc((*s1)[i].numSubject * sizeof(char *));
        (*s1)[i].scores = (double *)malloc((*s1)[i].numSubject * sizeof(double));

        // Read subjects and scores
        for (int j = 0; j < (*s1)[i].numSubject; j++)
        {
            (*s1)[i].subjects[j] = (char *)malloc(30 * sizeof(char));
            fread((*s1)[i].subjects[j], sizeof(char), 30, file);
            fread(&(*s1)[i].scores[j], sizeof(double), 1, file);
        }
    }

    fclose(file); // Close the file
    printf("Data loaded from file successfully!\n");
}

void displayFileContents()
{
    FILE *file = fopen(FILENAME, "rb"); // Open file in read-binary mode
    if (file == NULL)
    {
        printf("No data file found or error opening file!\n");
        return;
    }

    int studentCount;
    fread(&studentCount, sizeof(int), 1, file); // Read the number of students

    printf("Total Students: %d\n", studentCount);
    printf("---------------------------------\n");

    for (int i = 0; i < studentCount; i++)
    {
        int id;
        char name[10];
        int numSubject;

        // Read student data
        fread(&id, sizeof(int), 1, file);
        fread(name, sizeof(char), 10, file);
        fread(&numSubject, sizeof(int), 1, file);

        printf("Student ID: %d\n", id);
        printf("Name: %s\n", name);
        printf("Number of Subjects: %d\n", numSubject);

        // Read subjects and scores
        for (int j = 0; j < numSubject; j++)
        {
            char subject[30];
            double score;

            fread(subject, sizeof(char), 30, file);
            fread(&score, sizeof(double), 1, file);

            printf("  Subject: %s\n", subject);
            printf("  Score: %.2f\n", score);
        }

        printf("---------------------------------\n");
    }

    fclose(file); // Close the file
}

void addStudents(student **s1, int *studentCount)
{
    *s1 = (student *)realloc(*s1, (*studentCount + 1) * sizeof(student));
    student *newStudent = &(*s1)[*studentCount];
    system("cls");
    gotoxy(30, 0);
    SetColor(12, 0);
    printf("                             Add Student                          \n\n");
    SetColor(9, 0);

    printf("Enter student ID: ");
    scanf("%d", &newStudent->id);
    printf("Enter student name: ");
    scanf(" %[^\n]s", newStudent->name);

    printf("Enter number of subjects: ");
    scanf("%d", &newStudent->numSubject);

    newStudent->subjects = (char **)malloc(newStudent->numSubject * sizeof(char *));
    newStudent->scores = (double *)malloc(newStudent->numSubject * sizeof(double));

    for (int i = 0; i < newStudent->numSubject; i++)
    {
        newStudent->subjects[i] = (char *)malloc(30 * sizeof(char));
        printf("Enter subject %d name: ", i + 1);
        scanf(" %[^\n]s", newStudent->subjects[i]);
        printf("Enter score for %s: ", newStudent->subjects[i]);
        scanf("%lf", &newStudent->scores[i]);
    }

    (*studentCount)++;
    SetColor(10, 0);
    printf("Student added successfully!\n");

    // Save data to file after adding a student
    saveStudentsToFile(*s1, *studentCount);

    SetColor(7, 0);
    printf("Press any key to continue...\n");
    getch();
}

void updata(student *s1, int studentCount)
{
    system("cls");
    gotoxy(30, 0);
    SetColor(12, 0);
    printf("                                Update Student                         \n\n");
    SetColor(9, 0);

    int id;
    printf("Enter the ID of the student to update: ");
    scanf("%d", &id);

    int found = -1;
    for (int i = 0; i < studentCount; i++)
    {
        if (s1[i].id == id)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        SetColor(12, 0);
        printf("No student found with this ID.\n");
        SetColor(7, 0);
        printf("Press any key to return to the menu...\n");
        getch();
        return;
    }

    printf("[1] Edit name\n[2] Edit subjects\n");
    printf("Enter your choice: ");
    int choice;
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("Enter new name: ");
        scanf(" %[^\n]s", s1[found].name);
        SetColor(10, 0);
        printf("Name updated successfully!\n");
    }
    else if (choice == 2)
    {
        for (int i = 0; i < s1[found].numSubject; i++)
        {
            free(s1[found].subjects[i]);
        }
        free(s1[found].subjects);
        free(s1[found].scores);

        printf("Enter new number of subjects: ");
        scanf("%d", &s1[found].numSubject);

        s1[found].subjects = (char **)malloc(s1[found].numSubject * sizeof(char *));
        s1[found].scores = (double *)malloc(s1[found].numSubject * sizeof(double));

        for (int i = 0; i < s1[found].numSubject; i++)
        {
            s1[found].subjects[i] = (char *)malloc(30 * sizeof(char));
            printf("Enter subject %d name: ", i + 1);
            scanf(" %[^\n]s", s1[found].subjects[i]);
            printf("Enter score for %s: ", s1[found].subjects[i]);
            scanf("%lf", &s1[found].scores[i]);
        }

        SetColor(10, 0);
        printf("Subjects and scores updated successfully!\n");
    }
    else
    {
        SetColor(12, 0);
        printf("Invalid choice.\n");
    }

    // Save data to file after updating a student
    saveStudentsToFile(s1, studentCount);

    SetColor(7, 0);
    printf("Press any key to return to the menu...\n");
    getch();
}

void calc(student *s1, int studentCount)
{
    system("cls");
    gotoxy(30, 0);
    SetColor(12, 0);
    printf("                              Calculate Grades                           \n\n");
    SetColor(9, 0);
    double sum = 0, average;
    for (int i = 0; i < studentCount; i++)
    {
        sum = 0;
        for (int j = 0; j < s1[i].numSubject; j++)
        {
            sum += s1[i].scores[j];
        }

        average = sum / s1[i].numSubject;

        char grade;
        if (average >= 90)
        {
            grade = 'A';
        }
        else if (average >= 80)
        {
            grade = 'B';
        }
        else if (average >= 70)
        {
            grade = 'C';
        }
        else if (average >= 60)
        {
            grade = 'D';
        }
        else
        {
            grade = 'F';
        }

        printf("Student ID: %d\n", s1[i].id);
        printf("Name: %s\n", s1[i].name);
        printf("Average Score: %.2f\n", average);
        printf("Grade: %c\n\n", grade);
    }
    printf("Press any key to return to the menu...\n");
    getch();
}

int main()
{
    int studentCount = 0;
    student *s1 = NULL;

    int selected = 0;
    int n = 6; // Updated to 6 for the new "Display File Contents" option
    char ch;

    hideCursor();

    displayWelcome();

    // Load existing data from file
    loadStudentsFromFile(&s1, &studentCount);

    while (1)
    {
        system("cls");
        displayHeader();
        displayM(selected);

        ch = getch();
        if (ch == -32)
        {
            ch = getch();
            switch (ch)
            {
            case 72:
                selected = (selected - 1 + n) % n;
                break;
            case 80:
                selected = (selected + 1) % n;
                break;
            }
        }
        else if (ch == 13)
        {
            if (selected == 0)
            {
                addStudents(&s1, &studentCount);
            }
            else if (selected == 1)
            {
                updata(s1, studentCount);
            }
            else if (selected == 2)
            {
                calc(s1, studentCount);
            }
            else if (selected == 3)
            {
                system("cls");
                gotoxy(30, 0);
                SetColor(12, 0);
                printf("                             Display Results                           \n\n");
                SetColor(9, 0);

                if (studentCount == 0)
                {
                    printf("No students available to display.\n");
                }
                else
                {
                    for (int i = 0; i < studentCount; i++)
                    {
                        printf("Student ID: %d\n", s1[i].id);
                        printf("Name: %s\n", s1[i].name);
                        double total = 0;
                        for (int j = 0; j < s1[i].numSubject; j++)
                        {
                            total += s1[i].scores[j];
                        }
                        double average = total / s1[i].numSubject;
                        printf("Average Score: %.2f\n", average);
                        printf("-------------------------\n");
                    }
                }

                SetColor(7, 0);
                printf("Press to back \n");
                getch();
            }
            else if (selected == 4)
            {
                system("cls");
                gotoxy(30, 0);
                SetColor(12, 0);
                printf("                             Display File Contents                           \n\n");
                SetColor(9, 0);

                displayFileContents(); // Call the function to display file contents

                SetColor(7, 0);
                printf("Press any key to return to the menu...\n");
                getch();
            }
            else if (selected == 5)
            {
                // Save data to file before exiting
                saveStudentsToFile(s1, studentCount);

                // Free allocated memory
                for (int i = 0; i < studentCount; i++)
                {
                    for (int j = 0; j < s1[i].numSubject; j++)
                    {
                        free(s1[i].subjects[j]);
                    }
                    free(s1[i].subjects);
                    free(s1[i].scores);
                }
                free(s1);

                // Exit the program
                break;
            }
        }
    }

    return 0;
}
