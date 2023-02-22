#ifndef _DB_H
#define _DB_H

#define nameLen 20
typedef struct dataBaseStudent
{
   unsigned int sID;
   char name[nameLen];
} Student;
void ModifyName(unsigned int indx, char *Newname);
#endif // _DB_H
