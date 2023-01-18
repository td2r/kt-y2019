package ru.ivanau.sd.draw;

import lombok.Getter;
import lombok.RequiredArgsConstructor;

import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.font.FontRenderContext;
import java.awt.font.TextLayout;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.util.ArrayList;
import java.util.List;

@RequiredArgsConstructor
public class AwtDrawingApi implements DrawingApi {
    private final int width;
    private final int height;
    private final List<Shape> circles = new ArrayList<>();
    private final List<Shape> lines = new ArrayList<>();
    private final List<TextShape> texts = new ArrayList<>();

    @RequiredArgsConstructor
    private static class TextShape {
        @Getter
        final String text;
        @Getter
        final double x;
        @Getter
        final double y;
    }

    @Override
    public long getDrawingAreaWidth() {
        return width;
    }

    @Override
    public long getDrawingAreaHeight() {
        return height;
    }

    @Override
    public void drawCircle(final double x, final double y, final double r) {
        circles.add(new Ellipse2D.Double(x - r, y - r, 2 * r, 2 * r));
    }

    @Override
    public void drawLine(final double x1, final double y1, final double x2, final double y2) {
        lines.add(new Line2D.Double(new Point2D.Double(x1, y1), new Point2D.Double(x2, y2)));
    }

    @Override
    public void drawText(final String text, final double x, final double y) {
        texts.add(new TextShape(text, x, y));
    }

    @Override
    public void show() {
        final Frame frame = new AwtFrame();
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(final WindowEvent e) {
                System.exit(0);
            }
        });
        frame.setSize((int) getDrawingAreaWidth(), (int) getDrawingAreaHeight());
        frame.setVisible(true);
    }

    private class AwtFrame extends Frame {
        @Override
        public void paint(final Graphics g) {
            final Graphics2D graphics2D = (Graphics2D) g;
            circles.forEach(graphics2D::fill);
            lines.forEach(graphics2D::draw);

            final Font font = getFont().deriveFont(Font.BOLD, 20);
            final FontRenderContext frc = graphics2D.getFontRenderContext();
            texts.forEach(textShape -> new TextLayout(textShape.getText(), font, frc)
                    .draw(graphics2D, (float) textShape.getX(), (float) textShape.getY()));
        }
    }
}
