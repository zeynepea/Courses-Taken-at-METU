package hw2;

public class StudyGroup implements Group{

    private Lab lab;
    private String name;
    
    StudyGroup(String name, Lab lab){
        this.lab = lab;
        this.name = name;
    }

    public Lab getLab() {
        return lab;
    }
    
    public void startStudyingWith(){
        lab.enter(this);
    }

    public void stopStudyingWith(){
    	lab.exit();
    }

    @Override
    public String getName() {
    	return name;
    }
    
    @Override
    public Room getIntentRoom() {
    	return this.lab;
    }
    
}
