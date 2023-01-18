package ru.ivanau.sd.mvc.dao;

import org.springframework.jdbc.core.BeanPropertyRowMapper;
import org.springframework.jdbc.core.support.JdbcDaoSupport;
import ru.ivanau.sd.mvc.model.Task;

import javax.sql.DataSource;
import java.util.*;

public class TasklistJdbcDao extends JdbcDaoSupport implements TasklistDao {
    public TasklistJdbcDao(DataSource dataSource) {
        super();
        setDataSource(dataSource);
    }

    @Override
    public Map<String, List<Task>> getTasklists() {
        return new HashMap<>();
    }

    //    @Override
//    public Set<String> getTasklistNames() {
//        return new HashSet<>(getJdbcTemplate().query("SELECT DISTINCT Name FROM Tasklists",
//                new BeanPropertyRowMapper<>(String.class)));
//    }
//
//    @Override
//    public List<Task> getTasks(final String tasklistName) {
//        final String sql = String.format("SELECT * FROM Tasks WHERE TasklistId = (SELECT Id FROM Tasklists WHERE Name=%s)",
//                tasklistName);
//        return getJdbcTemplate().query(sql, new BeanPropertyRowMapper<>(Task.class));
//    }

    @Override
    public void addTasklist(final String tasklistName) {
        getJdbcTemplate().update("INSERT INTO Tasklists (Name) VALUES (?)", tasklistName);
    }

    @Override
    public void deleteTasklist(final String tasklistName) {
        getJdbcTemplate().update("DELETE FROM Tasklists WHERE Name = ?", tasklistName);
    }

    @Override
    public void addTask(final String tasklistName, final String taskName) {
        final String sql = String.format("INSERT INTO Tasks (Name, TasklistId, Finished)" +
                "VALUES (?, (SELECT Id FROM Tasklists WHERE Name = %s), FALSE)", tasklistName);
        getJdbcTemplate().update(sql, taskName);
    }

    @Override
    public void finishTask(final int id) {
        final String sql = String.format("UPDATE Tasks SET Finished = TRUE WHERE Id = %d", id);
        getJdbcTemplate().update(sql);
    }
}
