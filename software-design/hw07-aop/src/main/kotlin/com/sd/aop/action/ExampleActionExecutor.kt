package com.sd.aop.action

import com.sd.aop.example.Example

class ExampleActionExecutor {
    private fun executeAction(example: Example, action: ExampleAction) {
        when (action) {
            is Fun1 -> example.fun1()
            is Fun2 -> example.fun2()
            is Fun0 -> example.fun3()
        }
    }

    fun executeActions(example: Example, actions: List<ExampleAction>) {
        actions.forEach { executeAction(example, it) }
    }
}