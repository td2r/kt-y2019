import kotlinx.atomicfu.AtomicBoolean
import kotlinx.atomicfu.AtomicRef
import kotlinx.atomicfu.atomic
import kotlinx.atomicfu.atomicArrayOfNulls
import java.util.*
import kotlin.random.Random

class FCPriorityQueue<E : Comparable<E>> {
    private val q = PriorityQueue<E>()
    private val lock = atomic(false)
    private val tasks = atomicArrayOfNulls<Task>(TASKS_ARR_SIZE)

    private fun tryAsCombiner(t: Task) {
        var idx: Int
        while (true) {
            idx = Random.nextInt(0, TASKS_ARR_SIZE)
            if (tasks[idx].compareAndSet(null, t)) {
                break
            }
        }
        while (true) {
            if (lock.compareAndSet(false, true)) {
                workAsCombiner()
                lock.value = false
                break
            }
            if (t.completed.value) {
                break
            }
        }
        tasks[idx].compareAndSet(t, null)
    }

    private fun workAsCombiner() {
        for (i in 0 until TASKS_ARR_SIZE) {
            val task = tasks[i].value
            if (task == null || task.completed.value)
                continue
            else if (task is PollTask<*>) {
                pollImpl(task as PollTask<E>)
            } else if (task is PeekTask<*>) {
                peekImpl(task as PeekTask<E>)
            } else {
                addImpl(task as AddTask<E>)
            }
            task.completed.value = true
        }
    }

    private fun pollImpl(t: PollTask<E>) {
        t.result.value = q.poll()
    }

    private fun peekImpl(t: PeekTask<E>) {
        t.result.value = q.peek()
    }

    private fun addImpl(t: AddTask<E>) {
        q.add(t.element.value)
    }

    /**
     * Retrieves the element with the highest priority
     * and returns it as the result of this function;
     * returns `null` if the queue is empty.
     */
    fun poll(): E? {
        val task = PollTask<E>()
        tryAsCombiner(task)
        return task.result.value
    }

    /**
     * Returns the element with the highest priority
     * or `null` if the queue is empty.
     */
    fun peek(): E? {
        val task = PeekTask<E>()
        tryAsCombiner(task)
        return task.result.value
    }

    /**
     * Adds the specified element to the queue.
     */
    fun add(element: E) {
        val task = AddTask<E>(element)
        tryAsCombiner(task)
    }

    inner open class Task {
        val completed: AtomicBoolean = atomic(false)
    }

    inner class PollTask<E> : Task() {
        val result: AtomicRef<E?> = atomic(null)
    }

    inner class PeekTask<E> : Task() {
        val result: AtomicRef<E?> = atomic(null)
    }

    inner class AddTask<E>(element: E) : Task() {
        val element: AtomicRef<E> = atomic(element)
    }
}

private val TASKS_ARR_SIZE = 20