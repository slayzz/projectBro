
pub mod text {
  use std::fs::{File, OpenOptions};
  use std::io::{BufReader, BufRead, Seek, SeekFrom, Write};
  use std::string::ToString;
  use std::path::Path;
  const PATH_SESSION: &'static str = "../session.sess";

  pub enum SessionStatus {
    IsContain,
    NewSession
  }

  impl ToString for SessionStatus {
    fn to_string(&self) -> String {
      match self {
        &SessionStatus::IsContain => "IsContain".to_owned(),
        &SessionStatus::NewSession => "NewSession".to_owned(),
      }
    }
  }


  pub fn run_session(text: &str) -> Option<(SessionStatus, String)> {
    let path_of_file = Path::new(PATH_SESSION);
    let mut file: File = OpenOptions::new()
      .read(true)
      .create(true)
      .append(true)
      .open(path_of_file)
      .unwrap();

    if text.contains("FromServer") {
      let session_id = parse_from_server(text);
      
      match check_contain(&file, &session_id) {
        Some(message) => Some((SessionStatus::IsContain, message)),
        None => {
          write_new_session(&mut file, &session_id);
          Some((SessionStatus::NewSession, String::from("")))
        }

      }

    } else if text.contains("FromWebsocket") {
      let (session_id, message) = parse_from_websocket(text);
      return None;
      // return Some((SessionStatus::NewSession, "".to_owned()));
    } else {
      return None;
    }
  }

  fn check_contain(file: &File, session_id: &str) -> Option<String> {
    let buf_read = BufReader::new(file);
    for text in buf_read.lines() {
      let s = text.unwrap();
      let split: Vec<&str> = s.split(':').collect();
      if split[0] == session_id {
        return Some(split[0].to_owned());
      }
    }
    None
  }

  fn write_new_session(file: &mut File, session_id: &str) {
    file.seek(SeekFrom::End(0));
    file.write_all((session_id.to_owned() + "\n").as_bytes());
  }

  fn check(text: &str) -> Option<String> {
    if !text.contains("FromServer") || !text.contains("FromWebsocket") {
      return None;
    }
    return Some(text.to_owned());
  }

  fn parse(text: &str) -> Option<(String, String)> {
    let split: Vec<&str> = text.split(':').collect();
    Some((split[0].to_owned(), split[1].to_owned()))
  }


  fn parse_from_server(text: &str) -> String {
    let vec: Vec<&str> = text.split_terminator(':').collect();
    vec[1].to_owned()
  }

  fn parse_from_websocket(text: &str) -> (String, String){
    let vec: Vec<&str> = text.split_terminator(':').collect();
    (vec[1].to_owned(), vec[2].to_owned())
  }

}