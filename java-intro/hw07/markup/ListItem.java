package markup;

import java.util.List;

public class ListItem extends AbstractMarker<ListableMarkup> implements MarkupItem {
    public ListItem(final List<ListableMarkup> list) {
        super(list);
    }

    @Override
    public void toMarkdown(StringBuilder sb) {
        super.toMarkdown(sb, "", "");
    }

    @Override
    public void toHtml(StringBuilder sb) {
        super.toHtml(sb, "<li>", "</li>");
    }
}
