package ru.ivanau.sd.graph;

import org.apache.commons.lang3.tuple.Pair;

import java.util.List;

public interface AbstractEnumeratedGraph {
//    void setSize(int nVertices);
//    void addEdge(int v1, int v2);
    int countVertices();
    List<Pair<Integer, Integer>> getEdges();
}
