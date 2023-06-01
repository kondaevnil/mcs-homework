sealed class Nested<T> {
    abstract fun isIterator() : Boolean
    abstract fun get() : Any
}
data class Element<T>(val value: T) : Nested<T>() {
    override fun isIterator(): Boolean {
        return false
    }

    override fun get(): Any {
        return value!!
    }
}
data class IteratorWrapper<T>(val iterator: Iterator<T>) : Nested<T>() {
    override fun isIterator(): Boolean {
        return true
    }

    override fun get(): Any {
        return iterator
    }
}

class NestedIterator<T>(private val nestedElements: List<Nested<T>>) : Iterator<T> {
    private var currentIterator: Iterator<T>? = null
    private var currentIndex = 0

    override fun hasNext(): Boolean {
        if (currentIterator?.hasNext() == true) {
            return true
        }
        while (currentIndex < nestedElements.size) {
            val nestedElement = nestedElements[currentIndex]
            if (nestedElement is Element<T>) {
                currentIterator = null
                return true
            } else if (nestedElement.isIterator()) {
                currentIterator = nestedElement.get() as? Iterator<T>
                if (currentIterator!!.hasNext()) {
                    return true
                }
                currentIndex++
            }
        }

        return false
    }

    override fun next(): T {
        if (currentIterator?.hasNext() == true) {
            return currentIterator!!.next()
        }
        while (currentIndex < nestedElements.size) {
            val nestedElement = nestedElements[currentIndex]
            if (nestedElement is Element<T>) {
                currentIterator = null
                currentIndex++
                return nestedElement.value
            } else if (nestedElement.isIterator()) {
                currentIterator = nestedElement as Iterator<T>
                if (currentIterator!!.hasNext()) {
                    return currentIterator!!.next()
                }
                currentIndex++
            }
        }

        throw NoSuchElementException()
    }
}