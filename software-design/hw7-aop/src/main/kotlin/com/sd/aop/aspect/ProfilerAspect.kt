
package com.sd.aop.aspect

import de.vandermeer.asciitable.AsciiTable
import org.aspectj.lang.ProceedingJoinPoint
import org.aspectj.lang.annotation.Around
import org.aspectj.lang.annotation.Aspect

@Aspect
class ProfilerAspect {
    private val methodToExecutions: MutableMap<String, MutableList<Long>> = mutableMapOf()

    @Around("execution(* com.sd.aop.example.Example.*(..))")
    fun logMethodExecutionTime(jointPoint: ProceedingJoinPoint): Any? {
        val name = "${jointPoint.signature.declaringTypeName}.${jointPoint.signature.name}"
        val startTime = System.nanoTime()
        val res = jointPoint.proceed(jointPoint.args)
        val endTime = System.nanoTime()
        methodToExecutions.computeIfAbsent(name) { mutableListOf() }.add(endTime - startTime)
        return res
    }

    fun printExecutionTime() {
        val asciiTable = AsciiTable().apply {
            addRule()
            addRow(
                "method name",
                "total execution time",
                "average execution time",
                "total method calls"
            )
            addRule()
        }

        methodToExecutions
            .forEach { (k, v) ->
                asciiTable.apply {
                    addRow(k, v.sum(), v.average(), v.size)
                    addRule()
                }
            }

        println(asciiTable.render(150))
    }
}