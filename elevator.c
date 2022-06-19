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
    // PersonList *personsLeaving = NULL;
    // PersonList *personsRemaining = NULL;
    // PersonList *explorator = e->persons;

    // while (explorator)
    // {
    //     if (explorator->person->dest == e->currentFloor)
    //     {
    //         personsLeaving = insert(explorator->person, personsLeaving);
    //     }
    //     else{
    //         personsRemaining = insert(explorator->person, personsRemaining);
    //     }

    //     explorator = explorator->next;
    // }

    // e->persons = personsRemaining;

    // return personsLeaving;

    PersonList *personsLeaving = NULL;
    //this permits to not re-create the list of persons remainingat each call
    //instead, it 'cuts' the lists of persons in the elevator,
    //leaving only the ones who are remaining
    //see the else in the succeding else of the while
    PersonList *lastSeenPersonRemaining = NULL;
    PersonList *firstPersonRemaining = NULL;

    PersonList *explorator = e->persons;

    while (explorator)
    {
        if (explorator->person->dest == e->currentFloor)
        {
            personsLeaving = insert(explorator->person, personsLeaving);

            explorator = explorator->next;
        }
        else{
            if(lastSeenPersonRemaining)
            {
                lastSeenPersonRemaining->next = explorator;
                lastSeenPersonRemaining = explorator;
            }else{
                lastSeenPersonRemaining = explorator;
                firstPersonRemaining = explorator;
            }

            //cycling through persons in the elevator
            explorator = explorator->next;
            //cutting link between persons remaining and not yet treated persons in the elevator
            lastSeenPersonRemaining->next = NULL;
        }
    }

    e->persons = firstPersonRemaining;

    return personsLeaving;
}

//FIXME segmentation fault. can be in exitElevator too
PersonList* enterElevator(Elevator *e, PersonList *waitingList)
{
    // //calculating first how many persons are currently in the elevator
    int occupation = 0;
    PersonList *occupant = e->persons;
    while(occupant)
    {
        occupation++;
        occupant = occupant->next;
    }

    //then making people on the waiting list enter the escalator
    while(occupation < e->capacity && waitingList)
    {
        e->persons = insert(waitingList->person, e->persons);
        waitingList = waitingList->next;
        occupation++;
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
        *(b->waitingLists + currentFloor) = enterElevator(b->elevator, *(b->waitingLists + currentFloor));
    }
    else if(currentFloor > b->elevator->targetFloor){
        b->elevator->currentFloor--;
    }else{
        b->elevator->currentFloor++;
    }
}