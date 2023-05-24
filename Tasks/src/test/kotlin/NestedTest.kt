import org.junit.jupiter.api.Test

class NestedTest {
    @Test
    fun test1() {
        val nestedElements = listOf(
            Element("a"),
            IteratorWrapper(listOf("1", "2").iterator()),
            IteratorWrapper(listOf<String>().iterator()),
            IteratorWrapper(listOf("x", "y", "z").iterator()),
            Element("b"),
            Element("c"),
            IteratorWrapper(listOf("3").iterator()))

        val nestedIterator = NestedIterator(nestedElements)

        val result = listOf("a", "1", "2", "x", "y", "z", "b", "c", "3")

        var i = 0
        while (nestedIterator.hasNext()) {
            assert(nestedIterator.next() == result[i++])
        }
    }
}