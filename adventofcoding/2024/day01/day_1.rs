use std::fs::File;
use std::io::{BufReader, BufRead};

fn main() -> std::io::Result<()> {
    let file = File::open("day_1_input.txt")?;
    let reader = BufReader::new(file);
//    let calibrationsize:i32 = 0;
//    let first_digit:i16 = 0;
//    let last_digit:i16 = 0;

    for line in reader.lines() {
        let firstencounter: bool = false;
        let secondencounter: bool = false;
        let mut numarr = new Vec();
        for c in line?.chars() {:wq
            if c.is_numeric() {
                numarr.push()
            }
        }
    }

    return Ok(());
}
