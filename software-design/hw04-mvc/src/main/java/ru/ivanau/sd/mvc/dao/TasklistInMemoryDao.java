package ru.ivanau.sd.mvc.dao;

import ru.ivanau.sd.mvc.model.Task;

import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;

public class TasklistInMemoryDao implements TasklistDao {
    final AtomicInteger taskId;
    final Map<String, List<Task>> tasklistByName;
    final Map<Integer, Task> taskById;

    public TasklistInMemoryDao() {
        taskId = new AtomicInteger(0);
        tasklistByName = new HashMap<>();
        taskById = new HashMap<>();
    }

    @Override
    public Map<String, List<Task>> getTasklists() {
        return tasklistByName;
    }

    @Override
    public void addTasklist(final String tasklistName) {
        tasklistByName.putIfAbsent(tasklistName, new ArrayList<>());
    }

    @Override
    public void deleteTasklist(final String tasklistName) {
        tasklistByName.remove(tasklistName);
    }

    @Override
    public void addTask(final String tasklistName, final String taskName) {
        final List<Task> tasklist = tasklistByName.get(tasklistName);
        if (tasklist == null)
            return;
        final Task task = new Task(taskId.getAndIncrement(), taskName);
        taskById.put(task.getId(), task);
        tasklist.add(task);
    }

    @Override
    public void finishTask(final int id) {
        final Task task = taskById.get(id);
        if (task != null)
            task.finish();
    }
}
