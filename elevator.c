#include "elevator.h"
#include "person.h"
#include <stdlib.h>

Elevator *create_elevator(int capacity, int currentFloor, PersonList *persons)
{
    Elevator *elevator = malloc(sizeof(Elevator));
    elevator->capacity = capacity;
    elevator->currentFloor = currentFloor;
    elevator->persons = persons;

    return elevator;
}

Building *create_building(int nbFloor, Elevator *elevator, PersonList **waitingLists)
{
    Building *building = malloc(sizeof(Building));
    building->nbFloor = nbFloor;
    building->elevator = elevator;
    building->waitingLists = waitingLists;

    return building;
}

PersonList *exitElevator(Elevator *e)
{
    PersonList *personsLeaving = NULL;
    PersonList *personsRemaining = NULL;
    PersonList *explorator = e->persons;

    while (explorator)
    {
        if (explorator->person->dest == e->currentFloor)
        {
            insert(explorator->person, personsLeaving);
        }
        else{
            insert(explorator->person, personsRemaining);
        }

        explorator = explorator->next;
    }

    e->persons = personsRemaining;

    return personsLeaving;
}

PersonList* enterElevator(Elevator *e, PersonList *waitingList)
{
    //calculating first how many persons are currently in the elevator
    int occupation = 0;
    PersonList *occupants = e->persons;
    while(occupants)
    {
        occupation++;
        occupants = occupants->next;
    }

    //then making people on the waiting list enter the escalator
    while(occupation < e->capacity && waitingList)
    {
        insert(waitingList->person, e->persons);
        waitingList = waitingList->next;
    }

    return waitingList;
}

void stepElevator(Building *b)
{
    int currentFloor = b->elevator->currentFloor;
    
    if(currentFloor == b->elevator->targetFloor)
    {
        exitElevator(b->elevator);
        //refreshing waiting list
        *(b->waitingLists + currentFloor) = enterElevator(b->elevator, *(b->waitingLists) + currentFloor);
    }
    else if(currentFloor > b->elevator->targetFloor){
        b->elevator->targetFloor--;
    }else{
        b->elevator->targetFloor++;
    }
}