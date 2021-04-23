package markup;

public interface Markable {
    public void toMarkdown(StringBuilder sb);
    public void toHtml(StringBuilder sb);
}
