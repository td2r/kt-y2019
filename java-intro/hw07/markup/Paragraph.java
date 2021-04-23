package markup;

import java.util.List;

public class Paragraph extends AbstractMarker<BasicMarkup> implements ListableMarkup {
    public Paragraph(final List<BasicMarkup> list) {
        super(list);
    }

    @Override
    public void toMarkdown(final StringBuilder sb) {
        super.toMarkdown(sb, "", "");
    }

    @Override
    public void toHtml(final StringBuilder sb) {
        super.toHtml(sb, "", "");
    }
}
