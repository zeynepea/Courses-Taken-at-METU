package hw1;

public class Paragraph implements DocElement {
	private String paragraph;
	
	public Paragraph(String parag) {
		this.paragraph = parag;
	}
	
	public String getText() { 
		return this.paragraph; 
	}
	
	@Override
	public <T> T accept(TextVisitor<T> visitor) {
		return visitor.visit(this);
	}
}