package hw2;

import java.util.concurrent.*;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class Lab  implements Room{

    private final String name;
    private final int capacity;
    private Semaphore sem;
    private Group currentGroup;
    private ReentrantLock lock;
    private Condition cond;
    
    public Lab(String name, int capacity){
        this.name = name;
        this.capacity = capacity;
        this.sem= new Semaphore(capacity, true);
        this.lock = new ReentrantLock(true);
        this.currentGroup = null;
        this.cond = lock.newCondition();
    }

    @Override
    public void enter(Group group) {
    	while(true) {
    	    // lock this room
    		lock.lock();
    		// if there are not any group in the lab
            // or the group given is the same with the one inside the lab
            // and sem.tryAcquire() returns true meaning there is enough capacity
                // then change group inside the lab to given group
                // unlock this room
	    	if( (currentGroup == null || group == currentGroup) && sem.tryAcquire() ) {
	    		currentGroup = group;
	    		lock.unlock();
	    		return;
	    	}
	    	else {
	    	    // else wait until this room is available signal
	    	    try {
	    	        cond.await();
                }catch (InterruptedException e){
	    	        e.printStackTrace();
                }finally {
                    // unlock this room
                    lock.unlock();
                }
	    	}
    	}
    }
    
    @Override
    public void exit() {
        // lock this room
    	lock.lock();
    	// release a spot in the lab
    	sem.release();
    	// if the last member of the group exits this room
    	if(sem.availablePermits() == capacity) {
    		currentGroup = null;
    	}
    	// wake up all threads waiting to enter this room
    	cond.signalAll();
        // unlock this room
    	lock.unlock();
    }
    
    @Override
    public String getName() {
        return name;
    }

    @Override
    public int getCapacity() {
        return capacity;
    }
}