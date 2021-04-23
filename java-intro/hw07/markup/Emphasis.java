package markup;

import java.util.List;

public class Emphasis extends AbstractMarker<BasicMarkup> implements BasicMarkup {
    public Emphasis(final List<BasicMarkup> list) {
        super(list);
    }

    @Override
    public void toMarkdown(StringBuilder sb) {
        super.toMarkdown(sb, "*", "*");
    }

    @Override
    public void toHtml(StringBuilder sb) {
        super.toHtml(sb, "<em>", "</em>");
    }
}
