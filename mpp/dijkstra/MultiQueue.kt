package dijkstra

import java.util.*
import java.util.concurrent.locks.ReentrantLock

class MultiQueue<T : Any?>(threads: Int, private val comparator: Comparator<T>,
                           private val n: Int = (2 * threads).coerceAtLeast(2),
                           private val queues: ArrayList<PriorityQueue<T>> = ArrayList(n),
                           private val locks: ArrayList<ReentrantLock> = ArrayList(n)
) {
    init {
        for (i in 0 until n) {
            queues.add(PriorityQueue(comparator))
            locks.add(ReentrantLock())
        }
    }

    fun poll(): T? {
        while (true) {
            var i = (0 until n).random()
            var j = (0 until n).random()
            if (i == j)
                continue
            if (i > j)
                i = j.also { j = i }
            if (locks[i].tryLock()) {
                if (locks[j].tryLock()) {
                    val iValue = queues[i].peek()
                    val jValue = queues[j].peek()
                    val returnValue =
                        if (iValue == null)
                            queues[j].poll()
                        else if (jValue == null || comparator.compare(iValue, jValue) <= 0)
                            queues[i].poll()
                        else
                            queues[j].poll()
                    locks[j].unlock()
                    locks[i].unlock()
                    return returnValue
                } else {
                    locks[i].unlock()
                }
            }
        }
    }

    fun add(element: T) {
        while (true) {
            val idx = (0 until n).random()
            if (locks[idx].tryLock()) {
                queues[idx].add(element)
                locks[idx].unlock()
                return
            }
        }
    }
}