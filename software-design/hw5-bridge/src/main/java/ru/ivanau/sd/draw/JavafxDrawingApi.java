package ru.ivanau.sd.draw;

import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;

public class JavafxDrawingApi implements DrawingApi {
    private static Canvas canvas;
    private final List<Consumer<GraphicsContext>> drawingTasks = new ArrayList<>();

    public JavafxDrawingApi(int width, int height) {
        canvas = new Canvas(width, height);
    }

    @Override
    public long getDrawingAreaWidth() {
        return (long) canvas.getWidth();
    }

    @Override
    public long getDrawingAreaHeight() {
        return (long) canvas.getHeight();
    }

    @Override
    public void drawCircle(final double x, final double y, final double r) {
        drawingTasks.add(graphicsContext -> graphicsContext.fillOval(x - r, y - r, 2 * r, 2 * r));
    }

    @Override
    public void drawLine(final double x1, final double y1, final double x2, final double y2) {
        drawingTasks.add(graphicsContext -> graphicsContext.strokeLine(x1, y1, x2, y2));
    }

    @Override
    public void drawText(final String text, final double x, final double y) {
        drawingTasks.add(graphicsContext -> graphicsContext.strokeText(text, x, y));
    }

    @Override
    public void show() {
        final GraphicsContext graphicsContext2D = canvas.getGraphicsContext2D();
        drawingTasks.forEach(drawingTasks -> drawingTasks.accept(graphicsContext2D));
        JavafxApp.launch(JavafxApp.class);
    }

    public static class JavafxApp extends Application {
        @Override
        public void start(final Stage stage) throws Exception {
            final Group group = new Group();
            group.getChildren().add(canvas);
            final Scene scene = new Scene(group, Color.WHITE);
            stage.setScene(scene);
            stage.show();
            stage.setOnCloseRequest(windowEvent -> System.exit(0));
        }
    }
}
