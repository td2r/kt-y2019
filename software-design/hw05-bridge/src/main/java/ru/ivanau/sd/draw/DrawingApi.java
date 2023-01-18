package ru.ivanau.sd.draw;

public interface DrawingApi {
    long getDrawingAreaWidth();
    long getDrawingAreaHeight();
    void drawCircle(double x, double y, double r);
    void drawLine(double x1, double y1, double x2, double y2);
    void drawText(String text, double x, double y);
    void show();
}
