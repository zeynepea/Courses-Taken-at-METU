package hw2;

// Instead of having a single concrete class of Lab,
// I thought that letting users create new rooms like Classroom, WC etc wou
public interface Room {
	String getName();
	int getCapacity();
	void enter(Group group);
	void exit();
}
