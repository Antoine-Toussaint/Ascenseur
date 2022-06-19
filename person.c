#include "person.h"
#include<stdlib.h>

Person* createPerson(int src, int dest)
{
    Person *p = malloc(sizeof(Person));
     p->src = src;
     p->dest = dest;

     return p;
}

PersonList *insert(Person *p, PersonList *list)
{
    PersonList * pl = malloc(sizeof(PersonList));
    pl->person = p;
    pl->next = list;

    return pl;
}