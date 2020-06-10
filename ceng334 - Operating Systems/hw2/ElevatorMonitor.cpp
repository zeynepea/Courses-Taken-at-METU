#include "monitor.h"
#include <vector>
#include "ConfigHandler.h"
#include "ElevatorMonitor.h"
#include <sys/time.h>
#include <algorithm>

using namespace std;


ElevatorMonitor::ElevatorMonitor() : 
	canEnter(this), canExit(this), canRequest(this), getRequest(this) {
	
	this->personPriorityString.push_back("");
	this->personPriorityString.push_back("hp");
	this->personPriorityString.push_back("lp");

	this->elevatorStateString.push_back("Idle");
	this->elevatorStateString.push_back("Moving-up");
	this->elevatorStateString.push_back("Moving-down");
}

void ElevatorMonitor::setElevator(Elevator elevator) {
	this->elevator = elevator;
}


void ElevatorMonitor::handleNewRequest() {

	for (int i = 0; i < this->requestList.size(); i++) {
		if (this->elevator.state == IDLE) {
			this->performElevatorGotFirstReqAction();
			this->canRequest.notifyAll();
		}
		if (this->requestList[i].initialFloor != this->elevator.currentFloor)
			this->pushUniqueElement(this->requestList[i].initialFloor);
		if (!this->requestList[i].isProcessed) {
			this->requestList[i].isProcessed = true;
			this->printRequestMade(&(this->requestList[i]));
			this->printElevator();
		}
	}
}

void ElevatorMonitor::checkPossibleEntrance() {
	int size = this->requestList.size();
	
	for (int i = size - 1; i >= 0; i--) {
		Person request = this->requestList[i];
		if (request.initialFloor == this->elevator.currentFloor 
				&& getIsPersonAbleToRequest(&request)
				&& getIsPersonAbleToEnter(&request)) {
			if (request.priority == Priority::HIGH) {
				takePersonIn(request);
				this->requestList.erase(this->requestList.begin() + i);
				this->canEnter.notifyAll();
			}
			else {
				if (!this->isHighPrioWaitingOnSameFloor(request.initialFloor)) {
					takePersonIn(request);
					this->requestList.erase(this->requestList.begin() + i);
					this->canEnter.notifyAll();
				}
			}
		}
	}
}

bool ElevatorMonitor::isHighPrioWaitingOnSameFloor(int floor) {
	for (Person request : this->requestList) {
		if (request.initialFloor == floor && request.priority == Priority::HIGH && this->getIsPersonAbleToEnter(&request))
			return true;
	}
	return false;
}



void ElevatorMonitor::pushUniqueElement(int element) {
	std::vector<int>::iterator findResult = std::find(this->destinationFloors.begin(), this->destinationFloors.end(), element);
	if (findResult == this->destinationFloors.end()) {
		this->destinationFloors.push_back(element);
	}
}

void ElevatorMonitor::takePersonIn(Person person) {
	this->pushUniqueElement(person.destinationFloor);
	this->peopleInElavator.insert(person.pid);
	this->elevator.currentPeopleCount++;
	this->elevator.currentTotalWeight += person.weight;
	
	this->printEntered(&person);
	this->printElevator();
}

void ElevatorMonitor::waitForRequest(int timeout) {
	struct timespec waitUntil = this->calculateTimeAfterMicrosec(timeout);
	while (1) {
		if (this->getIsTimePassed(waitUntil))
			return;
		int result = this->getRequest.timedWait(waitUntil);
		if (result == 0) {
			this->handleNewRequest();
		}
		this->checkPossibleEntrance();
	}
}

void ElevatorMonitor::personReqNotSentState(Person* person) {
	__synchronized__;
	this->canRequest.wait();
	if (!this->getIsPersonAlreadyRequested(person->pid)) {
		if (this->getIsPersonAbleToRequest(person)) {
			person->state = PersonState::REQUEST_SENT;
			this->requestList.push_back(*person);
			person->lastRequestTime = this->calculateTimeAfterMicrosec(0);
			this->getRequest.notify();
		}
	}
}

void ElevatorMonitor::personReqSentState(Person* person) {
	__synchronized__;
	this->canEnter.wait();
	if (this->couldPersonEntered(person->pid)) {
		person->state = PersonState::ENTERED;
		return;
	}

	// a person can send a request once per transition to idle
	if (this->getIsFirstPassedSecond(this->lastTransitionToIdle, person->lastRequestTime)){ 
		person->state = PersonState::REQUEST_NOT_SENT;
		return;
	}
}



void ElevatorMonitor::personEnteredState(Person* person, bool& isServed) {
	__synchronized__;
	this->canExit.wait();
	if (this->elevator.currentFloor == person->destinationFloor) {
		this->performPersonExitedAction(person);
		isServed = true;
		this->peopleServedCount++;
	}
}

// DONE
void ElevatorMonitor::elevatorIdleState(ElevatorState& state, bool& areAllPeopleServed) {
	__synchronized__;
	if (this->peopleServedCount == this->totalPeople) {
		areAllPeopleServed = true;
		return;
	}
	this->requestList.clear();
	this->lastTransitionToIdle = this->calculateTimeAfterMicrosec(0);
	this->canExit.notifyAll();
	this->canEnter.notifyAll();
	this->canRequest.notifyAll();
	this->waitForRequest(this->elevator.idleTime);
	state = this->elevator.state;
}

// DONE
void ElevatorMonitor::elevatorMovingUpState(ElevatorState& state) {
	__synchronized__;
	this->waitForRequest(this->elevator.travelTime);
	this->elevator.currentFloor++;
	this->performElevatorNewFloorAction();
	state = this->elevator.state;
}

// DONE
void ElevatorMonitor::elevatorMovingDownState(ElevatorState& state) {
	__synchronized__;
	this->waitForRequest(this->elevator.travelTime);
	this->elevator.currentFloor--;
	this->performElevatorNewFloorAction();
	state = this->elevator.state;
}

bool ElevatorMonitor::couldPersonEntered(int pid) {
	return (this->peopleInElavator.find(pid)) != (this->peopleInElavator.end());
}

bool ElevatorMonitor::getIsPersonAbleToEnter(Person* person) {
	if (person->initialFloor != this->elevator.currentFloor)
		return false;
	if (this->elevator.personCapacity == this->elevator.currentPeopleCount)
		return false;
	if (this->elevator.currentTotalWeight + person->weight > this->elevator.weightCapacity)
		return false;
	return true;
}

void ElevatorMonitor::performElevatorGotFirstReqAction() {
	Person firstRequest = this->requestList[0];
	if (firstRequest.initialFloor == this->elevator.currentFloor) {
		if (firstRequest.destinationFloor > this->elevator.currentFloor) {
			this->elevator.state = ElevatorState::MOVING_UP;
		}
		else {
			this->elevator.state = ElevatorState::MOVING_DOWN;
		}
	}
	else if (firstRequest.initialFloor > this->elevator.currentFloor) {
		this->elevator.state = ElevatorState::MOVING_UP;
	}
	else {
		this->elevator.state = ElevatorState::MOVING_DOWN;
	}
}

void ElevatorMonitor::performElevatorNewFloorAction() {
	if (this->destinationFloors[0] == this->elevator.currentFloor) {
		this->destinationFloors.erase(this->destinationFloors.begin());
		if (this->destinationFloors.empty()) {
			this->elevator.state = ElevatorState::IDLE;
			this->requestList.clear();
			this->lastTransitionToIdle = this->calculateTimeAfterMicrosec(0);
		}
		this->printElevator();
		this->canExit.notifyAll();
		this->canEnter.notifyAll();
		this->waitForRequest(this->elevator.inOutTime);
	}
	else {
		this->printElevator();
	}
}



void ElevatorMonitor::performPersonExitedAction(Person* person) {
	this->elevator.currentPeopleCount--;
	this->elevator.currentTotalWeight -= person->weight;
	peopleInElavator.erase(person->pid);
	this->printExited(person);
	this->printElevator();
}

bool ElevatorMonitor::getIsPersonAbleToRequest(Person* person) {
	if (this->elevator.state == ElevatorState::IDLE) {
		if (this->requestList.size() == 0)
			return true;
		else
			return false;
	}

	//person moving up
	if (person->destinationFloor - person->initialFloor > 0) {
		if (!(this->elevator.state == ElevatorState::MOVING_UP)) 
			return false;
		if (person->initialFloor < this->elevator.currentFloor)
			return false;
	}
	//person moving down
	else {
		if (!(this->elevator.state == ElevatorState::MOVING_DOWN))
			return false;
		if (person->initialFloor > this->elevator.currentFloor)
			return false;
	}
	return true;
}


bool ElevatorMonitor::getIsPersonAlreadyRequested(int pid) {
	for (Person person: this->requestList) {
		if (person.pid == pid) {
			return true;
		}
	}
	return false;
}

void ElevatorMonitor::setTotalPeople(int totalPeople) {
	this->totalPeople = totalPeople;
}

struct timespec ElevatorMonitor::calculateTimeAfterMicrosec(long microseconds) {
	struct timeval tv;
	struct timespec ts;
	gettimeofday(&tv, NULL);

	int secondDiff = (tv.tv_usec + microseconds) / 1000000;
	long remainingMicroSeconds = (tv.tv_usec + microseconds) % 1000000;

	ts.tv_sec = tv.tv_sec + secondDiff;
	ts.tv_nsec = remainingMicroSeconds * 1000;

	return ts;
}

bool ElevatorMonitor::getIsTimePassed(struct timespec ts1) {
	
	long nanoTs1 = (ts1.tv_sec * SEC_TO_NANO) + ts1.tv_nsec;

	struct timeval tv;
	gettimeofday(&tv, NULL);

	long nanoTs2 = (tv.tv_sec * SEC_TO_NANO) + (tv.tv_usec * 1000);
	return nanoTs1 < nanoTs2;
}

bool ElevatorMonitor::getIsFirstPassedSecond(struct timespec ts1, struct timespec ts2) {
	long nanoTs1 = (ts1.tv_sec * SEC_TO_NANO) + ts1.tv_nsec;
	long nanoTs2 = (ts2.tv_sec * SEC_TO_NANO) + ts2.tv_nsec;
	return nanoTs1 > nanoTs2;
}

void ElevatorMonitor::sortDestinationVector() {
	if (this->elevator.state == ElevatorState::MOVING_DOWN) 
		std::sort(this->destinationFloors.begin(), this->destinationFloors.end(), std::greater<int>());
	else
		std::sort(this->destinationFloors.begin(), this->destinationFloors.end(), std::less<int>());
}

void ElevatorMonitor::printRequestMade(Person* person) {
	std::cout << "Person (" << person->pid << ", "
		<< this->personPriorityString[(int)person->priority] << ", "
		<< person->initialFloor << " -> "
		<< person->destinationFloor << ", "
		<< person->weight << ") made a request\n";
}

void ElevatorMonitor::printEntered(Person* person) {
	std::cout << "Person (" << person->pid << ", "
		<< this->personPriorityString[(int)person->priority] << ", "
		<< person->initialFloor << " -> "
		<< person->destinationFloor << ", "
		<< person->weight << ") entered the elevator\n";
}

void ElevatorMonitor::printExited(Person* person) {
	std::cout << "Person (" << person->pid << ", "
		<< this->personPriorityString[(int)person->priority] << ", "
		<< person->initialFloor << " -> "
		<< person->destinationFloor << ", "
		<< person->weight << ") has left the elevator\n";
}

void ElevatorMonitor::printElevator(){
	std::cout << "Elevator ("
		<< this->elevatorStateString[(int)this->elevator.state] << ", "
		<< this->elevator.currentTotalWeight << ", "
		<< this->elevator.currentPeopleCount << ", "
		<< this->elevator.currentFloor << " ->";

	this->sortDestinationVector();
	for (int i = 0; i < this->destinationFloors.size(); i++) {
		if (i == 0) {
			std::cout << " "<<destinationFloors[0];
		}
		else {
			std::cout << "," << destinationFloors[i];
		}
	}
	std::cout << ")\n";
}

