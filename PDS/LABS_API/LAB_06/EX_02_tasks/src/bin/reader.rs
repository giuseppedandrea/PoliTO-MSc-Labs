use std::{env, fs, process};
use std::thread::sleep;
use std::time::Duration;

fn run_task(task: &str, duration: u64) {
    eprintln!("[READER #{}] running {} for {} ms", process::id(), task, duration);
    sleep(Duration::from_millis(duration));
}

fn main() {
    let args: Vec<String> = env::args().collect();

    let reader_stdin = std::io::stdin();

    loop {
        let mut buf = String::new();

        println!("req_lock");
        eprintln!("[READER #{}] lock requested", process::id());

        reader_stdin.read_line(&mut buf).unwrap();

        if buf.starts_with("lock_granted") {
            eprintln!("[READER #{}] lock granted", process::id());
            let lines = fs::read_to_string(&args[1]).unwrap();
            let mut lines: Vec<&str> = lines.split("\n").filter(|l| { l.len() > 0 }).collect();
            lines.reverse();
            match lines.pop() {
                Some(line) => {
                    let (task, duration) = line.split_at(line.find(" ").unwrap());
                    run_task(task.trim(), duration.trim().parse::<u64>().unwrap());
                    lines.reverse();
                    fs::write(&args[1], lines.join("\n")).unwrap();
                }
                None => {
                    eprintln!("[READER #{}] No tasks", process::id());
                    sleep(Duration::from_millis(1000));
                }
            }

            println!("release_lock");
            eprintln!("[READER #{}] lock released", process::id());
        } else {
            eprintln!("[READER #{}] unknown command by coordinator", process::id());
            break;
        }
    }
}
