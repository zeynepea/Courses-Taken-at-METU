#include "monitor.h"
#include "ConfigHandler.h"
#include "ElevatorMonitor.h"
#include <vector>
#include <pthread.h>
#include <stdio.h>

using namespace std;

ElevatorMonitor elevatorMonitor;

void* elevatorRoutine(void* arg) {
	ElevatorState state = ElevatorState::IDLE;
	bool areAllPeopleServed = false;
	while (1) {
		switch (state)
		{
			case IDLE:
				elevatorMonitor.elevatorIdleState(state, areAllPeopleServed);
				if (areAllPeopleServed) {
					return NULL;
				}
				break;
			case MOVING_UP:
				elevatorMonitor.elevatorMovingUpState(state);
				break;
			case MOVING_DOWN:
				elevatorMonitor.elevatorMovingDownState(state);
				break;
			default:
				break;
		}
	}
	return NULL;
}

void* personRoutine(void* arg) {
	Person* person = (Person*)arg;
	bool isServed = false;
	while (1) {
		switch (person->state)
		{
			case(PersonState::REQUEST_NOT_SENT): {
				elevatorMonitor.personReqNotSentState(person);
				break;
			}
			case(PersonState::REQUEST_SENT): {
				elevatorMonitor.personReqSentState(person);
				break;
			}
			case(PersonState::ENTERED): {
				elevatorMonitor.personEnteredState(person, isServed);
				if(isServed)
					return NULL;
				break;
			}
			default:
				break;
		}
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	ConfigHandler configHandler(argv[1]);
	Elevator elevator = configHandler.getElevator();
	vector<Person> people = configHandler.getPeople();

	elevatorMonitor.setElevator(elevator);
	elevatorMonitor.setTotalPeople(people.size());

	pthread_t* personThreads = new pthread_t[people.size()];
	pthread_t elevatorThread;

	int result = pthread_create(&elevatorThread, NULL, elevatorRoutine, (void*)NULL);

	for (int i = 0; i < people.size(); i++) {
		pthread_create(personThreads + i, NULL, personRoutine, (void*)&people[i]);
	}

	for (int i = 0; i < people.size(); i++) {
		pthread_join(personThreads[i], NULL);
	}
	pthread_join(elevatorThread, NULL);

	delete[] personThreads;
	return 0;
}
