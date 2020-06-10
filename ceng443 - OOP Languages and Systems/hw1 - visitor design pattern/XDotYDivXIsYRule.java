package hw1;

public class XDotYDivXIsYRule implements Rule {
	private MathExpression premise;
	private MathExpression entails;
	private Var x;
	private Var y;

	public XDotYDivXIsYRule(Var x, Var y) {
		this.premise = new Op("/", new Op("*", x, y), x);
		this.entails = y;
		this.x = x;
		this.y = y;
	}
	
	public Var getX() { return this.x; }
	
	public Var getY() { return this.y; }

	@Override
	public MathExpression getEntails() {
		return entails;
	}

	@Override
	public MathExpression getPremise() {
		return premise;
	}
}