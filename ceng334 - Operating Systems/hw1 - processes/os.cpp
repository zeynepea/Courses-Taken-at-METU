#include <iostream>
#include <string.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include "unistd.h"
#include "poll.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include<sys/socket.h>
#include "logging.h"
#include <vector>
#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)
using namespace std;

int startingBid = 0, minimumIncrement = 0, numberOfBidders = 0;
static const int parentsocket = 0, childsocket = 1;
int activeBidders = 0;
vector<string> bidderLines;
int smallestDelay = 999999999; 
struct client_message clientBuf;
struct server_message serverBuf;

struct output_info outputBuffer;
struct input_info inputBuffer;
int currentHighestBid;
int currentWinnerID;

void splitByDelimiterGetInt(string & line, int & esitlenen){

    string delimiter = " ";
    esitlenen = stoi(line.substr(0, line.find(delimiter)));
    line.erase(0, line.find(delimiter) + delimiter.length());
}

char * splitByDelimiterGetCharPointer(string & line){

    string delimiter = " ";
    string esitlenenString = line.substr(0, line.find(delimiter));
    char * esitlenen = new char[esitlenenString.length() + 1];
    strcpy(esitlenen, esitlenenString.c_str());
    line.erase(0, line.find(delimiter) + delimiter.length());
    return esitlenen;
}

void readBidderLines(){

    string line;
    while(getline(cin, line)){
        bidderLines.push_back(line);
    }
}
void readFirstInputLine(){

    string firstLine;
    getline(cin, firstLine);
    splitByDelimiterGetInt(firstLine, startingBid);
    splitByDelimiterGetInt(firstLine, minimumIncrement);
    splitByDelimiterGetInt(firstLine, numberOfBidders);
    currentHighestBid = startingBid;
}

void executeBidder(int child, int parent, int id){
    
    string bidderLine = bidderLines[id];
    char *fileName = nullptr;
    int numberOfArgs;
    fileName = splitByDelimiterGetCharPointer(bidderLine);
    splitByDelimiterGetInt(bidderLine, numberOfArgs);
    const char **args = new const char *[numberOfArgs+2];
    args[0] = fileName;
    int i;
    for( i = 1; i < numberOfArgs+1; i++){
        args[i] = splitByDelimiterGetCharPointer(bidderLine);
    }
    args[i] = NULL ;
    execv(args[0], (char**)args);
    exit(0);
}

void clientConnect(int uniqueID, int pid){

    serverBuf.message_id = CLIENT_CONNECT;
    outputBuffer.type = CLIENT_CONNECT;
    if(smallestDelay>clientBuf.params.delay){
        smallestDelay = clientBuf.params.delay;
    }
    serverBuf.params.start_info.client_id = uniqueID;
    serverBuf.params.start_info.current_bid = currentHighestBid;
    serverBuf.params.start_info.minimum_increment = minimumIncrement;
    serverBuf.params.start_info.starting_bid = startingBid;
    outputBuffer.info = serverBuf.params;
    outputBuffer.pid = pid;

}

void clientBid(int uniqueID, int pid){

    serverBuf.message_id = SERVER_BID_RESULT;
    outputBuffer.type = SERVER_BID_RESULT;
    int givenBid = clientBuf.params.bid;
    if(givenBid < startingBid){
        serverBuf.params.result_info.result = BID_LOWER_THAN_STARTING_BID;
    }
    else if(givenBid < currentHighestBid){
        serverBuf.params.result_info.result = BID_LOWER_THAN_CURRENT;
    }
    else if((givenBid - currentHighestBid) < minimumIncrement){
        serverBuf.params.result_info.result = BID_INCREMENT_LOWER_THAN_MINIMUM;
    }
    else{
        serverBuf.params.result_info.result = BID_ACCEPTED;
        currentHighestBid = givenBid;
        currentWinnerID = uniqueID;
    }
    serverBuf.params.result_info.current_bid = currentHighestBid;
    outputBuffer.info = serverBuf.params;
    outputBuffer.pid = pid;
}

void clientFinnish(int uniqueID, int finishingStatus[]){

    finishingStatus[uniqueID] = clientBuf.params.status;
    activeBidders--;  
}

void setRequest(int uniqueID, int pid, int finishingStatus[]){

    switch (clientBuf.message_id){
        case 1: //CLIENT_CONNECT
            clientConnect(uniqueID,pid);
            break;
        case 2: //CLIENT_BID
            clientBid(uniqueID, pid);
            break;
        case 3: //CLIENT_FINISHED
            clientFinnish(uniqueID, finishingStatus);
            break; 
    }
}

void readRequestIfReady(struct pollfd * fd, int uniqueID, int pid, int finishingStatus[]) {
    
    if (fd->revents & POLLIN) {
        fd->revents &= (~POLLIN);
        int n = read(fd->fd, &clientBuf, sizeof(clientBuf));
        inputBuffer.info = clientBuf.params;
        inputBuffer.type = clientBuf.message_id;
        inputBuffer.pid = pid;
        print_input(&inputBuffer, uniqueID);
        setRequest(uniqueID, pid, finishingStatus);
        write(fd->fd, &serverBuf, sizeof(serverBuf));
        print_output(&outputBuffer, uniqueID);
    }
}



void createPoll(int fd[][2], int pids[], int finishingStatus[]){

    struct pollfd fds[numberOfBidders];
    for(int i = 0; i<numberOfBidders; i++){
        fds[i].fd = fd[i][parentsocket];
        fds[i].events = POLLIN;
    }
    activeBidders = numberOfBidders;
    while (activeBidders>0) {
        int pollResult = poll(fds, numberOfBidders, smallestDelay);
        if (pollResult > 0) {
            for (int i = 0; i < numberOfBidders; i++) {
                readRequestIfReady(&fds[i], i, pids[i], finishingStatus);
            }
        }
    }
}

void broadcastWinner(int fd[][2], int winnerID, int winningBid, int pids[]) {
    
    serverBuf.message_id = SERVER_AUCTION_FINISHED;
    serverBuf.params.winner_info.winner_id = winnerID;
    serverBuf.params.winner_info.winning_bid = winningBid;
    outputBuffer.info = serverBuf.params;
    outputBuffer.type = serverBuf.message_id;
    for (int i = 0; i < numberOfBidders; i++) {
        outputBuffer.pid = pids[i];
        print_output(&outputBuffer, i);
        write(fd[i][parentsocket], &serverBuf, sizeof(serverBuf));
    }
}

void reapChildren(int finishingStatus[]) {

    int status;
    for (int i = 0; i < numberOfBidders; i++) {
        wait(&status);
        print_client_finished(i, status, (int) finishingStatus[i] == status);
    }
}

void pipeAndFork(int fd[][2], int pids[]){
    
    for(int i=0;i<numberOfBidders;i++){
        PIPE(fd[i]);
        pid_t pid = fork();
        if(pid == 0){ // child aka bidder
            close(fd[i][parentsocket]); // Close the parent file descriptor 
            dup2(fd[i][childsocket], 0);
            dup2(fd[i][childsocket], 1);
            executeBidder(fd[i][childsocket], fd[i][parentsocket], i);
        }
        else{ // parent aka server
            pids[i] = (int)pid;
            close(fd[i][childsocket]); // Close the child file descriptor
        }       
    }
}

int main(int argc, char *argv[]){

    readFirstInputLine();
    int fd[numberOfBidders][2];
    int pids[numberOfBidders];
    int finishingStatus[numberOfBidders];
    readBidderLines();
    pipeAndFork(fd, pids);
    createPoll(fd, pids, finishingStatus);
    print_server_finished(currentWinnerID, currentHighestBid);
    broadcastWinner(fd, currentWinnerID, currentHighestBid, pids);
    finishingStatus[currentWinnerID] = clientBuf.params.status;
    reapChildren(finishingStatus);
    return 0;
}