#include <stdio.h>
#include "DB.h"

Student s1 = {1, "Ahmed Ali"};
Student s2 = {2, "Sara Ibrahim"};
Student *DB[] = {&s1, &s2};

void main()
{
    unsigned int ID;
    char Newname[nameLen];
    printf("PLease Enter ID of student\n");
    scanf("%d", &ID);
    getchar();
    printf("Please Enter New Name\n");
    gets(Newname);

    ModifyName(ID, Newname);
    printf("After Modification\n");
    for (int i = 0; i < 2; i++)
    {
        printf("%d\t%s\n", DB[i]->sID, DB[i]->name);
    }
}
