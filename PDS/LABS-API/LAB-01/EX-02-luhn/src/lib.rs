/// Check a Luhn checksum.
pub fn is_valid(code: &str) -> bool {
    let stripped_code = code.to_string().replace(" ", "");

    if stripped_code.len() <= 1 { return false }

    let mut sum = 0;

    for (idx, chr) in stripped_code.chars().rev().enumerate() {
        match chr {
            '0'..='9' => {
                let mut digit = chr as u32 - '0' as u32;
                if idx % 2 == 1 { digit *= 2 }
                if digit > 9 { digit -= 9 }
                sum += digit;
            },
            _ => { return false; },
        }
    }

    sum % 10 == 0
}
