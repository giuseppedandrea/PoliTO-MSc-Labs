pub mod cb {
    use std::sync::{Condvar, Mutex};

    enum State {
        Entering(usize),
        Exiting(usize),
    }

    pub struct CyclicBarrier {
        size: usize,
        m: Mutex<State>,
        cv: Condvar,
    }

    impl CyclicBarrier {
        pub fn new(n: usize) -> Self {
            CyclicBarrier {
                size: n,
                m: Mutex::new(State::Entering(0)),
                cv: Condvar::new(),
            }
        }

        pub fn wait(&self) {
            let mut state = self.m.lock().unwrap();

            // Wait if threads are exiting
            state = self.cv.wait_while(state, |s| {
                match s {
                    State::Exiting(_) => true,
                    State::Entering(_) => false
                }
            }).unwrap();

            match *state {
                State::Entering(i) => {
                    if i == self.size - 1 {
                        *state = State::Exiting(self.size - 1);
                        self.cv.notify_all();
                    } else {
                        *state = State::Entering(i + 1);
                        state = self.cv.wait_while(state, |s| {
                            match s {
                                State::Entering(_) => true,
                                State::Exiting(_) => false
                            }
                        }).unwrap();

                        match *state {
                            State::Exiting(i) => {
                                if i == 1 {
                                    *state = State::Entering(0);
                                    self.cv.notify_all();
                                } else {
                                    *state = State::Exiting(i - 1);
                                }
                            }
                            _ => { panic!("Unexpected state!") }
                        }
                    }
                }
                _ => { panic!("Unexpected state!"); }
            }
        }
    }
}