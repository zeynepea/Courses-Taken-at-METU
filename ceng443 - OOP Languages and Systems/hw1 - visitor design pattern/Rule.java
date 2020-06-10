package hw1;

public interface Rule {

	default public void clear(){
		this.getPremise().accept(new ClearVarsVisitor());
		this.getEntails().accept(new ClearVarsVisitor());
	}

	default public boolean apply(MathExpression me){
		this.clear();
		if(this.getPremise().match(me)) {
			return true;
		}
		else {
			this.clear();
			return false;
		}
	}

	public MathExpression getPremise();

	public MathExpression getEntails();

	default public MathExpression entails(MathExpression me) {
		this.apply(me);
		return this.getEntails();
	}
}
