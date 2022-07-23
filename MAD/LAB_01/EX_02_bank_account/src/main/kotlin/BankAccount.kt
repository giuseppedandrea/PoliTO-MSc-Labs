class BankAccount {
    private var isOpen: Boolean = true
    var balance: Long = 0
        get() {
            if (!isOpen)
                throw IllegalStateException()

            return field
        }
        private set

    fun adjustBalance(amount: Long) {
        if (!isOpen)
            throw IllegalStateException()

        synchronized(this) {
            balance += amount
        }
    }

    fun close() {
        isOpen = false
    }
}
