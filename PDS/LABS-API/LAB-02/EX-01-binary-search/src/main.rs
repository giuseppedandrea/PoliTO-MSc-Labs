use std::io;
use clap::Parser;
use binary_search::find;

#[derive(Parser)]
#[clap(author, version, about, long_about = None)]
struct Args {
    #[clap(value_parser)]
    key: i32,
}

fn main() {
    let key = Args::parse().key;

    let mut array: Vec<i32> = vec![];
    loop {
        let mut input = String::new();
        match io::stdin().read_line(&mut input) {
            Ok(0) => {
                break;
            }
            Ok(_) => {
                match input.trim().parse::<i32>() {
                    Ok(v) => {
                        array.push(v);
                    }
                    Err(e) => {
                        panic!("{}", e);
                    }
                }
            }
            Err(e) => {
                panic!("{}", e);
            }
        }
    }

    match find(array, key) {
        Some(idx) => println!("{}", idx),
        None => println!("-1"),
    }
}