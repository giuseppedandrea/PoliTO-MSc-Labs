class Forth {
    private var stack: MutableList<Int> = mutableListOf()

    private var words: MutableMap<String, () -> Unit> = mutableMapOf(
        "+" to ::arithmeticAddition,
        "-" to ::arithmeticSubtraction,
        "*" to ::arithmeticMultiplication,
        "/" to ::arithmeticDivision,
        "DUP" to ::stackDup,
        "DROP" to ::stackDrop,
        "SWAP" to ::stackSwap,
        "OVER" to ::stackOver
    )

    private var userWords: MutableMap<String, List<String>> = mutableMapOf()

    fun evaluate(vararg line: String): List<Int> {
        for (arg in line) {
            val inputs = if (Regex("^:.*;$").matches(arg)) {
                listOf(arg.uppercase())
            } else {
                arg.split(' ').map { it.uppercase() }
            }
            for (input in inputs) {
                if (input.startsWith(":"))
                    defineUserWord(input)
                else
                    evaluateInput(input)
            }
        }

        return stack.toList()
    }

    private fun defineUserWord(input: String) {
        var userWord = input.split(' ').toMutableList()
        userWord.removeFirst()
        userWord.removeLast()
        val wordName = userWord.removeFirst()
        var definition = mutableListOf<String>()
        userWord.forEach {
            if (it in userWords)
                userWords[it]?.forEach { definition.add(it) }
            else
                definition.add(it)
        }
        when (wordName.toIntOrNull()) {
            null -> userWords[wordName] = definition
            else -> throw Exception("illegal operation")
        }
    }

    operator fun Regex.contains(text: CharSequence): Boolean = this.matches(text)

    private fun evaluateInput(input: String) {
        when (input) {
            in Regex("[0-9]+") -> stack.add(input.toInt())
            in userWords -> userWords[input]?.forEach { evaluateInput(it) }
            in words -> words[input]?.invoke()
            else -> throw Exception("undefined operation")
        }
    }

    private fun arithmeticAddition() {
        when (stack.size) {
            0 -> throw Exception("empty stack")
            1 -> throw Exception("only one value on the stack")
            else -> {
                val n1 = stack.removeLast()
                val n2 = stack.removeLast()
                stack.add(n1 + n2)
            }
        }
    }

    private fun arithmeticSubtraction() {
        when (stack.size) {
            0 -> throw Exception("empty stack")
            1 -> throw Exception("only one value on the stack")
            else -> {
                val n1 = stack.removeLast()
                val n2 = stack.removeLast()
                stack.add(n2 - n1)
            }
        }
    }

    private fun arithmeticMultiplication() {
        when (stack.size) {
            0 -> throw Exception("empty stack")
            1 -> throw Exception("only one value on the stack")
            else -> {
                val n1 = stack.removeLast()
                val n2 = stack.removeLast()
                stack.add(n1 * n2)
            }
        }
    }

    private fun arithmeticDivision() {
        when (stack.size) {
            0 -> throw Exception("empty stack")
            1 -> throw Exception("only one value on the stack")
            else -> {
                val n1 = stack.removeLast()
                if (n1 == 0)
                    throw Exception("divide by zero")
                val n2 = stack.removeLast()
                stack.add(n2 / n1)
            }
        }
    }

    private fun stackDup() {
        when (stack.size) {
            0 -> throw Exception("empty stack")
            else -> {
                val n = stack.last()
                stack.add(n)
            }
        }
    }

    private fun stackDrop() {
        when (stack.size) {
            0 -> throw Exception("empty stack")
            else -> {
                stack.removeLast()
            }
        }
    }

    private fun stackSwap() {
        when (stack.size) {
            0 -> throw Exception("empty stack")
            1 -> throw Exception("only one value on the stack")
            else -> {
                val n1 = stack.removeLast()
                val n2 = stack.removeLast()
                stack.add(n1)
                stack.add(n2)
            }
        }
    }

    private fun stackOver() {
        when (stack.size) {
            0 -> throw Exception("empty stack")
            1 -> throw Exception("only one value on the stack")
            else -> {
                val n = stack[stack.size - 2]
                stack.add(n)
            }
        }
    }
}
