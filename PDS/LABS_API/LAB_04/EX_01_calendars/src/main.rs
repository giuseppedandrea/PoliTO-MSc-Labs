use std::fs::File;
use std::io;
use std::io::{BufRead, BufReader};
use clap::Parser;
use crate::calendars::Calendar;

mod calendars {
    use std::cmp::{max, min};

    pub struct Calendar {
        bounds: (u32, u32),
        schedule: Vec<(u32, u32)>,
    }

    impl Calendar {
        pub fn new(lines: Vec<String>) -> Self {
            let mut intervals: Vec<(u32, u32)> = vec![];
            for line in lines.iter() {
                let tokens = line.split(" ").collect::<Vec<&str>>();
                intervals.push((Calendar::time_to_u32(tokens[0]), Calendar::time_to_u32(tokens[1])));
            }

            let bounds = intervals.remove(0);
            let schedule = intervals.clone();
            Calendar { bounds, schedule }
        }

        fn time_to_u32(time: &str) -> u32 {
            let tokens = time.split(":").collect::<Vec<&str>>();
            tokens[0].parse::<u32>().unwrap() * 60 + tokens[1].parse::<u32>().unwrap()
        }

        pub fn common_intervals(cal1: &Calendar, cal2: &Calendar, duration: u32) -> Vec<String> {
            let mut res = vec![];
            let mut iters = (cal1.schedule.iter(), cal2.schedule.iter());
            let mut heads = (iters.0.next(), iters.1.next());
            let start_day = max(cal1.bounds.0, cal2.bounds.0);
            let end_day = min(cal1.bounds.1, cal2.bounds.1);

            let mut start_possible_interval = start_day;
            loop {
                let possible_interval = match heads {
                    (Some(i1), Some(i2)) => {
                        if i1.0 < i2.0 {
                            heads.0 = iters.0.next();
                            *i1
                        } else {
                            heads.1 = iters.1.next();
                            *i2
                        }
                    },
                    (Some(i1), None) => {
                        heads.0 = iters.0.next();
                        *i1
                    },
                    (None, Some(i2)) => {
                        heads.1 = iters.1.next();
                        *i2
                    },
                    (None, None) => break,
                };

                if start_possible_interval < possible_interval.0 && possible_interval.0 - start_possible_interval >= duration {
                    res.push(Calendar::interval_to_string((start_possible_interval, possible_interval.0)));
                }

                if start_possible_interval < possible_interval.1 {
                    start_possible_interval = possible_interval.1;
                }
            }

            if end_day - start_possible_interval > duration {
                res.push(Calendar::interval_to_string((start_possible_interval, end_day)));
            }

            res
        }

        fn interval_to_string(interval: (u32, u32)) -> String {
            format!("{:02}:{:02} {:02}:{:02}", interval.0 / 60, interval.0 % 60, interval.1 / 60, interval.1 % 60)
        }
    }
}

#[derive(Parser)]
#[clap(author, version, about, long_about = None)]
struct Args {
    #[clap(value_parser)]
    cal1: String,
    #[clap(value_parser)]
    cal2: String,
    #[clap(value_parser)]
    dur: u32,
}

fn read_file(path: String) -> Result<Vec<String>, io::Error> {
    let file = File::open(path)?;
    let reader = BufReader::new(file);
    let mut res = vec![];
    for line in reader.lines() {
        res.push(line?);
    }
    Ok(res)
}

fn main() {
    let file1 = read_file(Args::parse().cal1).expect("Error in parsing file");
    let cal1 = Calendar::new(file1);
    let file2 = read_file(Args::parse().cal2).expect("Error in parsing file");
    let cal2 = Calendar::new(file2);

    let free_intervals = Calendar::common_intervals(&cal1, &cal2, Args::parse().dur);
    free_intervals.iter().for_each(|free_interval| println!("{}", free_interval));
}

#[cfg(test)]
mod tests {
    use crate::Calendar;

    #[test]
    fn empty_calendar() {
        let cal1 = Calendar::new(vec!["08:00 17:00".to_string()]);
        let cal2 = Calendar::new(vec!["09:00 16:30".to_string(), "09:00 14:30".to_string(), "15:00 16:00".to_string()]);
        let intervals = Calendar::common_intervals(&cal1, &cal2, 10);
        let expected: Vec<String> = vec!["14:30 15:00".to_string(), "16:00 16:30".to_string()];
        assert_eq!(intervals, expected);
    }

    #[test]
    fn full_calendar() {
        let cal1 = Calendar::new(vec!["08:00 17:00".to_string()]);
        let cal2 = Calendar::new(vec!["09:00 16:30".to_string(), "09:00 14:30".to_string(), "14:30 16:30".to_string()]);
        let intervals = Calendar::common_intervals(&cal1, &cal2, 10);
        let expected: Vec<String> = vec![];
        assert_eq!(intervals, expected);
    }

    #[test]
    fn insufficient_duration() {
        let cal1 = Calendar::new(vec!["08:00 17:00".to_string()]);
        let cal2 = Calendar::new(vec!["09:00 16:30".to_string(), "09:00 14:00".to_string(), "15:30 16:30".to_string()]);
        let intervals = Calendar::common_intervals(&cal1, &cal2, 120);
        let expected: Vec<String> = vec![];
        assert_eq!(intervals, expected);
    }

    #[test]
    fn interval_at_beginning_and_at_end() {
        let cal1 = Calendar::new(vec!["08:00 17:00".to_string()]);
        let cal2 = Calendar::new(vec!["09:00 16:30".to_string(), "09:30 14:30".to_string(), "14:30 16:00".to_string()]);
        let intervals = Calendar::common_intervals(&cal1, &cal2, 10);
        let expected: Vec<String> = vec!["09:00 09:30".to_string(), "16:00 16:30".to_string()];
        assert_eq!(intervals, expected);
    }

    #[test]
    fn exact_interval() {
        let cal1 = Calendar::new(vec!["08:00 17:00".to_string()]);
        let cal2 = Calendar::new(vec!["09:00 16:30".to_string(), "09:00 14:30".to_string(), "15:00 16:30".to_string()]);
        let intervals = Calendar::common_intervals(&cal1, &cal2, 30);
        let expected: Vec<String> = vec!["14:30 15:00".to_string()];
        assert_eq!(intervals, expected);
    }
}
