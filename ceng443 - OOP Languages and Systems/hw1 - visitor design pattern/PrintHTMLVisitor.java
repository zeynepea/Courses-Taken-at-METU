package hw1;
import java.util.ArrayList;

public class PrintHTMLVisitor implements TextVisitor<String> {
	@Override
	public String visit(Document document) {	
		ArrayList<DocElement> docs = document.getElements();
		String body = "";
		for(DocElement doc : docs) {
			body = body.concat(doc.accept(this));
		}
		return "<html><head><title>" + document.getTitle() + "</title></head><body>" + body + "</body></html>";
	}

	@Override
	public String visit(EquationText equationText) {
		return "<math>" + equationText.getInnerMath().accept(new PrintMathMLVisitor()) + "</math>";
	}

	@Override
	public String visit(Paragraph paragraph) {
		return "<p>" + paragraph.getText() + "</p>";
	}
}