use std::collections::HashMap;
use std::fs::read_dir;
use std::io::{BufRead, BufReader, Write};
use std::process::{ChildStdin, ChildStdout, Command, Stdio};
use std::thread;
use clap::Parser;

const N_CHILDREN: usize = 8;
const EXTENSION: &str = "txt";

#[derive(Parser)]
#[clap(author, version, about, long_about = None)]
struct Args {
    #[clap(value_parser)]
    path: String,
}

fn main() {
    let start_path: String = Args::parse().path;

    let mut child_stdin: Vec<ChildStdin> = Vec::new();
    let mut child_stdout: Vec<BufReader<ChildStdout>> = Vec::new();

    for _ in 0..N_CHILDREN {
        let mut child = Command::new("./target/debug/sandbox")
            .stdin(Stdio::piped())
            .stdout(Stdio::piped())
            .spawn()
            .expect("Failed to spawn child");
        child_stdin.push(child.stdin.take().unwrap());
        child_stdout.push(BufReader::new(child.stdout.take().unwrap()));
    }

    let indexer = thread::spawn(move || {
        let mut n: usize = 0;
        let mut dirs = vec![start_path];

        while !dirs.is_empty() {
            for entry in read_dir(dirs.pop().unwrap()).unwrap() {
                let path = entry.unwrap().path();
                if path.is_dir() {
                    dirs.push(path.to_str().unwrap().to_string());
                } else if path.is_file() && path.extension().unwrap_or_default() == EXTENSION {
                    let path_str = path.to_str().unwrap();
                    println!("Indexer - {}", path_str);
                    child_stdin[n % N_CHILDREN].write(format!("{}\n", path_str).as_bytes()).unwrap();
                    n += 1;
                }
            }
        }
    });

    let reader = thread::spawn(move || {
        let mut n: usize = 0;
        let mut res: HashMap<String, usize> = HashMap::new();

        let mut buf = String::new();
        loop {
            match child_stdout[n % N_CHILDREN].read_line(&mut buf) {
                Ok(0) => break,
                Ok(_) => {
                    let child_res: HashMap<String, usize> = serde_json::from_str(&buf).unwrap();
                    println!("child_res = {:?}", child_res);
                    for (k, v) in child_res {
                        *(res.entry(k).or_insert(0)) += v;
                    }
                    buf.clear();
                }
                Err(e) => panic!("{}", e),
            }

            n += 1;
        }

        res
    });

    indexer.join().unwrap();
    let res = reader.join().unwrap();
    println!("{:?}", res);
}