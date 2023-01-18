package com.sd.aop

import com.sd.aop.action.ExampleActionGenerator
import com.sd.aop.action.ExampleActionExecutor
import com.sd.aop.aspect.ProfilerAspect
import com.sd.aop.example.Example
import org.springframework.context.ApplicationContext
import org.springframework.context.annotation.AnnotationConfigApplicationContext

fun main() {
    val ctx: ApplicationContext = AnnotationConfigApplicationContext(ContextConfiguration::class.java)
    val cache = ctx.getBean(Example::class.java) as Example

    val actionsGenerator = ExampleActionGenerator()
    val actionsExecutor = ExampleActionExecutor()

    val actions = actionsGenerator.generateActions(10_000)
    actionsExecutor.executeActions(cache, actions)

    val profilingAspect = ctx.getBean(ProfilerAspect::class.java)
    profilingAspect.printExecutionTime()
}