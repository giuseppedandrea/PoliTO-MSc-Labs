use std::io::{BufRead, BufReader, Write};
use std::process::{Command, Stdio};
use std::sync::{Arc, Mutex};
use std::thread;
use std::thread::JoinHandle;
use clap::Parser;

const N_READERS: usize = 2;

#[derive(Parser)]
#[clap(author, version, about, long_about = None)]
struct Args {
    #[clap(value_parser)]
    file: String,
}

fn main() {
    let file: String = Args::parse().file;

    let m = Arc::new(Mutex::new(()));

    let mut rh: Vec<JoinHandle<()>> = Vec::new();

    for _ in 0..N_READERS {
        let file = file.clone();
        let m = m.clone();
        rh.push(thread::spawn(move || {
            let mut reader = Command::new("./target/debug/reader")
                .arg(file)
                .stdin(Stdio::piped())
                .stdout(Stdio::piped())
                .spawn()
                .expect("Failed to spawn reader");
            let mut reader_stdin = reader.stdin.take().unwrap();
            let mut reader_stdout = BufReader::new(reader.stdout.take().unwrap());

            loop {
                let mut buf = String::new();

                reader_stdout.read_line(&mut buf).unwrap();

                if buf.starts_with("req_lock") {
                    println!("[COORDINATOR] lock requested by reader #{}", reader.id());
                    let m = m.lock().unwrap();

                    println!("[COORDINATOR] lock granted by reader #{}", reader.id());
                    reader_stdin.write("lock_granted\n".as_bytes()).unwrap();

                    buf.clear();
                    match reader_stdout.read_line(&mut buf) {
                        Ok(_) => {
                            if buf.starts_with("release_lock") {
                                println!("[COORDINATOR] lock released by reader #{}", reader.id());
                            } else {
                                println!("[COORDINATOR] unknown command by {}", reader.id());
                                reader.wait().unwrap();
                                break;
                            }
                        }
                        Err(_) => {
                            println!("[COORDINATOR] reader #{} exited while locked", reader.id());
                            break;
                        }
                    }
                } else {
                    println!("[COORDINATOR] unknown command by {}", reader.id());
                    reader.wait().unwrap();
                    break;
                }
            }
        }));
    }

    rh.into_iter().for_each(|h| h.join().unwrap());
}
