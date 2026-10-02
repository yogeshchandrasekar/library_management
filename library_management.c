#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 100
#define MAX_ISSUES 100

struct Book
{
    int id;
    char title[50];
    char author[50];
    int quantity;
};

struct Issue
{
    int issue_id;
    int book_id;
    int user_id;
    char user_name[50];

    char issue_date[20];
    char due_date[20];
    char return_date[20];

    int fine;
    int returned;
};

struct Book books[MAX_BOOKS];
struct Issue issues[MAX_ISSUES];

int book_count = 0;
int issue_count = 0;
int next_issue_id = 1;


/* ---------- DATE FUNCTIONS ---------- */

void get_date(char date[])
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    sprintf(date, "%02d-%02d-%04d",
            tm->tm_mday,
            tm->tm_mon + 1,
            tm->tm_year + 1900);
}

void get_due_date(char date[])
{
    time_t t = time(NULL);
    t += 7 * 24 * 60 * 60;

    struct tm *tm = localtime(&t);

    sprintf(date, "%02d-%02d-%04d",
            tm->tm_mday,
            tm->tm_mon + 1,
            tm->tm_year + 1900);
}

time_t convert_date(char date[])
{
    int d, m, y;
    struct tm tm = {0};

    sscanf(date, "%d-%d-%d", &d, &m, &y);

    tm.tm_mday = d;
    tm.tm_mon = m - 1;
    tm.tm_year = y - 1900;

    return mktime(&tm);
}


/* ---------- BOOK FUNCTIONS ---------- */

void add_book()
{
    printf("Enter Book ID: ");
    scanf("%d", &books[book_count].id);

    printf("Enter Title: ");
    scanf(" %[^\n]", books[book_count].title);

    printf("Enter Author: ");
    scanf(" %[^\n]", books[book_count].author);

    printf("Enter Quantity: ");
    scanf("%d", &books[book_count].quantity);

    book_count++;

    printf("Book added successfully.\n");
}

int find_book(int id)
{
    int i;

    for(i = 0; i < book_count; i++)
    {
        if(books[i].id == id)
            return i;
    }

    return -1;
}

void update_book()
{
    int id, choice, index;

    printf("1. By Book ID\n");
    printf("2. By Book Name\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    index = -1;

    if(choice == 1)
    {
        printf("Enter Book ID: ");
        scanf("%d", &id);

        index = find_book(id);
    }
    else if(choice == 2)
    {
        char name[50];

        printf("Enter Book Name: ");
        scanf(" %[^\n]", name);

        for(int i = 0; i < book_count; i++)
        {
            if(strcmp(books[i].title, name) == 0)
            {
                index = i;
                break;
            }
        }
    }

    if(index == -1)
    {
        printf("Book not found.\n");
        return;
    }

    printf("Enter new title: ");
    scanf(" %[^\n]", books[index].title);

    printf("Enter new author: ");
    scanf(" %[^\n]", books[index].author);

    printf("Enter new quantity: ");
    scanf("%d", &books[index].quantity);

    printf("Book updated.\n");
}

void remove_book()
{
    int id, index;

    printf("Enter Book ID: ");
    scanf("%d", &id);

    index = find_book(id);

    if(index == -1)
    {
        printf("Book not found.\n");
        return;
    }

    for(int i = index; i < book_count - 1; i++)
    {
        books[i] = books[i + 1];
    }

    book_count--;

    printf("Book removed.\n");
}

void search_book()
{
    int choice;

    printf("1. By Book ID\n");
    printf("2. By Book Name\n");
    printf("3. By Author Name\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if(choice == 1)
    {
        int id;

        printf("Enter Book ID: ");
        scanf("%d", &id);

        int index = find_book(id);

        if(index == -1)
        {
            printf("Book not found.\n");
            return;
        }

        printf("\nID: %d\n", books[index].id);
        printf("Title: %s\n", books[index].title);
        printf("Author: %s\n", books[index].author);
        printf("Quantity: %d\n", books[index].quantity);
    }

    else if(choice == 2)
    {
        char name[50];

        printf("Enter Book Name: ");
        scanf(" %[^\n]", name);

        for(int i = 0; i < book_count; i++)
        {
            if(strcmp(books[i].title, name) == 0)
            {
                printf("\nID: %d\n", books[i].id);
                printf("Title: %s\n", books[i].title);
                printf("Author: %s\n", books[i].author);
                printf("Quantity: %d\n", books[i].quantity);
                return;
            }
        }

        printf("Book not found.\n");
    }

    else if(choice == 3)
    {
        char author[50];

        printf("Enter Author Name: ");
        scanf(" %[^\n]", author);

        for(int i = 0; i < book_count; i++)
        {
            if(strcmp(books[i].author, author) == 0)
            {
                printf("\nID: %d\n", books[i].id);
                printf("Title: %s\n", books[i].title);
                printf("Author: %s\n", books[i].author);
                printf("Quantity: %d\n", books[i].quantity);
            }
        }
    }
}

void view_books()
{
    printf("\n----- ALL BOOKS -----\n");

    for(int i = 0; i < book_count; i++)
    {
        printf("ID: %d | Title: %s | Author: %s | Quantity: %d\n",
               books[i].id,
               books[i].title,
               books[i].author,
               books[i].quantity);
    }
}


/* ---------- ISSUE ---------- */

void issue_book()
{
    int book_id, index;

    printf("Enter Book ID: ");
    scanf("%d", &book_id);

    index = find_book(book_id);

    if(index == -1)
    {
        printf("Book not found.\n");
        return;
    }

    if(books[index].quantity <= 0)
    {
        printf("Book not available.\n");
        return;
    }

    issues[issue_count].issue_id = next_issue_id++;

    issues[issue_count].book_id = book_id;

    printf("Enter User ID: ");
    scanf("%d", &issues[issue_count].user_id);

    printf("Enter User Name: ");
    scanf(" %[^\n]", issues[issue_count].user_name);

    get_date(issues[issue_count].issue_date);
    get_due_date(issues[issue_count].due_date);

    issues[issue_count].return_date[0] = '\0';
    issues[issue_count].fine = 0;
    issues[issue_count].returned = 0;

    books[index].quantity--;

    issue_count++;

    printf("Book issued successfully.\n");
}


/* ---------- RETURN ---------- */

void return_book()
{
    int book_id, user_id;
    int index = -1;

    printf("Enter Book ID: ");
    scanf("%d", &book_id);

    printf("Enter User ID: ");
    scanf("%d", &user_id);

    for(int i = 0; i < issue_count; i++)
    {
        if(issues[i].book_id == book_id &&
           issues[i].user_id == user_id &&
           issues[i].returned == 0)
        {
            index = i;
            break;
        }
    }

    if(index == -1)
    {
        printf("Issue record not found.\n");
        return;
    }

    get_date(issues[index].return_date);

    time_t due = convert_date(issues[index].due_date);
    time_t ret = convert_date(issues[index].return_date);

    double difference = difftime(ret, due);

    int late_days = difference / (24 * 60 * 60);

    if(late_days > 0)
        issues[index].fine = late_days * 5;
    else
        issues[index].fine = 0;

    issues[index].returned = 1;

    int book_index = find_book(book_id);

    if(book_index != -1)
        books[book_index].quantity++;

    printf("Book returned successfully.\n");
    printf("Fine: Rs.%d\n", issues[index].fine);
}


/* ---------- ISSUED BOOKS ---------- */

void list_issued()
{
    printf("\n----- ISSUED BOOKS -----\n");

    for(int i = 0; i < issue_count; i++)
    {
        int book_index = find_book(issues[i].book_id);

        printf("\nIssue ID: %d", issues[i].issue_id);
        printf("\nBook ID: %d", issues[i].book_id);

        if(book_index != -1)
            printf("\nBook Title: %s", books[book_index].title);

        printf("\nUser ID: %d", issues[i].user_id);
        printf("\nUser Name: %s", issues[i].user_name);
        printf("\nIssue Date: %s", issues[i].issue_date);
        printf("\nDue Date: %s", issues[i].due_date);
        printf("\nReturn Date: %s",
               issues[i].returned ? issues[i].return_date : "Not Returned");

        printf("\nFine: Rs.%d\n", issues[i].fine);
    }
}


/* ---------- FILE HANDLING ---------- */

void save_data()
{
    FILE *fp;

    fp = fopen("books.dat", "wb");

    if(fp == NULL)
    {
        printf("File error.\n");
        return;
    }

    fwrite(&book_count, sizeof(int), 1, fp);
    fwrite(books, sizeof(struct Book), book_count, fp);

    fclose(fp);


    fp = fopen("issued.dat", "wb");

    if(fp == NULL)
    {
        printf("File error.\n");
        return;
    }

    fwrite(&issue_count, sizeof(int), 1, fp);
    fwrite(issues, sizeof(struct Issue), issue_count, fp);

    fclose(fp);

    printf("Data saved.\n");
}

void load_data()
{
    FILE *fp;

    fp = fopen("books.dat", "rb");

    if(fp != NULL)
    {
        fread(&book_count, sizeof(int), 1, fp);
        fread(books, sizeof(struct Book), book_count, fp);

        fclose(fp);
    }

    fp = fopen("issued.dat", "rb");

    if(fp != NULL)
    {
        fread(&issue_count, sizeof(int), 1, fp);
        fread(issues, sizeof(struct Issue), issue_count, fp);

        fclose(fp);

        next_issue_id = issue_count + 1;
    }
}


/* ---------- MAIN ---------- */

int main()
{
    int choice;

    load_data();

    while(1)
    {
        printf("\n\n");
        printf("===== LIBRARY MANAGEMENT SYSTEM =====\n");
        printf("1. Add New Book\n");
        printf("2. Update Book Details\n");
        printf("3. Remove Book\n");
        printf("4. Search Book\n");
        printf("5. View All Books\n");
        printf("6. Issue Book\n");
        printf("7. Return Book\n");
        printf("8. List Issued Books\n");
        printf("9. Save\n");
        printf("10. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                add_book();
                break;

            case 2:
                update_book();
                break;

            case 3:
                remove_book();
                break;

            case 4:
                search_book();
                break;

            case 5:
                view_books();
                break;

            case 6:
                issue_book();
                break;

            case 7:
                return_book();
                break;

            case 8:
                list_issued();
                break;

            case 9:
                save_data();
                break;

            case 10:
                save_data();
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }
}