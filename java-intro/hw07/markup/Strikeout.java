package markup;

import java.util.List;

public class Strikeout extends AbstractMarker<BasicMarkup> implements BasicMarkup {
    public Strikeout(final List<BasicMarkup> list) {
        super(list);
    }

    @Override
    public void toMarkdown(StringBuilder sb) {
        super.toMarkdown(sb, "~", "~");
    }

    @Override
    public void toHtml(StringBuilder sb) {
        super.toHtml(sb, "<s>", "</s>");
    }
}
