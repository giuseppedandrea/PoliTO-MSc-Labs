use std::collections::HashMap;
use std::fs::File;
use std::io;
use std::io::{BufRead, BufReader};
use std::string::String;

fn main() {
    let stdin = io::stdin();
    for file_path in stdin.lock().lines() {
        let mut res: HashMap<String, usize> = HashMap::new();
        let file_handle = File::open(file_path.unwrap()).unwrap();

        let mut reader = BufReader::new(file_handle);
        let mut line = String::new();
        loop {
            match reader.read_line(&mut line) {
                Ok(0) => break,
                Ok(_) => {
                    let lowercase_line = line.to_lowercase();
                    let words = lowercase_line.split(|c: char| {
                        !(c.is_alphabetic() || c == '\'')
                    }).filter(|s| !s.is_empty());
                    for word in words {
                        *(res.entry(word.to_string()).or_insert(0)) += 1;
                    }
                }
                Err(e) => panic!("{}", e),
            }
        }

        println!("{}", serde_json::to_string(&res).unwrap());
    }
}