package com.sd.aop.action

class ExampleActionGenerator {
    fun generateActions(n: Int): List<ExampleAction> {
        return (0 until n).map {
            when ((0..2).random()) {
                0 -> Fun0()
                1 -> Fun1()
                2 -> Fun2()
                else -> throw IllegalStateException("Impossible")
            }
        }
    }
}