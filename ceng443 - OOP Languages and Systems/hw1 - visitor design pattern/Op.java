package hw1;

public class Op implements MathExpression {
	private String operand;

	private MathExpression first;

	private MathExpression second;
	
	public Op(String operand,MathExpression first,MathExpression second) {
		this.operand = operand;
		this.first = first;
		this.second = second;
	}
	
	public String getOperand() { 
		return this.operand; 
	}
	
	public MathExpression getFirst() { 
		return this.first; 
	}
	
	public MathExpression getSecond() { 
		return this.second; 
	}

	@Override
	public <T> T accept(MathVisitor<T> visitor) {
		return visitor.visit(this);
	}
	
	@Override
	public boolean match(MathExpression me) {
		if(me instanceof Op) {
			String meOperand = ((Op)me).getOperand();
			boolean operandResult = meOperand.equals(this.operand);
			if(operandResult == false){
				return false;
			}

			MathExpression meFirst = ((Op)me).getFirst();
			boolean firstResult = this.first.match(meFirst);
			if(firstResult == false){
				return false;
			}

			MathExpression meSecond = ((Op)me).getSecond();
			boolean secondResult = this.second.match(meSecond);
			if(secondResult == false){
				return false;
			}

			return true;
		}
		return false;
	}
}