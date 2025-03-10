import kotlin.math.max
import kotlin.math.min

data class MinesweeperBoard(val inputBoard: List<String>) {
    private var nRows = 0
    private var nCols = 0

    init {
        nRows = inputBoard.size
        nCols = inputBoard.firstOrNull()?.length ?: 0
    }

    fun withNumbers(): List<String> {
        var boardWithNumbers: MutableList<MutableList<Int>> = inputBoard.map {
            it.map {
                when (it) {
                    '*' -> -1
                    else -> 0
                }
            }.toMutableList()
        }.toMutableList()

        inputBoard.forEachIndexed(fun(row: Int, str: String) {
            str.forEachIndexed(fun(col: Int, square: Char) {
                if (square == '*') {
                    if ((row - 1) >= 0) {
                        for (j in max(0, col - 1)..min(nCols - 1, col + 1)) {
                            if (boardWithNumbers[row - 1][j] != -1)
                                boardWithNumbers[row - 1][j]++
                        }
                    }
                    if ((row + 1) < nRows) {
                        for (j in max(0, col - 1)..min(nCols - 1, col + 1)) {
                            if (boardWithNumbers[row + 1][j] != -1)
                                boardWithNumbers[row + 1][j]++
                        }
                    }
                    if ((col - 1) >= 0) {
                        if (boardWithNumbers[row][col - 1] != -1)
                            boardWithNumbers[row][col - 1]++
                    }
                    if ((col + 1) < nCols) {
                        if (boardWithNumbers[row][col + 1] != -1)
                            boardWithNumbers[row][col + 1]++
                    }
                }
            })
        })

        return boardWithNumbers.map {
            it.fold("") { str, curr ->
                when (curr) {
                    -1 -> "$str*"
                    0 -> "$str "
                    else -> "$str${(curr + '0'.code).toChar()}"
                }
            }.toString()
        }
    }
}
