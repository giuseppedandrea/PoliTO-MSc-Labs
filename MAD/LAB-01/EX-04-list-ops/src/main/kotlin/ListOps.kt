fun <T> List<T>.customAppend(list: List<T>): List<T> {
    return list.customFoldLeft(this) { acc, el ->acc + el }
}

fun List<Any>.customConcat(): List<Any> {
    return this.customFoldLeft(emptyList()) { acc, el ->
        if (el is List<*>)
            acc.customAppend((el.filterIsInstance<Any>()).customConcat())
        else
            acc + el
    }
}

fun <T> List<T>.customFilter(predicate: (T) -> Boolean): List<T> {
    return this.customFoldLeft(emptyList()) { acc, el -> if (predicate(el)) acc + el else acc }
}

val List<Any>.customSize: Int get() = this.customFoldLeft(0) { acc, _ -> acc + 1}

fun <T, U> List<T>.customMap(transform: (T) -> U): List<U> {
    return this.customFoldLeft(emptyList()) { acc, el -> acc + transform(el) }
}

fun <T, U> List<T>.customFoldLeft(initial: U, f: (U, T) -> U): U {
    var acc = initial
    for (el in this) {
        acc = f(acc, el)
    }
    return acc
}

fun <T, U> List<T>.customFoldRight(initial: U, f: (T, U) -> U): U {
    return this.customReverse().customFoldLeft(initial) { acc, el -> f(el, acc) }
}

fun <T> List<T>.customReverse(): List<T> {
    return this.customFoldLeft(emptyList()) { acc, el -> listOf(el) + acc }
}
