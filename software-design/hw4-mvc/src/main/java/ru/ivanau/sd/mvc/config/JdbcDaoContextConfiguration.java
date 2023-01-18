package ru.ivanau.sd.mvc.config;

import org.springframework.context.annotation.Bean;
import org.springframework.jdbc.datasource.DriverManagerDataSource;
import ru.ivanau.sd.mvc.dao.TasklistJdbcDao;

import javax.sql.DataSource;

public class JdbcDaoContextConfiguration {
    @Bean
    public TasklistJdbcDao productJdbcDao(DataSource dataSource) {
        return new TasklistJdbcDao(dataSource);
    }

    @Bean
    public DataSource dataSource() {
        DriverManagerDataSource dataSource = new DriverManagerDataSource();
        dataSource.setDriverClassName("org.sqlite.JDBC");
        dataSource.setUrl("jdbc:sqlite:tasklists.db");
        dataSource.setUsername("");
        dataSource.setPassword("");
        return dataSource;
    }
}
