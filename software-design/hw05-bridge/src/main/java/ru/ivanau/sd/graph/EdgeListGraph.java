package ru.ivanau.sd.graph;

import org.apache.commons.lang3.tuple.Pair;

import java.util.ArrayList;
import java.util.List;

import static java.lang.Math.max;

public class EdgeListGraph implements AbstractEnumeratedGraph {
    private int nVertices;
    private List<Pair<Integer, Integer>> edges;

//    @Override
//    public void setSize(final int nVertices) {
//        this.nVertices = nVertices;
//    }
//
//    @Override
//    public void addEdge(final int v1, final int v2) {
//        edges.add(Pair.of(v1, v2));
//        nVertices = Math.max(nVertices, v1);
//        nVertices = Math.max(nVertices, v2);
//    }

    public EdgeListGraph(List<Integer> rowEdges) {
        this(rowEdges.toArray(new Integer[0]));
    }

    public EdgeListGraph(Integer... rowEdges) {
        if (rowEdges.length % 2 == 1) {
            throw new IllegalArgumentException("Incomplete edges array: odd amount of vertices");
        }
        nVertices = 0;
        edges = new ArrayList<>();
        for (int i = 0; i < rowEdges.length; i += 2) {
            if (rowEdges[i] < 0 || rowEdges[i + 1] < 0) {
                throw new IllegalArgumentException("Vertex numbers should be non-negative");
            }
            edges.add(Pair.of(rowEdges[i], rowEdges[i + 1]));
            nVertices = max(nVertices, rowEdges[i] + 1);
            nVertices = max(nVertices, rowEdges[i + 1] + 1);
        }
    }

    @Override
    public int countVertices() {
        return nVertices;
    }

    @Override
    public List<Pair<Integer, Integer>> getEdges() {
        return List.copyOf(edges);
    }
}
