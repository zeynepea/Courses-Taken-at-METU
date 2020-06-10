package hw1;

public class CountAtomicsVisitor implements MathVisitor<Integer> {
	
	@Override
	public Integer visit(Num num) {
		return 1;
	}

	@Override
	public Integer visit(Var var) {
		return 1;
	}

	@Override
	public Integer visit(Sym sym) {
		return 1;
	}

	@Override
	public Integer visit(Op op) {
		Integer firstResult = op.getFirst().accept(this);
		Integer secondResult = op.getSecond().accept(this);
		return firstResult + secondResult;
	}
}