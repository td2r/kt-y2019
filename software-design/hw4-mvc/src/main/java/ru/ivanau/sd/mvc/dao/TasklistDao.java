package ru.ivanau.sd.mvc.dao;

import ru.ivanau.sd.mvc.model.Task;

import java.util.List;
import java.util.Map;

public interface TasklistDao {
    Map<String, List<Task>> getTasklists();

    void addTasklist(final String tasklistName);
    void deleteTasklist(final String tasklistName);
    void addTask(final String tasklistName, final String taskName);
    void finishTask(final int id);
}
