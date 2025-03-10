use clap::Parser;
use regex::Regex;
use luhn::is_valid;

/// Check if credit card number is valid
#[derive(Parser)]
#[clap(author, version, about, long_about = None)]
struct Args {
    /// Credit card number (XXXX-XXXX-XXXX-XXXX)
    credit_card_number: String,
}

fn main() {
    let args = Args::parse();

    let re = Regex::new(r"^\d{4} \d{4} \d{4} \d{4}$").unwrap();
    if !re.is_match(&args.credit_card_number) {
        println!("invalid format");
        return;
    }

    if is_valid(&args.credit_card_number) {
        println!("valid");
    } else {
        println!("not valid");
    }
}
