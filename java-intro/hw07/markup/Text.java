package markup;

public class Text implements BasicMarkup {
    private final String text;

    public Text(final String text) {
        this.text = text;
    }

    @Override
    public void toMarkdown(final StringBuilder sb) {
        sb.append(text);
    }

    @Override
    public void toHtml(final StringBuilder sb) {
        sb.append(text);
    }
}
