package ru.ivanau.sd.mvc.controller;

import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import ru.ivanau.sd.mvc.dao.TasklistDao;
import ru.ivanau.sd.mvc.model.Task;

import java.util.List;
import java.util.Map;

@Controller
public class ProductController {
    private final TasklistDao tasklistDao;

    public ProductController(TasklistDao tasklistDao) {
        this.tasklistDao = tasklistDao;
    }

    @RequestMapping(value = "/add-tasklist", method = RequestMethod.POST)
    public String addTasklist(@ModelAttribute("tasklistName") String tasklistName) {
        tasklistDao.addTasklist(tasklistName);
        return "redirect:/get-all";
    }

    @RequestMapping(value = "/delete-tasklist", method = RequestMethod.POST)
    public String deleteTasklist(@RequestParam(value = "tasklistName") String tasklistName) {
        tasklistDao.deleteTasklist(tasklistName);
        return "redirect:/get-all";
    }

    @RequestMapping(value = "/add-task", method = RequestMethod.POST)
    public String addQuestion(@ModelAttribute("tasklistName") String tasklistName, @ModelAttribute("name") String name) {
        tasklistDao.addTask(tasklistName, name);
        return "redirect:/get-all";
    }

    @RequestMapping(value = "/finish-task", method = RequestMethod.POST)
    public String finishTask(@ModelAttribute("taskId") Integer id) {
        tasklistDao.finishTask(id);
        return "redirect:/get-all";
    }

    @RequestMapping(value = "/get-all", method = RequestMethod.GET)
    public String getProducts(ModelMap map) {
        prepareModelMap(map, tasklistDao.getTasklists());
        return "index";
    }

    private void prepareModelMap(ModelMap map, Map<String, List<Task>> tasklists) {
        map.addAttribute("tasklists", tasklists);
//        map.addAttribute("product", new Task());
//        map.addAttribute("filter", new TaskList());
    }
}
