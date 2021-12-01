use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where P: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn main() {
    let mut answer : u32 = 0;
    let mut previous = u32::MAX;
    let lines = read_lines("./input").unwrap(); 
    for line in lines {
        if let Ok(num_in_str) = line {
            let num = num_in_str.parse::<u32>().unwrap();
            if num > previous {answer = answer + 1;}
            previous = num;
        }
    }
    
    print!("The answer is: {}" , answer);
}
