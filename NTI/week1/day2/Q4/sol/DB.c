#include <stdio.h>
#include "DB.h"
#include <string.h>

// char *strcpy(char *dest, const char *src);

extern Student *DB[2];
void ModifyName(unsigned int ID, char *Newname)
{
  int i;
  for (i = 0; i < 2; i++)
  {
    if (ID == DB[i]->sID)
    {
      strncpy(DB[i]->name, Newname, nameLen);
    }
  }
}
