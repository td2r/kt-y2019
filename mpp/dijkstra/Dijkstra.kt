package dijkstra

import java.util.*
import java.util.concurrent.Phaser
import java.util.concurrent.atomic.AtomicInteger
import kotlin.concurrent.thread

private val NODE_DISTANCE_COMPARATOR = Comparator<Node> { o1, o2 -> o1!!.distance.compareTo(o2!!.distance) }

// Returns `Integer.MAX_VALUE` if a path has not been found.
fun shortestPathParallel(start: Node) {
    val workers = Runtime.getRuntime().availableProcessors()
    val q = MultiQueue(workers, NODE_DISTANCE_COMPARATOR)
    start.distance = 0
    q.add(start)
    val activeNodes = AtomicInteger(1)
    val onFinish = Phaser(workers + 1)
    repeat(workers) {
        thread {
            while (activeNodes.get() > 0) {
                val u = q.poll() ?: continue
                val du = u.distance
                for (e in u.outgoingEdges) {
                    while (true) {
                        val dv = e.to.distance
                        if (dv <= du + e.weight)
                            break
                        if (e.to.casDistance(dv, du + e.weight)) {
                            q.add(e.to)
                            activeNodes.incrementAndGet()
                            break
                        }
                    }
                }
                activeNodes.decrementAndGet()
            }
            onFinish.arrive()
        }
    }
    onFinish.arriveAndAwaitAdvance()
}