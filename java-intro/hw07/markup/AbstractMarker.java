package markup;

import java.util.List;

public abstract class AbstractMarker<T extends Markable> {
    private final List<T> list;

    protected AbstractMarker(final List<T> list) {
        this.list = list;
    }

    public void toMarkdown(final StringBuilder sb, final String openMarker, final String closeMarker) {
        sb.append(openMarker);
        for (T i : list) {
            i.toMarkdown(sb);
        }
        sb.append(closeMarker);
    }

    public void toHtml(final StringBuilder sb, final String openTag, final String closeTag) {
        sb.append(openTag);
        for (T i : list) {
            i.toHtml(sb);
        }
        sb.append(closeTag);
    }
}
