package com.sd.aop

import com.sd.aop.aspect.ProfilerAspect
import com.sd.aop.example.Example
import com.sd.aop.example.ExampleImpl
import org.springframework.context.annotation.Bean
import org.springframework.context.annotation.Configuration
import org.springframework.context.annotation.EnableAspectJAutoProxy

@Configuration
@EnableAspectJAutoProxy
class ContextConfiguration {
    @Bean
    fun aspect(): ProfilerAspect {
        return ProfilerAspect()
    }

    @Bean
    fun example(): Example {
        return ExampleImpl()
    }
}