use clap::Parser;
use minesweeper::{annotate, annotate2};

/// Add the mine counts to a completed Minesweeper board
#[derive(Parser)]
#[clap(author, version, about, long_about = None)]
struct Args {
    /// Rows
    #[clap(long)]
    rows: usize,

    /// Columns
    #[clap(long)]
    cols: usize,

    /// Board
    board: String,
}

fn main() {
    let args = Args::parse();

    let mut board: Vec<&str> = vec![];
    for idx in 0..args.rows {
        board.push(&args.board[(args.cols * idx)..((args.cols * idx) + args.cols)]);
    }
    println!("{:#?}", annotate(&board));

    println!("{:#?}", annotate2(args.board, args.rows, args.cols));
}
