package ru.ivanau.sd.graph;

import org.apache.commons.lang3.tuple.Pair;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

public class AdjacencyMatrixGraph implements AbstractEnumeratedGraph{
    List<List<Boolean>> adj;

//    @Override
//    public void setSize(final int nVertices) {
//        adj = IntStream.range(0, nVertices)
//                .mapToObj(i -> IntStream.range(0, nVertices)
//                        .mapToObj(j -> false)
//                        .toList())
//                .toList();
//    }
//
//    @Override
//    public void addEdge(final int v1, final int v2) {
//        adj.get(v1).set(v2, true);
//        adj.get(v2).set(v1, true);
//    }

        private void emptyGraph(int nVertices) {
        adj = new ArrayList<>(nVertices);
        for (int i = 0; i < nVertices; ++i) {
            adj.add(new ArrayList<>(nVertices));
            for (int j = 0; j < nVertices; ++j) {
                adj.get(i).add(false);
            }
        }
    }

    public AdjacencyMatrixGraph(List<Integer> rowEdges) {
        this(rowEdges.toArray(new Integer[0]));
    }

    public AdjacencyMatrixGraph(Integer... rowEdges) {
        if (rowEdges.length % 2 == 1) {
            throw new IllegalArgumentException("Incomplete edges array: odd amount of vertices");
        }
        int nVertices = Stream.of(rowEdges).max(Comparator.naturalOrder()).get() + 1;
        emptyGraph(nVertices);
        for (int i = 0; i < rowEdges.length; i += 2) {
            if (rowEdges[i] < 0 || rowEdges[i + 1] < 0) {
                throw new IllegalArgumentException("Vertex numbers should be non-negative");
            }
            adj.get(rowEdges[i]).set(rowEdges[i + 1], true);
            adj.get(rowEdges[i + 1]).set(rowEdges[i], true);
        }
    }

    @Override
    public int countVertices() {
        return adj.size();
    }

    @Override
    public List<Pair<Integer, Integer>> getEdges() {
        List<Pair<Integer, Integer>> edges = new ArrayList<>();
        for (int i = 0; i < adj.size(); ++i) {
            for (int j = 0; j < i; ++j) {
                if (adj.get(i).get(j)) {
                    edges.add(Pair.of(i, j));
                }
            }
        }
        return edges;
    }
}
