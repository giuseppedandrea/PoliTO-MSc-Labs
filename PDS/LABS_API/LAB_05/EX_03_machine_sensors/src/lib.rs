pub mod cbv {
    use std::sync::{Condvar, Mutex};

    enum Phase {
        Reading,
        Writing,
    }

    struct Data<T> {
        phase: Phase,
        vals: Vec<T>,
    }

    pub struct CyclicBarrierValue<T> where T: Clone {
        size: usize,
        m: Mutex<Data<T>>,
        cv: Condvar,
    }

    impl<T> CyclicBarrierValue<T> where T: Clone {
        pub fn new(n: usize) -> Self {
            CyclicBarrierValue {
                size: n,
                m: Mutex::new(Data { phase: Phase::Reading, vals: Vec::new() }),
                cv: Condvar::new(),
            }
        }

        pub fn wait_vals(&self, value: T) {
            let mut data = self.m.lock().unwrap();
            data.vals.push(value);

            if data.vals.len() == self.size {
                data.phase = Phase::Writing;
                self.cv.notify_all();
            } else {
                data = self.cv.wait_while(data, |d| {
                    match d.phase {
                        Phase::Reading => true,
                        Phase::Writing => false,
                    }
                }).unwrap();
            }

            let _ = self.cv.wait_while(data, |d| {
                match d.phase {
                    Phase::Writing => true,
                    Phase::Reading => false,
                }
            }).unwrap();
        }

        pub fn read_vals(&self) -> Vec<T> {
            let mut data = self.m.lock().unwrap();
            data = self.cv.wait_while(data, |d| {
                match d.phase {
                    Phase::Reading => true,
                    Phase::Writing => false,
                }
            }).unwrap();

            let vals = data.vals.to_vec();

            data.vals = Vec::new();

            vals
        }

        pub fn reset(&self) {
            let mut data = self.m.lock().unwrap();
            data.phase = Phase::Reading;
            self.cv.notify_all();
        }
    }
}

pub mod cbc {
    use std::sync::mpsc::{sync_channel, SyncSender, Receiver};

    pub struct ChannelReceiver<T> where T: Clone {
        tx: SyncSender<Vec<T>>,
        rx: Receiver<Vec<T>>,
    }

    pub struct ChannelProcessor<T> where T: Clone {
        tx: SyncSender<Vec<T>>,
        rx: Receiver<Vec<T>>,
    }

    pub fn init<T>(n: usize) -> (Vec<ChannelReceiver<T>>, ChannelProcessor<T>) where T: Clone {
        let (mut tx, rx): (Vec<SyncSender<Vec<T>>>, Vec<Receiver<Vec<T>>>) = (0..n)
            .map(|_| sync_channel(0))
            .unzip();
        let (mut tx0, rx0) = sync_channel(0);

        tx.push(tx0);
        tx.rotate_left(1);
        tx0 = tx.remove(n);
        let v: Vec<ChannelReceiver<T>> = tx
            .into_iter()
            .zip(rx.into_iter())
            .map(|(tx, rx)| {
                ChannelReceiver {
                    tx,
                    rx,
                }
            }).collect();
        let sr = ChannelProcessor {
            tx: tx0,
            rx: rx0,
        };
        (v, sr)
    }

    impl<T> ChannelReceiver<T> where T: Clone {
        pub fn send(&self, value: T) {
            let mut v = self.rx.recv().unwrap();
            v.push(value);
            self.tx.send(v).unwrap();
        }

        pub fn reset(&self) {
            let _ = self.rx.recv().unwrap();
            self.tx.send(vec![]).unwrap();
        }
    }

    impl<T: Clone> ChannelProcessor<T> {
        pub fn receive(&self) -> Vec<T> {
            self.tx.send(vec![]).unwrap();
            let v = self.rx.recv().unwrap();
            return v;
        }

        pub fn reset(&self) {
            self.tx.send(vec![]).unwrap();
            let _ = self.rx.recv().unwrap();
        }
    }
}