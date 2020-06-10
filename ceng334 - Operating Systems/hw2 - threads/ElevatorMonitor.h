#pragma once

#include "monitor.h"
#include <unordered_set>

const long SEC_TO_NANO = 1000000000;

class ElevatorMonitor : public Monitor {

public:
	ElevatorMonitor();
	void setElevator(Elevator elevator);
	void setTotalPeople(int totalPeople);

	// Below 6 functions are synchronized
	void personReqNotSentState(Person* person);
	void personReqSentState(Person* person);
	void personEnteredState(Person* person, bool& isServed);
	void elevatorIdleState(ElevatorState& state, bool& areAllPeopleServed);
	void elevatorMovingUpState(ElevatorState& state);
	void elevatorMovingDownState(ElevatorState& state);

private:
	Condition canEnter;
	Condition canExit;
	Condition canRequest;
	Condition getRequest;
	vector<int> destinationFloors;
	unordered_set<int> peopleInElavator;
	vector<Person> requestList;
	vector<Person> highPriorityList;
	vector<Person> lowPriorityList;
	Elevator elevator;
	vector<string> personPriorityString;
	vector<string> elevatorStateString;
	struct timespec lastTransitionToIdle;
	int totalPeople;
	int peopleServedCount = 0;

	bool couldPersonEntered(int pid);
	bool getIsPersonAlreadyRequested(int pid);
	bool getIsPersonAbleToRequest(Person* person);
	bool getIsPersonAbleToEnter(Person* person);
	void performPersonExitedAction(Person* person);
	void performElevatorGotFirstReqAction();
	void performElevatorNewFloorAction();
	
	void waitForRequest(int timeout);
	void handleNewRequest();
	void checkPossibleEntrance();
	void takePersonIn(Person person);
	bool isHighPrioWaitingOnSameFloor(int floor);

	// Utility functions
	void pushUniqueElement(int element);
	struct timespec calculateTimeAfterMicrosec(long microseconds);
	bool getIsTimePassed(struct timespec ts1);
	bool getIsFirstPassedSecond(struct timespec ts1, struct timespec ts2);
	void sortDestinationVector();
	void printRequestMade(Person* person);
	void printEntered(Person* person);
	void printExited(Person* person);
	void printElevator();
};

