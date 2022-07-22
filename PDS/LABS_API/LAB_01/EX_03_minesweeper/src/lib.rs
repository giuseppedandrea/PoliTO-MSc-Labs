use std::cmp::min;

struct Board {
    board: Vec<Vec<u8>>,
    n_rows: usize,
    n_cols: usize,
}

impl Board {
    fn annotated(&self) -> Vec<String> {
        let mut board_annotated = vec![];

        for i in 0..self.n_rows {
            let mut row = String::new();
            for j in 0..self.n_cols {
                let count = self.count_adjacent_mines(i, j);
                let c = match self.board[i][j] {
                    b' ' if count == 0 => { b' ' },
                    b' ' => { count + b'0' },
                    _  => { self.board[i][j] },
                };
                row.push(char::from(c));
            }
            board_annotated.push(row);
        }

        board_annotated
    }

    fn count_adjacent_mines(&self, x: usize, y: usize) -> u8 {
        let mut count = 0;

        if x >= self.n_rows || y >= self.n_cols {
            return 0;
        }

        if x >= 1 {
            for y in (if y >= 1 { y - 1 } else { 0 })..=(min(self.n_cols - 1, y + 1)) {
                if self.board[x - 1][y] == b'*' { count += 1; }
            }
        }

        if x + 1 < self.n_rows {
            for y in (if y >= 1 { y - 1 } else { 0 })..=(min(self.n_cols - 1, y + 1)) {
                if self.board[x + 1][y] == b'*' { count += 1; }
            }
        }

        if y >= 1 {
            if self.board[x][y - 1] == b'*' { count += 1; }
        }

        if y + 1 < self.n_cols {
            if self.board[x][y + 1] == b'*' { count += 1; }
        }

        count
    }
}

pub fn annotate(minefield: &[&str]) -> Vec<String> {
    let board = Board {
        board: minefield.iter().map(|&row| row.bytes().collect()).collect(),
        n_rows: minefield.len(),
        n_cols: if !minefield.is_empty() { minefield[0].len() } else { 0 }
    };

    board.annotated()
}

pub fn annotate2(minefield: String, rows: usize, cols: usize) -> String {
    let bytes = minefield.into_bytes();
    let mut bytes_board = vec![];

    for i in 0..rows {
        let mut bytes_row = vec![];
        for j in 0..cols {
            bytes_row.push(bytes[(i * rows) + j]);
        }
        bytes_board.push(bytes_row);
    }

    let board = Board {
        board: bytes_board,
        n_rows: rows,
        n_cols: cols
    };

    board.annotated().join("")
}
