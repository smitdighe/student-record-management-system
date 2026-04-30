#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME     50
#define DATA_FILE    "students.dat"
#define TEMP_FILE    "temp.dat"

typedef struct {
    int  id;
    char name[MAX_NAME];
    float marks;
} Student;

void add_student(void);
void view_all_students(void);
void search_student(void);
void update_student(void);
void delete_student(void);
void sort_students(void);
void display_topper(void);
void display_menu(void);

int  is_duplicate_id(int id);
int  count_records(void);
int  read_valid_id(const char *prompt);
void read_valid_name(const char *prompt, char *dest, int size);
float read_valid_marks(const char *prompt);
void clear_input_buffer(void);
void pause_screen(void);
void print_header(void);
void print_student_row(const Student *s);
void print_separator(void);

int cmp_marks_desc(const void *a, const void *b);
int cmp_marks_asc(const void *a, const void *b);
int cmp_name_asc(const void *a, const void *b);
int cmp_name_desc(const void *a, const void *b);

int main(void) {
    int choice;

    while (1) {
        display_menu();
        printf("  Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            printf("\n  [!] Invalid input. Please enter a number.\n");
            pause_screen();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1: add_student();       break;
            case 2: view_all_students(); break;
            case 3: search_student();    break;
            case 4: update_student();    break;
            case 5: delete_student();    break;
            case 6: sort_students();     break;
            case 7: display_topper();    break;
            case 0:
                printf("\n  Goodbye! Records saved in '%s'.\n\n", DATA_FILE);
                return 0;
            default:
                printf("\n  [!] Invalid choice. Try again.\n");
                pause_screen();
        }
    }
    return 0;
}

void display_menu(void) {
    system("cls || clear");
    printf("\n");
    printf("  ╔══════════════════════════════════════════╗\n");
    printf("  ║   STUDENT RECORD MANAGEMENT SYSTEM       ║\n");
    printf("  ╠══════════════════════════════════════════╣\n");
    printf("  ║  1. Add Student                          ║\n");
    printf("  ║  2. View All Students                    ║\n");
    printf("  ║  3. Search Student by ID                 ║\n");
    printf("  ║  4. Update Student Details               ║\n");
    printf("  ║  5. Delete Student                       ║\n");
    printf("  ║  6. Sort Students                        ║\n");
    printf("  ║  7. Display Topper                       ║\n");
    printf("  ║  0. Exit                                 ║\n");
    printf("  ╚══════════════════════════════════════════╝\n\n");
}

void print_separator(void) {
    printf("  +-------+----------------------------------+--------+\n");
}

void print_header(void) {
    printf("\n");
    print_separator();
    printf("  | %-5s | %-32s | %-6s |\n", "ID", "Name", "Marks");
    print_separator();
}

void print_student_row(const Student *s) {
    printf("  | %-5d | %-32s | %6.2f |\n", s->id, s->name, s->marks);
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pause_screen(void) {
    printf("\n  Press Enter to continue...");
    getchar();
}

int read_valid_id(const char *prompt) {
    int id;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &id) != 1 || id <= 0) {
            clear_input_buffer();
            printf("  [!] Invalid ID. Enter a positive integer.\n");
            continue;
        }
        clear_input_buffer();
        return id;
    }
}

void read_valid_name(const char *prompt, char *dest, int size) {
    while (1) {
        printf("%s", prompt);
        if (fgets(dest, size, stdin) == NULL) {
            dest[0] = '\0';
            continue;
        }
        
        dest[strcspn(dest, "\n")] = '\0';
        if (strlen(dest) == 0) {
            printf("  [!] Name cannot be empty.\n");
            continue;
        }

        int valid = 1;
        for (int i = 0; dest[i]; i++) {
            if (!isalpha((unsigned char)dest[i]) && dest[i] != ' '
                && dest[i] != '.' && dest[i] != '-') {
                valid = 0;
                break;
            }
        }
        if (!valid) {
            printf("  [!] Name should contain only letters, spaces, dots, or hyphens.\n");
            continue;
        }
        return;
    }
}

float read_valid_marks(const char *prompt) {
    float marks;
    while (1) {
        printf("%s", prompt);
        if (scanf("%f", &marks) != 1 || marks < 0.0f || marks > 100.0f) {
            clear_input_buffer();
            printf("  [!] Invalid marks. Enter a value between 0 and 100.\n");
            continue;
        }
        clear_input_buffer();
        return marks;
    }
}

int is_duplicate_id(int id) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) return 0;

    Student s;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int count_records(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) return 0;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);

    return (int)(size / sizeof(Student));
}

void add_student(void) {
    Student s;

    printf("\n  ── Add New Student ──────────────────────\n\n");

    s.id = read_valid_id("  Enter Student ID : ");

    if (is_duplicate_id(s.id)) {
        printf("\n  [!] Student with ID %d already exists.\n", s.id);
        pause_screen();
        return;
    }

    read_valid_name("  Enter Student Name : ", s.name, MAX_NAME);
    s.marks = read_valid_marks("  Enter Marks (0-100): ");

    FILE *fp = fopen(DATA_FILE, "ab");
    if (!fp) {
        printf("\n  [!] Error: Could not open data file for writing.\n");
        pause_screen();
        return;
    }

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    printf("\n  [✓] Student added successfully!\n");
    pause_screen();
}

void view_all_students(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        printf("\n  [!] No records found. Add a student first.\n");
        pause_screen();
        return;
    }

    Student s;
    int count = 0;
    print_header();

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        print_student_row(&s);
        count++;
    }

    print_separator();
    printf("  Total records: %d\n", count);
    fclose(fp);
    pause_screen();
}

void search_student(void) {
    printf("\n  ── Search Student ───────────────────────\n\n");

    int id = read_valid_id("  Enter Student ID to search: ");

    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        printf("\n  [!] No records found.\n");
        pause_screen();
        return;
    }

    Student s;
    int found = 0;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            found = 1;
            print_header();
            print_student_row(&s);
            print_separator();
            break;
        }
    }

    fclose(fp);
    if (!found)
        printf("\n  [!] Student with ID %d not found.\n", id);
    pause_screen();
}

void update_student(void) {
    printf("\n  ── Update Student ──────────────────────\n\n");

    int id = read_valid_id("  Enter Student ID to update: ");

    FILE *fp = fopen(DATA_FILE, "r+b");
    if (!fp) {
        printf("\n  [!] No records found.\n");
        pause_screen();
        return;
    }

    Student s;
    int found = 0;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            found = 1;
            printf("\n  Current record:\n");
            print_header();
            print_student_row(&s);
            print_separator();

            printf("\n  Enter new details:\n");
            read_valid_name("  New Name  : ", s.name, MAX_NAME);
            s.marks = read_valid_marks("  New Marks : ");

            /* Move back one record and overwrite */
            fseek(fp, -(long)sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, fp);

            printf("\n  [✓] Student record updated!\n");
            break;
        }
    }

    fclose(fp);
    if (!found)
        printf("\n  [!] Student with ID %d not found.\n", id);
    pause_screen();
}

void delete_student(void) {
    printf("\n  ── Delete Student ──────────────────────\n\n");

    int id = read_valid_id("  Enter Student ID to delete: ");

    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        printf("\n  [!] No records found.\n");
        pause_screen();
        return;
    }

    FILE *temp = fopen(TEMP_FILE, "wb");
    if (!temp) {
        printf("\n  [!] Error: Could not create temporary file.\n");
        fclose(fp);
        pause_screen();
        return;
    }

    Student s;
    int found = 0;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            found = 1;
            printf("\n  Deleting record:\n");
            print_header();
            print_student_row(&s);
            print_separator();
        } else {
            fwrite(&s, sizeof(Student), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(DATA_FILE);
        rename(TEMP_FILE, DATA_FILE);
        printf("\n  [✓] Student deleted successfully!\n");
    } else {
        remove(TEMP_FILE);
        printf("\n  [!] Student with ID %d not found.\n", id);
    }
    pause_screen();
}

int cmp_marks_desc(const void *a, const void *b) {
    float diff = ((const Student *)b)->marks - ((const Student *)a)->marks;
    return (diff > 0) ? 1 : (diff < 0) ? -1 : 0;
}

int cmp_marks_asc(const void *a, const void *b) {
    float diff = ((const Student *)a)->marks - ((const Student *)b)->marks;
    return (diff > 0) ? 1 : (diff < 0) ? -1 : 0;
}

int cmp_name_asc(const void *a, const void *b) {
    return strcmp(((const Student *)a)->name, ((const Student *)b)->name);
}

int cmp_name_desc(const void *a, const void *b) {
    return strcmp(((const Student *)b)->name, ((const Student *)a)->name);
}

void sort_students(void) {
    int n = count_records();
    if (n == 0) {
        printf("\n  [!] No records to sort.\n");
        pause_screen();
        return;
    }

    printf("\n  ── Sort Students ───────────────────────\n");
    printf("  1. By Marks (High → Low)\n");
    printf("  2. By Marks (Low → High)\n");
    printf("  3. By Name  (A → Z)\n");
    printf("  4. By Name  (Z → A)\n");
    printf("  Enter choice: ");

    int choice;
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
        clear_input_buffer();
        printf("\n  [!] Invalid sort option.\n");
        pause_screen();
        return;
    }
    clear_input_buffer();

    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        printf("\n  [!] Error reading data file.\n");
        pause_screen();
        return;
    }

    Student *arr = (Student *)malloc(n * sizeof(Student));
    if (!arr) {
        printf("\n  [!] Memory allocation failed.\n");
        fclose(fp);
        pause_screen();
        return;
    }

    fread(arr, sizeof(Student), n, fp);
    fclose(fp);

    switch (choice) {
        case 1: qsort(arr, n, sizeof(Student), cmp_marks_desc); break;
        case 2: qsort(arr, n, sizeof(Student), cmp_marks_asc);  break;
        case 3: qsort(arr, n, sizeof(Student), cmp_name_asc);   break;
        case 4: qsort(arr, n, sizeof(Student), cmp_name_desc);  break;
    }

    fp = fopen(DATA_FILE, "wb");
    if (!fp) {
        printf("\n  [!] Error writing data file.\n");
        free(arr);
        pause_screen();
        return;
    }
    fwrite(arr, sizeof(Student), n, fp);
    fclose(fp);

    print_header();
    for (int i = 0; i < n; i++)
        print_student_row(&arr[i]);
    print_separator();

    const char *labels[] = {
        "Marks (High to Low)", "Marks (Low to High)",
        "Name (A to Z)", "Name (Z to A)"
    };
    printf("  Sorted by: %s\n", labels[choice - 1]);

    free(arr);
    pause_screen();
}

void display_topper(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        printf("\n  [!] No records found.\n");
        pause_screen();
        return;
    }

    Student s, topper;
    int found = 0;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (!found || s.marks > topper.marks) {
            topper = s;
            found = 1;
        }
    }
    fclose(fp);

    if (!found) {
        printf("\n  [!] No records available.\n");
    } else {
        printf("\n  ★ ── Class Topper ──────────────────────\n");
        print_header();
        print_student_row(&topper);
        print_separator();
        printf("  Congratulations to %s! You are the class topper!\n", topper.name);
    }
    pause_screen();
}
