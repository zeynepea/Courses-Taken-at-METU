package hw1;

public class PrintMathMLVisitor implements MathVisitor<String> {

	@Override
	public String visit(Num n) {
		return "<mrow><mn>" + n.getValue() +"</mn></mrow>";
	}

	@Override
	public String visit(Var v) {
		MathExpression prevMatch = v.getPreviousMatch();
		if(prevMatch == null) {
			return "<mrow><msub><mi>V</mi><mn>" + v.getId() + "</mn></msub></mrow>";
		}
		else {
			return "<mrow><msub><mi>V</mi><mn>" + v.getId() + "</mn></msub><mo>[</mo>" + prevMatch.accept(this) + "<mo>]</mo></mrow>";
		}
	}

	@Override
	public String visit(Sym s) {
		return "<mrow><mi>" + s.getValue() + "</mi></mrow>";
		
	}

	@Override
	public String visit(Op o) {
		String operand = o.getOperand();
		MathExpression first = o.getFirst();
		MathExpression second = o.getSecond();
		
		if(operand.equals("/")) {
			return "<mrow><mfrac>" + first.accept(this) + second.accept(this) + "</mfrac></mrow>";
		}

		if(operand.equals("+")) {
			return "<mrow><mo>(</mo>" + first.accept(this) + "<mo>+</mo>" + second.accept(this) + "<mo>)</mo></mrow>";
		}
		else if(operand.equals("*")) {
			return "<mrow><mo>(</mo>" + first.accept(this) + "<mo>&times;</mo>" + second.accept(this) + "<mo>)</mo></mrow>";
		}
		else{
			return "<mrow><mo>(</mo>" + first.accept(this) + "<mo>&vdash;</mo>" + second.accept(this) + "<mo>)</mo></mrow>";
		}
	}
}