use std::sync::Arc;
use cyclic_barrier::cb;

fn barrier_example() {
    let barrier = Arc::new(cb::CyclicBarrier::new(3));

    let mut vt = Vec::new();

    for i in 0..3 {
        let tbarrier = barrier.clone();

        vt.push(std::thread::spawn(move || {
            for j in 0..10 {
                tbarrier.wait();
                println!("after barrier {} {}\n", i, j);
            }
        }));
    }

    for t in vt {
        t.join().unwrap();
    }
}

fn main() {
    barrier_example();
}