use std::sync::Arc;
use std::thread;
use std::thread::sleep;
use std::time::Duration;
use rand;
use machine_sensors::cbv;
use machine_sensors::cbc;

fn read_value() -> (u32, u32) {
    let t = (rand::random::<f32>() * 1000_f32) as u32;
    let v = (rand::random::<f32>() * 10_f32) as u32;
    sleep(Duration::from_millis(t.into()));
    (v, t)
}

fn set_speed(current_speed: u32, vals_sum: u32) -> u32 {
    let mut t = (rand::random::<f32>() * 1000_f32) as u32;
    loop {
        if vals_sum > 50 {
            if t <= current_speed { break; }
        } else {
            if t >= current_speed { break; }
        }
        t = (rand::random::<f32>() * 1000_f32) as u32;
    }
    t
}

fn machine_example_with_cyclic_barrier() {
    let barrier = Arc::new(cbv::CyclicBarrierValue::new(10));

    for i in 0..10 {
        let wbarrier = barrier.clone();

        thread::spawn(move || {
            loop {
                let (v, t) = read_value();
                println!("sensor #{} reads {} after {} ms", i, v, t);
                wbarrier.wait_vals(v);
            }
        });
    }

    let rbarrier = barrier.clone();
    let h = thread::spawn(move || {
        let mut speed: u32 = 500;
        loop {
            let vals = rbarrier.read_vals();
            println!("vals: {:?}", vals);
            let vals_sum = vals.iter().fold(0_u32, |acc, val| {
                acc + val
            });
            speed = set_speed(speed, vals_sum);
            println!("machine speed = {}", speed);
            sleep(Duration::from_millis(speed.into()));
            rbarrier.reset();
        }
    });

    h.join().unwrap();
}

fn machine_example_with_channel() {
    let (receivers, processor) = cbc::init(10);

    for (i, receiver) in receivers.into_iter().enumerate() {
        thread::spawn(move || {
            loop {
                let (v, t) = read_value();
                println!("sensor #{} reads {} after {} ms", i, v, t);
                receiver.send(v);
                receiver.reset();
            }
        });
    }

    let h = thread::spawn(move || {
        let mut speed: u32 = 500;
        loop {
            let vals = processor.receive();
            println!("vals: {:?}", vals);
            let vals_sum = vals.iter().fold(0_u32, |acc, val| {
                acc + val
            });
            speed = set_speed(speed, vals_sum);
            println!("machine speed = {}", speed);
            sleep(Duration::from_millis(speed.into()));
            processor.reset();
        }
    });

    h.join().unwrap();
}

fn main() {
    // machine_example_with_cyclic_barrier();
    machine_example_with_channel();
}