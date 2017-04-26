pub mod parse {
  use std::str::FromStr;

  #[derive(Debug)]
  pub struct ParsedMessage {
    pub from: String,
    pub action: String,
    pub session_id: String,
    pub message: String
  }

  impl FromStr for ParsedMessage {
    type Err = String;
    fn from_str(s: &str) -> Result<Self, Self::Err> {
      let splits: Vec<_> = s.split_terminator(':').collect();
      match splits.len() {
        2 => Ok(Self { from: splits[0].to_owned(), action: splits[1].to_owned(), session_id: String::new(), message: String::new() }),
        3 => Ok(Self { from: splits[0].to_owned(), action: splits[1].to_owned(), session_id: splits[2].to_owned(), message: String::new() }),
        4 => Ok(Self { from: splits[0].to_owned(), action: splits[1].to_owned(), session_id: splits[2].to_owned(), message: splits[3].to_owned() }),
        _ => Err("Not correct message".to_owned())
      }
    }
  }
}