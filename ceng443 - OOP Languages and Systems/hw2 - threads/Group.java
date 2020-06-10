package hw2;

// Instead of directly creating StudyGroup class,
// I thought that enabling client to create another groups will go nice with OOP.
public interface Group {
	
	String getName();
	
	// Gets the Room that this group aims to acquire
	Room getIntentRoom();
}
