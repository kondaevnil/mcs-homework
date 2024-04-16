import org.junit.jupiter.api.Test

class PalindromeTest {
    @Test
    fun test1() {
        assert(isPalindrome("Аргентина манит негра."))
    }

    @Test
    fun test2() {
        assert(isPalindrome(""))
    }

    @Test
    fun test3() {
        assert(!isPalindrome("asd"))
    }
}