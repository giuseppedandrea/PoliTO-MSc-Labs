use clap::Parser;

/// Capitalize first character of each word
#[derive(Parser)]
#[clap(author, version, about, long_about = None)]
struct Args {
    /// Sentence to capitalize
    sentence: String,
}

pub fn capitalize(s: &str) -> String {
    let mut new_s = String::new();
    let mut is_space_before = true;

    for c in s.chars() {
        let new_c = match c {
            ' ' => {
                is_space_before = true;
                ' '
            }
            _ => {
                if is_space_before {
                    is_space_before = false;
                    c.to_uppercase().nth(0).unwrap()
                } else {
                    c
                }
            }
        };
        new_s.push(new_c);
    }

    return new_s;
}

fn main() {
    let args = Args::parse();

    println!("Original sentence:");
    println!("{}", args.sentence);
    println!("Capitalize sentence:");
    println!("{}", capitalize(&args.sentence));
}

#[cfg(test)]
mod tests {
    use crate::capitalize;

    fn process_valid_case(input_string: &str, expected_string: &str) {
        assert_eq!(capitalize(input_string), expected_string);
    }

    #[test]
    fn test_string_with_multiple_words() {
        process_valid_case("this is a sentence", "This Is A Sentence");
    }

    #[test]
    fn test_string_with_single_word_no_spaces() {
        process_valid_case("word", "Word");
    }

    #[test]
    fn test_string_with_word_with_accented_initial() {
        process_valid_case("è un esempio", "È Un Esempio");
    }

    #[test]
    fn test_empty_string() {
        process_valid_case("", "");
    }

    #[test]
    fn test_string_with_multiple_spaces() {
        process_valid_case("this     is  a sentence", "This     Is  A Sentence");
    }
}
