package hw1;

public interface MathExpression {
	public <T> T accept(MathVisitor<T> visitor);
	public boolean match(MathExpression me);
}