package md2html;

import java.util.*;

public class MultiText implements TextElement {
    private final LinkedList<TextElement> list;

    public MultiText() {
        list = new LinkedList<TextElement>();
    }

    public MultiText(final LinkedList<TextElement> list) {
        this.list = list;
    }

    public boolean add(TextElement e) {
        return list.add(e);
    }

    public void addFirst(TextElement e) {
        list.addFirst(e);
    }

    public void toText(final StringBuilder sb) {
        for (TextElement i : list) {
            i.toText(sb);
        }
    }
}
