package md2html;

public class Text implements TextElement {
    private final String text;

    public Text(final String  text) {
        this.text = text;
    }

    public void toText(final StringBuilder sb) {
        sb.append(text);
    }
}
