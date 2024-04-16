import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test
import kotlin.collections.listOf

class ItTest {
    @Test
    fun test1() {
        val iteratorOfIterators = IteratorOfIterators(
            listOf(
                iterator {
                    for (i in 1..10)
                        yield(i)
                },
                iterator {  },
                iterator {
                    yield(11)
                    yield(12)
                }
            )
        )

        for (i in 1..12)
        {
            assert(i == iteratorOfIterators.next())
            println(i)
        }

        assert(!iteratorOfIterators.hasNext())
    }

    @Test
    fun test2() {
        val iteratorOfIterators = IteratorOfIterators(listOf<Iterator<Int>>())
        assert(!iteratorOfIterators.hasNext())
    }
}