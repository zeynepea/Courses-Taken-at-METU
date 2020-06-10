package hw1;

public interface MathVisitor<T> {
	T visit(Num num);
	T visit(Var var);
	T visit(Sym sym);
	T visit(Op op);
}
