package hw1;

public class XDotZeroIsZeroRule implements Rule {
	private Var x;
	private MathExpression premise;
	private MathExpression entails;
	
	public XDotZeroIsZeroRule(Var x) {
		this.x = x;
		this.premise = new Op("*", x, new Num(0));
		this.entails = new Num(0);
	}
	
	public Var getX() { return this.x; }
	
	@Override
	public MathExpression getEntails() {
		return entails;
	}

	@Override
	public MathExpression getPremise() {
		return premise;
	}
}