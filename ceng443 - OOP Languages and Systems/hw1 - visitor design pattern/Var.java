package hw1;

public class Var implements MathExpression {
	private int id;
	private MathExpression prevMatch;
	
	public Var(int id) { 
		this.id = id;
		this.prevMatch = null;
	}
	
	public int getId() { 
		return this.id; 
	}
	
	public MathExpression getPreviousMatch() { 
		return this.prevMatch; 
	}
	
	public void setPreviousMatch(MathExpression me) { 
		this.prevMatch = me;
	}
	
	@Override
	public <T> T accept(MathVisitor<T> visitor) {
		return visitor.visit(this);
	}
	
	@Override
	public boolean match(MathExpression me) {
		boolean res;
		if(this.prevMatch == null) {
			this.prevMatch = me;
			res = true;
		}
		else {
			res = this.prevMatch.match(me);
		}
		return res;		
	}
}