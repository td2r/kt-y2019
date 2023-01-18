package com.sd.aop.example

interface Example {
    fun fun1(): String
    fun fun2(): String
    fun fun3(): String
}

class ExampleImpl : Example {
    private fun concatBench(n: Int): String {
        var result = "a"
        repeat(n) { result += "b$result" }
        return result;
    }

    override fun fun1(): String {
        return concatBench(10)
    }

    override fun fun2(): String {
        return concatBench(15)
    }

    override fun fun3(): String {
        return concatBench(20)
    }
}