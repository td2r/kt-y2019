package markup;

import java.util.List;

public class Strong extends AbstractMarker<BasicMarkup> implements BasicMarkup {
    public Strong(final List<BasicMarkup> list) {
        super(list);
    }

    @Override
    public void toMarkdown(StringBuilder sb) {
        super.toMarkdown(sb, "__", "__");
    }

    @Override
    public void toHtml(StringBuilder sb) {
        super.toHtml(sb, "<strong>", "</strong>");
    }
}
