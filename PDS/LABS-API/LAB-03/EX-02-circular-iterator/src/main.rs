#[derive(Clone)]
pub struct MyCycle<I: Clone + Iterator> {
    clone: I,
    iter: I,
    repeat: usize,
    infinite: bool,
}

impl<I> Iterator for MyCycle<I> where I: Clone + Iterator {
    type Item = I::Item;

    fn next(&mut self) -> Option<Self::Item> {
        let value = self.iter.next();
        match value {
            Some(value) => Some(value),
            None => {
                if self.infinite {
                    self.iter = self.clone.clone();
                    self.iter.next()
                } else {
                    if self.repeat > 1 {
                        self.repeat -= 1;
                        self.iter = self.clone.clone();
                        self.iter.next()
                    } else {
                        None
                    }
                }
            }
        }
    }
}

impl<I> MyCycle<I> where I: Clone + Iterator {
    pub fn new(iter: I, repeat: usize) -> Self {
        Self {
            clone: iter.clone(),
            iter,
            repeat,
            infinite: repeat == 0,
        }
    }
}

fn main() {
    let ci = MyCycle::new(vec![0, 1, 2].into_iter(), 2);
    for v in ci {
        println!("{}", v);
    }
}

#[cfg(test)]
mod tests {
    use crate::MyCycle;

    #[test]
    fn my_cycle_with_infinite_cycles_from_zero_element_iter() {
        let v0: Vec<usize> = vec![];
        let i0 = v0.iter();
        let mut ci0 = MyCycle::new(i0, 0);
        assert_eq!(ci0.next(), None);
    }

    #[test]
    fn my_cycle_with_finite_cycles_from_zero_element_iter() {
        let v0: Vec<usize> = vec![];
        let i0 = v0.iter();
        let mut ci0 = MyCycle::new(i0, 867);
        assert_eq!(ci0.next(), None);
    }

    #[test]
    fn my_cycle_with_finite_cycles_from_iter() {
        let l = 4;
        let v: Vec<usize> = vec![1; l];
        let i = v.iter();
        let n = 5;
        let ci = MyCycle::new(i, n);
        assert_eq!(ci.count(), l * n);
    }

    #[test]
    fn my_cycle_from_another_my_cycle() {
        let l = 5;
        let v: Vec<usize> = vec![1; l];
        let i = v.iter();
        let n2 = 5;
        let ci2 = MyCycle::new(i, n2);
        let n1 = 3;
        let ci1 = MyCycle::new(ci2, n1);
        assert_eq!(ci1.count(), l * n1 * n2);
    }

    #[test]
    fn chain_two_my_cycle() {
        let l1 = 2;
        let v1: Vec<usize> = vec![1; l1];
        let i1 = v1.iter();
        let n1 = 5;
        let ci1 = MyCycle::new(i1, n1);
        let l2 = 6;
        let v2: Vec<usize> = vec![2; l2];
        let i2 = v2.iter();
        let n2 = 1;
        let ci2 = MyCycle::new(i2, n2);
        let ci1ci2 = ci1.chain(ci2);
        assert_eq!(ci1ci2.count(), l1 * n1 + l2 * n2);
    }

    #[test]
    fn zip_two_my_cycle() {
        let l1 = 2;
        let v1: Vec<usize> = vec![1; l1];
        let i1 = v1.iter();
        let n1 = 5;
        let ci1 = MyCycle::new(i1, n1);
        let l2 = 6;
        let v2: Vec<usize> = vec![2; l2];
        let i2 = v2.iter();
        let n2 = 1;
        let ci2 = MyCycle::new(i2, n2);
        let mut ci1ci2 = ci1.zip(ci2);
        for idx in 0..std::cmp::min(l1 * n1, l2 * n2) {
            assert_eq!(ci1ci2.next(), Some((&v1[idx % l1], &v2[idx % l2])));
        }
    }
}
