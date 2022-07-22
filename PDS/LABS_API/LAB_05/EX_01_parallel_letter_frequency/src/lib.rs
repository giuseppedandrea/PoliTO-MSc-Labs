use std::collections::HashMap;

pub fn frequency(input: &[&str], worker_count: usize) -> HashMap<char, usize> {
    let mut result: HashMap<char, usize> = HashMap::new();

    let mut chunks: Vec<Vec<String>> = Vec::with_capacity(worker_count);
    let chunk_len = input.len() / worker_count + if input.len() % worker_count != 0 { 1 } else { 0 };
    for _ in 0..worker_count {
        chunks.push(Vec::with_capacity(chunk_len));
    }

    let mut idx: usize = 0;
    for s in input.iter() {
        chunks[idx].push(s.to_string());
        idx = (idx + 1) % worker_count;
    }

    let mut workers = vec![];
    for chunk in chunks {
        if chunk.len() > 0 {
            workers.push(std::thread::spawn(move || {
                let mut counts = HashMap::new();
                for s in chunk {
                    for c in s.chars() {
                        if c.is_alphabetic() {
                            let key = c.to_lowercase().next().unwrap();
                            let count = counts.entry(key).or_insert(0);
                            *count += 1;
                        }
                    }
                }
                counts
            }));
        }
    }

    for worker in workers {
        let ret = worker.join();
        match ret {
            Ok(counts) => {
                counts.into_iter().for_each(|(k, v)| {
                    let count = result.entry(k).or_insert(0);
                    *count += v;
                })
            }
            Err(e) => { eprintln!("{:?}", e) }
        }
    }

    result
}
