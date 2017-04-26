pub mod handlers {
  use std::io::{BufReader, BufRead, Seek, SeekFrom, Write, Read};
  use std::fs::{File, OpenOptions};
  use std::path::Path;

  use session::message::parse::{ParsedMessage};
  use session::filesaver::text::PATH_SESSION;

  pub trait MessageHandler {
    fn handle(&mut self, message: ParsedMessage) -> Option<String>;
  }

  pub struct WebsocketHandler {
    answer_message: String,
    file: File
  }


  impl WebsocketHandler {
    pub fn new(file: File) -> Self {
      WebsocketHandler { answer_message: String::new(), file: file }
    }

    fn read_all(&mut self) -> String {
      let mut buffer = String::new();
      self.file.read_to_string(&mut buffer);
      return buffer;
    }
  }

  impl MessageHandler for WebsocketHandler {
    fn handle(&mut self, message: ParsedMessage) -> Option<String>{
      match message.action.as_ref() {
        "read" => Some(self.read_all()),
        "new" => {
          // Write new session
          if check(&message.session_id, &self.file) {
            return None;
          }
          self.file.seek(SeekFrom::End(0));
          self.file.write_all((message.session_id + ":\n").as_bytes());
          Some("".to_owned())
        },
        "message" => {
          // Append to exists session
          let buf_read = BufReader::new(&mut self.file);
          let mut result_list: Vec<String> = Vec::new();
          for text in buf_read.lines() {
            let mut s = text.unwrap().to_owned();
            let cloned = s.clone();
            let split: Vec<&str> = cloned.split(':').collect();
            if split[0] == message.session_id {
              s.push_str(&(message.message.clone() + ";"));
            }
            result_list.push(s.to_owned() + "\n");
          }

          let result = result_list
            .into_iter()
            .fold("".to_owned(), |acc, elem| acc + &elem);

          println!("Message Get");
          println!("{}", result);
          let path_of_file = Path::new(PATH_SESSION);
          let mut file_to_read: File = OpenOptions::new()
            .write(true)
            .open(path_of_file)
            .unwrap();
          file_to_read.write(result.as_bytes());
          Some( "".to_owned())
        },
        _ => unreachable!()
      }
    }
  }

  fn check(session_id: &str, file: &File) -> bool {
    let file_clone = file.try_clone();
    let buf_read = BufReader::new(file_clone.unwrap());
    buf_read.lines().any(|line| {
      let cloned_line = line.unwrap().clone();
      let vec: Vec<&str> = cloned_line.split_terminator(':').collect();
      vec[0].contains(session_id)
    })
  }

}
