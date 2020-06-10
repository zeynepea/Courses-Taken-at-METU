#include "ConfigHandler.h"
#include <fstream>

ConfigHandler::ConfigHandler(string inputFile) {
	this->inputFile = inputFile;
	this->parseInput();
	this->elevator.currentFloor = 0;
	this->elevator.currentPeopleCount = 0;
	this->elevator.currentTotalWeight = 0;
	this->elevator.state = ElevatorState::IDLE;
}

void ConfigHandler::parseInput() {
	fstream file(this->inputFile, std::ios_base::in);
	file >> this->elevator.nFloors;
	file >> this->nPeople;
	file >> this->elevator.weightCapacity;
	file >> this->elevator.personCapacity;
	file >> this->elevator.travelTime;
	file >> this->elevator.idleTime;
	file >> this->elevator.inOutTime;

	int peopleIdx = 0;
	while (peopleIdx < this->nPeople) {
		Person newPerson;
		file >> newPerson.weight;
		file >> newPerson.initialFloor;
		file >> newPerson.destinationFloor;
		int priority;
		file >> priority;
		newPerson.priority = (Priority)priority;
		newPerson.pid = peopleIdx;
		newPerson.state = PersonState::REQUEST_NOT_SENT;
		newPerson.isProcessed = false;
		this->people.push_back(newPerson);
		peopleIdx++;
	}
}

vector<Person> ConfigHandler::getPeople() {
	return this->people;
}

Elevator ConfigHandler::getElevator() {
	return this->elevator;
}

void ConfigHandler::printParsedInput() {
	cout << elevator.nFloors << " "
		<< people.size() << " "
		<< elevator.weightCapacity << " "
		<< elevator.personCapacity << " "
		<< elevator.travelTime << " "
		<< elevator.idleTime << " "
		<< elevator.inOutTime << "\n";
	for (int i = 0; i < people.size(); i++) {
		cout << people[i].weight << " "
			<< people[i].initialFloor << " "
			<< people[i].destinationFloor << " "
			<< (int)people[i].priority << "\n";
	}
}