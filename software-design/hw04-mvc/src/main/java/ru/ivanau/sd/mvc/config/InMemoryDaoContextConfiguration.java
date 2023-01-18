package ru.ivanau.sd.mvc.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import ru.ivanau.sd.mvc.dao.TasklistDao;
import ru.ivanau.sd.mvc.dao.TasklistInMemoryDao;

@Configuration
public class InMemoryDaoContextConfiguration {
    @Bean
    public TasklistDao productDao() {
        return new TasklistInMemoryDao();
    }
}
