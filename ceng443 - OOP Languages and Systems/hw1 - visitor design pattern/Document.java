package hw1;
import java.util.*;

public class Document implements DocElement {
	private String title;
	
	private ArrayList<DocElement> docs = new ArrayList<DocElement>();
	
	public Document(String title) { 
		this.title = title; 
	}
	
	public void setTitle(String newTitle) { 
		this.title = newTitle; 
	}
	
	public String getTitle() { 
		return this.title; 
	}
	
	public ArrayList<DocElement> getElements() { 
		return this.docs; 
	}
	
	public void setElements(ArrayList<DocElement> elements) {
		this.docs = elements;
	}
	
	public void add(DocElement d) {
		this.docs.add(d);
	}
	
	@Override
	public <T> T accept(TextVisitor<T> visitor) {
		return visitor.visit(this);
	}

}
