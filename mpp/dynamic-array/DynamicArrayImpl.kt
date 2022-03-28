import kotlinx.atomicfu.*

class DynamicArrayImpl<E> : DynamicArray<E> {
    private val sz = atomic(0)
    private val core = atomic(Core<E>(INITIAL_CAPACITY))

    override fun get(index: Int): E {
        require(index < sz.value)
        while (true) {
            val elem = core.value.array[index].value ?: continue
            return elem.first
        }
    }

    override fun put(index: Int, element: E) {
        require(index < sz.value)
        while (true) {
            val elem = core.value.array[index].value ?: continue
            if (!elem.second && core.value.array[index].compareAndSet(elem, Pair(element, false)))
                return
        }
    }

    override fun pushBack(element: E) {
        while (true) {
            val curSize = sz.value
            val curCore = core.value
            if (curSize < curCore.capacity) {
                if (sz.compareAndSet(curSize, curSize + 1)) {
                    curCore.array[curSize].value = Pair(element, false)
                    return
                }
            } else if (curCore.next.compareAndSet(null, Core(2 * curCore.capacity))) {
                val newCore = curCore.next.value!!
                for (i in 0 until curSize) {
                    while (true) {
                        val elem = curCore.array[i].value ?: continue
                        val frozenElem = Pair(elem.first, true)
                        if (curCore.array[i].compareAndSet(elem, frozenElem)) {
                            newCore.array[i].value = elem
                            break
                        }
                    }
                }
                core.value = newCore
            }
        }
    }

    override val size: Int get() = sz.value

    inner class Core<E>(
        val capacity: Int,
    ) {
        val array = atomicArrayOfNulls<Pair<E, Boolean>>(capacity)
        val next: AtomicRef<Core<E>?> = atomic(null)
    }
}

private const val INITIAL_CAPACITY = 1 // DO NOT CHANGE ME