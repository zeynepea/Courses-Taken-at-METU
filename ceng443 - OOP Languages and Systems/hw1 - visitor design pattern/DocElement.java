package hw1;

public interface DocElement {
	public <T> T accept(TextVisitor<T> visitor);
}