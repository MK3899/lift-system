#include <iostream>
#include<vector>
#include<iterator>
#include<string>
#include<cmath>
#include<cstdlib>

using namespace std; // will have to use pointer objects and also use destructor where we have used pointer object

class Elevator;
class Persons;
class Floor;
class Building;

class Elevator
{
protected:
    short int currentFloor;
    short int peopleIn;
    short int maxCapacity;
    short int direction; // '1' for up and '-1' for down or else 0 if the lift is idle
    Floor *curr; //position of the lift
    vector<int> in;
public:
    elevator(short int mcap)
    {
        maxCapacity = mcap;
        direction = 0; //lift is initially idle
    }
};

class Floor
{
protected:
    static short int floorNumber;
    vector< pair<int,int> > *queUp; // first value will be priority and second will be destination
    vector< pair<int,int> > *queDown;
    Floor *aboveFloor;
    Floor *belowFloor;
public:
    Floor()
    {
        floorNumber = floorCount;
        vector< pair<int,int> > up;
        vector< pair<int,int> > down; //check
        queUp = up;
        queDown = down;
        aboveFloor = NULL;
        belowFloor = NULL;
        floorCounter();
    }
    void static floorCounter()
    {
        ++floorCount;
    }
};
short int Floor::floorNumber;

class Person
{
protected:
    short int priority;
    short int source;
    short int destination;
public:
    static short int counterp;
    Person()
    {
        do
        {
        this->source = generateRequestSource();
        this->destination = generateRequestDestination();
        }while(this->source != this->destination);
        priority = counterp;
        priorityCounter();
    }
    static void priorityCounter()
    {
        ++counterp;
    }
    int generateRequestSource();
    int generateRequestDestination();
};
short int Person::counterp;
int Person::generateRequestSource()
{
    int sour = (rand()%(numberOfFloors+1));  //we have to inherit the attribute no of floor
    return sour;
}
int Person::generateRequestDestination()
{
    int dest = (rand()%(numberOfFloors+1)); //we have to inherit the attribute no of floor
    return dest;
}

class Building
{
protected:
    short int numberOfFloors;
    short int numberOfElevators;
    Floor *base;
    Floor *top;
    //Elevator objects reference if there were multiple objects
    Elevator *el;
public:
    Building(short int nf,short int ne)
    {
        numberOfFloors = nf;
        Floors *fl = new floor[numberOfFloors];
        numberOfElevators = ne;
        base = fl;
        // top needs to be defined,base
        el = new Elevator;
    }
    void generatePerson() //calling this function after regular interval
    {
        Person p = new Person();
        enqueueRequest(p);
    }
    void enqueueRequest(Person p);
    void controller();
    void dequeueRequest(Floor *temp,  int d);
    void drop();
    bool checkRequest(int cfloor);
};
void Building::enqueueRequest(Person p)
{
    Floor *temp = base;
    while(temp != NULL)
    {
        if(p->source == temp->floorNumber)
        {
            if(p->destination > p->source)
            {
                temp->queUp->push_back(make_pair(p->priority,p->destination));
            }
            else
            {
                temp->queDown->push_back(make_pair(p->priority,p->destination));
            }
        }
        else
        {
            temp = temp->aboveFloor;
        }
    }
}
void Building::controller()
{
    if(el->direction == 0)//idle condition
    {
        Floor *temp = base;
        int min = INT_MAX;
        int minFloor;
        while(temp != NULL)
        {
            auto itr1 = temp->queUp->begin();
            if(itr1.first < min)
            {
                minFloor = temp->floorNumber;
                min = itr1.first;
            }
            auto itr2 = temp->queDown->begin();
            if(itr2.first < min)
            {
                minFloor = temp->floorNumber;
                min = itr2.first;
            }
            temp = temp->aboveFloor;
        }
        if(el->currentFloor > minFloor)
        {
            el->direction = -1;
        }
        else if(el->currentFloor < minFloor)
        {
            el->direction = 1;
        }
        else
        {
            dequeueRequest() // remove this pair from the queue because it is already there
        }
    }
    else if(el->direction == 1)
    {
        Floor *temp = el->curr;
        while(temp != NULL)
        {
            if(temp->queUp != NULL)
            {
                for(auto itr = temp->queUp->begin(); itr != temp->queUp->end(); itr++)
                {
                    if(el->peopleIn < el->maxCapacity)
                    {
                        el->in->push_back(itr.second);
                        dequeueRequest(temp , el->direction); //dequeue the push element from the floor
                    }
                }
            }
            temp = temp->aboveFloor;
            el->curr = temp; // current floor of the elevator is changed
            drop();
        }
        //if request present down then change the direction or else make it idle
    }
    else if(el->direction == -1)
    {
        Floor *temp = el->curr;
        while(temp != NULL)
        {
            if(temp->queDown != NULL)
            {
                for(auto itr = temp->queDown->begin(); itr != temp->queDown->end(); itr++)
                {
                    if(el->peopleIn < el->maxCapacity)
                    {
                        el->in->push_back(itr.second);
                        dequeueRequest(temp , el->direction); //dequeue the push element from the floor
                    }
                }
            }
            temp = temp->belowFloor;
            el->curr = temp; // current floor of the elevator is changed
            drop();
        }
    }
}
void Building::dequeueRequest(Floor *temp,  int d)
{
    if(d == 1)
    {
        auto itr = temp->queUp;
        temp->queUp.erase(itr);
    }
    else
    {
        auto itr = temp->queDown;
        temp->queDown.erase(itr);
    }
}
void Building::drop()
{
    for(auto itr = el->in->begin() ; itr != el->in->end() ; itr++)
    {
        if(*itr == el->currentFloor)
        {
            el->in->erase(itr);
        }
    }
    bool check = checkRequest(el->currentFloor);
    if((!check) && (el->in->empty()))
    {
        el->direction = 0; //made the elevator idle since no request and the elevator is empty
    }
}
bool Building::checkRequest(int cfloor)
{

}

int main()
{

    return 0;
}
