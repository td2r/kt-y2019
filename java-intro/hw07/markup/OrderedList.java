package markup;

import java.util.List;

public class OrderedList extends AbstractMarker<MarkupItem> implements ListableMarkup {
    public OrderedList(final List<MarkupItem> list) {
        super(list);
    }

    @Override
    public void toMarkdown(StringBuilder sb) {
        super.toMarkdown(sb, "", "");
    }

    @Override
    public void toHtml(StringBuilder sb) {
        super.toHtml(sb, "<ol>", "</ol>");
    }
}
