class IteratorOfIterators<T>(private val iterators: List<Iterator<T>>) : Iterator<T> {

    private var currentIterator: Iterator<T>? = null
    private var currentIndex = 0

    override fun hasNext(): Boolean {
        if (currentIterator == null || !currentIterator!!.hasNext()) {
            while (currentIndex < iterators.size) {
                currentIterator = iterators[currentIndex]
                currentIndex++
                if (currentIterator!!.hasNext()) {
                    return true
                }
            }
            return false
        }
        return true
    }

    override fun next(): T {
        if (!hasNext()) {
            throw NoSuchElementException()
        }
        return currentIterator!!.next()
    }
}
