import java.util.*

fun isPalindrome(input: String): Boolean {
    val lettersOnly = input.filter { it.isLetter() }.lowercase(Locale.getDefault())
    return lettersOnly == lettersOnly.reversed()
}