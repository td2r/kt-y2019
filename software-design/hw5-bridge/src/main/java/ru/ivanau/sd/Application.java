package ru.ivanau.sd;

import ru.ivanau.sd.draw.AwtDrawingApi;
import ru.ivanau.sd.draw.DrawingApi;
import ru.ivanau.sd.draw.JavafxDrawingApi;
import ru.ivanau.sd.graph.AbstractEnumeratedGraph;
import ru.ivanau.sd.graph.AdjacencyMatrixGraph;
import ru.ivanau.sd.graph.EdgeListGraph;
import ru.ivanau.sd.visual.GraphVisualizer;

import java.io.*;
import java.util.List;
import java.util.function.Function;
import java.util.stream.Collectors;

public class Application {
    private static DrawingApi drawingApiFromToken(final String token) {
        if (token.equals("awt")) {
            return new AwtDrawingApi(600, 900);
        } else if (token.equals("javafx")) {
            return new JavafxDrawingApi(600, 900);
        } else {
            throw new IllegalArgumentException("unknown drawing api");
        }
    }

    private static Function<List<Integer>, AbstractEnumeratedGraph> graphFromToken(final String token) {
        if (token.equals("matrix")) {
            return AdjacencyMatrixGraph::new;
        } else if (token.equals("list")) {
            return EdgeListGraph::new;
        } else {
            throw new IllegalArgumentException("unknown graph type");
        }
    }

    private static AbstractEnumeratedGraph fillGraph(
            final Function<List<Integer>, AbstractEnumeratedGraph> graphConstructor,
            final String filePath)
    {
        try (final BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            final List<Integer> rowEdges = reader.lines()
                    .map(line -> {
                        final String[] vertices = line.split("\\s+");
                        return List.of(Integer.parseInt(vertices[0]), Integer.parseInt(vertices[1]));
                    })
                    .flatMap(List::stream)
                    .toList();
            return graphConstructor.apply(rowEdges);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static void main(String[] args) {
        for (int i = 0; i + 3 <= args.length; i += 3) {
            final DrawingApi drawingApi = drawingApiFromToken(args[i]);
            final Function<List<Integer>, AbstractEnumeratedGraph> graphConstructor = graphFromToken(args[i + 1]);
            final AbstractEnumeratedGraph graph = fillGraph(graphConstructor, args[i + 2]);
            new GraphVisualizer(drawingApi).draw(graph);
        }
    }
}
