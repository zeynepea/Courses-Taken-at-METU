#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Playlist.hpp"
#include "Entry.hpp"

using namespace std;


Playlist::Playlist()
{
    srand(15);
}

int Playlist::getRandomNumber(int i, int n) const
{
    int range=n-i;
    int random = rand() % range + i;
    return random;
}

void Playlist::print()
{
    cout << "[PLAYLIST SIZE=" << entries.getSize() <<"]";
    entries.print();
}
void Playlist::printHistory()
{
    cout<<"[HISTORY]";
    history.print();
}

void Playlist::load(std::string fileName){
	int o=0;
	ifstream fin;
    fin.open(fileName.c_str());
    string line,word,title1,genre1,year1;
    string delimiter=";";
    string C_3PO;
    while(getline(fin,line)){
        C_3PO=line;
        title1=C_3PO.substr(0,C_3PO.find(delimiter));
    	C_3PO.erase(0, C_3PO.find(delimiter) + delimiter.length());
    	genre1=C_3PO.substr(0,C_3PO.find(delimiter));
    	C_3PO.erase(0, C_3PO.find(delimiter) + delimiter.length());
    	year1=C_3PO.substr(0,C_3PO.find(delimiter));
    	C_3PO.erase(0, C_3PO.find(delimiter) + delimiter.length());
    	if(entries.isEmpty()){
    		Entry e(title1,genre1,year1);
    		insertEntry(e);
    	}
    	else{
    		Entry e(title1,genre1,year1);
    		insertEntry(e);
        }
    }
}

void Playlist::insertEntry(const Entry &e) {
    Node<Entry>* x=new Node<Entry>(e);
    HistoryRecord y(INSERT,e);
    Node<HistoryRecord>* z =new Node<HistoryRecord>(y);
    if(entries.isEmpty()){
    	entries.insertNode(NULL,e);
    }
    else{
    	Node<Entry>* p=entries.getTail();
    	entries.insertNode(p,e);
    }
    history.push(y);
}


void Playlist::deleteEntry(const std::string &_title){
	Node<Entry>* p=entries.getHead();
	Node<Entry>* q=entries.getHead();
	if(p->getData().getTitle()==_title){
		Entry e(p->getData().getTitle(), p->getData().getGenre(), p->getData().getYear());
		HistoryRecord h(DELETE,e);
        history.push(h);
        entries.deleteNode(NULL);
		return;
	} 
	p=p->getNext();
	while(p){
		if(p->getData().getTitle()==_title){
			Entry e(p->getData().getTitle(), p->getData().getGenre(), p->getData().getYear());
			HistoryRecord h(DELETE,e);
            history.push(h);
            entries.deleteNode(q);
            break;
		}
		p=p->getNext();
		q=q->getNext();
	}
}

	/*Moves the entry with given title to the left.*/
void Playlist::moveLeft(const std::string &title){
    Node<Entry>* p=entries.getHead();
	Node<Entry>* q=entries.getHead();
	if(p->getData().getTitle()==title) return;
	p=p->getNext();
	if(p->getData().getTitle()==title){
		Entry e1(p->getData().getTitle(), p->getData().getGenre(), p->getData().getYear());
		Node<Entry>* x=new Node<Entry>(e1);
		Entry e2(q->getData().getTitle(), q->getData().getGenre(), q->getData().getYear());
		Node<Entry>* y=new Node<Entry>(e2);
		entries.setHead(x);
		x->setNext(y);
		x=x->getNext();
		x->setNext(p->getNext());
		return;
	}
	while(p){
		if(p->getNext()->getData().getTitle()==title){
			Entry e1(p->getNext()->getData().getTitle(), p->getNext()->getData().getGenre(), p->getNext()->getData().getYear());
			entries.insertNode(q,e1);
			entries.deleteNode(p);
			break;
		}
		q=q->getNext();
		p=p->getNext();
	}
}
	/*Moves the entry with given title to the right.*/
void Playlist::moveRight(const std::string &title){

}

void Playlist::reverse(){
	MyStack<Entry> rvr;
	Node<Entry>* p=entries.getHead();
	while(p){
		rvr.push(p->getData());
		p=p->getNext();
	}
	entries.clear();
	Node<Entry>* q=entries.getHead();
	entries.insertNode(q,rvr.Top()->getData());
	q=entries.getHead();
	rvr.pop();
	while(!rvr.isEmpty()){
		entries.insertNode(q,rvr.Top()->getData());
		rvr.pop();
		q=q->getNext();
	}
	HistoryRecord h(REVERSE);
	history.push(h);
}


void Playlist::sort(){
	Node<Entry>* x=entries.getHead();
	while(x){
		Node<Entry>* min = x;
        Node<Entry>* y=x->getNext();
		while(y){
			if(y->getData().getTitle() < min->getData().getTitle()){
                 min=y;
			}
			y=y->getNext();
		}
		if(x->getData()==min->getData()){
            x=x->getNext();
		}
		else{
            Node<Entry>* minprev=entries.findPrev(min->getData());
            Node<Entry>* xprev=entries.findPrev(x->getData());
            Node<Entry>* minnext=min->getNext();
            if(!xprev){            	
            	minprev->setNext(x);
                entries.setHead(min);
                min->setNext(x->getNext());
                x->setNext(minnext);
                x=min->getNext();
            }
            else{    
                minprev->setNext(x);
                xprev->setNext(min);
                min->setNext(x->getNext());
                x->setNext(minnext);
                x=min->getNext();  
            }
		}
	}
}


void Playlist::merge(const Playlist & pl){
	Node<Entry>* second=pl.entries.getHead();
	Node<Entry>* first=entries.getHead();
	while(second){
		if(first->getData().getTitle()>second->getData().getTitle()){
		    Node<Entry>* x=entries.findPrev(first->getData());
			entries.insertNode(x,second->getData());
			second=second->getNext();
		}
		else first=first->getNext();
	}
}


void Playlist::shuffle(){
    Node<Entry>* legolas=entries.getHead();
    Node<Entry>* WillTurner=new Node<Entry>();
    Node<Entry>* xD=new Node<Entry>();
    int j,x;
    for(int i=0;i<entries.getSize()-1;i++){
        x=i;
        j=getRandomNumber(i, entries.getSize());
        xD=legolas;
        WillTurner=legolas;
        while(WillTurner){
            if(x==j){
                if(j==i) break;
                Node<Entry>* willprev=entries.findPrev(WillTurner->getData());
                Node<Entry>* legolasprev=entries.findPrev(legolas->getData());
                Node<Entry>* harry=new Node<Entry>(WillTurner->getData());
                Node<Entry>* ginny=new Node<Entry>(legolas->getData());                
                if(!legolasprev){
                    harry->setNext(legolas->getNext());
                    ginny->setNext(WillTurner->getNext());
                    entries.setHead(harry);
                    willprev->setNext(ginny);
                    WillTurner=harry->getNext();
                }
                else{
                    if(legolas->getNext()->getData().getTitle()==WillTurner->getData().getTitle()){
                        ginny->setNext(WillTurner->getNext());
                        harry->setNext(ginny);
                        legolasprev->setNext(harry);
                        xD=harry;
                    }
                    else{
                        ginny->setNext( WillTurner->getNext());
                        harry->setNext(legolas->getNext());
                        willprev->setNext(ginny);
                        legolasprev->setNext(harry);
                    }        
                }
                break;
            }
            x++;
            WillTurner=WillTurner->getNext();
        }
        legolas=xD->getNext();
      }
}
  
void Playlist::undo(){
	if(entries.isEmpty()) return;
	Node<HistoryRecord>* y=history.Top();
	Node<Entry>* x=entries.getHead();
	
	if(y->getData().getOperation()==REVERSE){
		reverse();
		history.pop();
	}
	else if(y->getData().getOperation()==DELETE){
	    insertEntry(y->getData().getEntry());
	    history.pop();
	}
    else if(y->getData().getOperation()==INSERT){
	   	deleteEntry(y->getData().getEntry().getTitle());
	    history.pop();
    }

}

