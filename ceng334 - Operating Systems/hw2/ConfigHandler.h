#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sys/time.h>

using namespace std;

enum PersonState {
	REQUEST_NOT_SENT,
	REQUEST_SENT,
	ENTERED
};

enum Priority {
	HIGH = 1,
	LOW = 2
};

enum ElevatorState {
	IDLE,
	MOVING_UP,
	MOVING_DOWN
};

typedef struct Person {
	int weight;
	int initialFloor;
	int destinationFloor;
	Priority priority;
	int pid;
	PersonState state;
	struct timespec lastRequestTime { 0, 0 };
	bool isProcessed;
} Person;

typedef struct Elevator {
	int nFloors;
	int weightCapacity;
	int personCapacity;
	int travelTime;
	int idleTime;
	int inOutTime;
	ElevatorState state;
	int currentPeopleCount;
	int currentTotalWeight;
	int currentFloor;
} Elevator;

class ConfigHandler {
public:
	ConfigHandler(string inputFile);
	void parseInput();
	vector<Person> getPeople();
	Elevator getElevator();
	void printParsedInput();
private:
	string inputFile;
	int nPeople;
	vector<Person> people;
	Elevator elevator;
};