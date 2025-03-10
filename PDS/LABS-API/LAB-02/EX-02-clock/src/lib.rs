use std::fmt::{Display, Formatter};
use std::ops::{Add, Sub};

#[derive(Debug, Eq, PartialEq)]
pub struct Clock {
    hours: i32,
    minutes: i32,
}

impl Display for Clock {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{:02}:{:02}", self.hours, self.minutes)
    }
}

impl Add for Clock {
    type Output = Self;

    fn add(self, other: Self) -> Self {
        Self::new(self.hours + other.hours, self.minutes + other.minutes)
    }
}

impl Add<i32> for Clock {
    type Output = Self;

    fn add(self, minutes: i32) -> Self {
        Self::new(self.hours, self.minutes + minutes)
    }
}

impl Sub for Clock {
    type Output = Self;

    fn sub(self, other: Self) -> Self {
        Self::new(self.hours - other.hours, self.minutes - other.minutes)
    }
}

impl Sub<i32> for Clock {
    type Output = Self;

    fn sub(self, minutes: i32) -> Self {
        Self::new(self.hours, self.minutes - minutes)
    }
}

impl Clock {
    pub fn new(hours: i32, minutes: i32) -> Self {
        let mut m = (hours * 60 + minutes) % (24 * 60);
        if m < 0 {
            m += 24 * 60;
        }

        Self {
            hours: m / 60,
            minutes: m % 60
        }
    }

    pub fn add_minutes(&self, minutes: i32) -> Self {
        Clock::new(self.hours, self.minutes + minutes)
    }
}
