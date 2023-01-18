package ru.ivanau.sd.mvc.model;

public class Task {
    private int id;
    private String name;
    private boolean finished;

    public Task() {
    }

    public Task(int id, String name) {
        this.id = id;
        this.name = name;
        finished = false;
    }

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public boolean isFinished() {
        return finished;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void finish() {
        finished = true;
    }
}
