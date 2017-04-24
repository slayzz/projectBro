
pub mod text {
  use std::fs::{File, OpenOptions};
  use std::io::{BufReader, BufRead, Seek, SeekFrom, Write, Read};
  use std::string::ToString;
  use std::path::Path;
  const PATH_SESSION: &'static str = "../session.sess";

  pub enum SessionStatus {
    Message,
    NewSession,
    ReadAll
  }

  impl ToString for SessionStatus {
    fn to_string(&self) -> String {
      match self {
        &SessionStatus::Message => "Message".to_owned(),
        &SessionStatus::NewSession => "NewSession".to_owned(),
        &SessionStatus::ReadAll => "ReadAll".to_owned(),
      }
    }
  }


  pub fn run_session(text: &str) -> Option<(SessionStatus, String)> {
    let path_of_file = Path::new(PATH_SESSION);
    let file: File = OpenOptions::new()
      .read(true)
      .create(true)
      .append(true)
      .open(path_of_file)
      .unwrap();
    
    let message_type = get_message_type(text);

    match message_type.as_ref() {
      "Server" => {
        return None;
      },
      "Websocket" => {
        return websocket_handle(text, file);
      },
      _ => unreachable!()
    }
    return None;
  }

  fn read_all(file: &mut File) -> String {
    let mut buffer = String::new();
    file.read_to_string(&mut buffer);
    return buffer;
  }

  fn get_action(text: &str) -> String {
    let vec: Vec<&str> = text.split_terminator(':').collect();
    vec[1].to_owned()
  }

  fn get_message_type(text: &str) -> String {
    let vec: Vec<&str> = text.split_terminator(':').collect();
    vec[0].to_owned()
  }

  fn get_session_id_and_message(text: &str) -> (String, String) {
    let vec: Vec<&str> = text.split_terminator(':').collect();
    (vec[2].to_owned(), vec[3].to_owned())
  }

  fn get_session_id(text: &str) -> String {
    let vec: Vec<&str> = text.split_terminator(':').collect();
    (vec[2].to_owned())
  }

  fn websocket_handle(text: &str, mut file: File) 
    -> Option<(SessionStatus, String)> {
    let action = get_action(text);
    println!("{}", action);
    match action.as_ref() {
      "read" => Some((SessionStatus::ReadAll, read_all(&mut file))),
      "new" => {
        // Write new session
        let session_id = get_session_id(text);
        if check(&session_id, &file) {
          return None;
        }
        file.seek(SeekFrom::End(0));
        file.write_all((session_id.to_owned() + ":\n").as_bytes());
        Some((SessionStatus::NewSession, "".to_owned()))
      },
      "message" => {
        // Append to exists session
        let (session_id, message) = get_session_id_and_message(text);
        let buf_read = BufReader::new(file);
        let mut result_list: Vec<String> = Vec::new();
        for text in buf_read.lines() {
          let mut s = text.unwrap().to_owned();
          let cloned = s.clone();
          let split: Vec<&str> = cloned.split(':').collect();
          println!("{} == {}", split[0], session_id);
          if split[0] == session_id {
            s.push_str(&(message.clone() + ";\n"));
          } 
          result_list.push(s.to_owned());
        }

        let result = result_list
          .into_iter()
          .fold("".to_owned(), |acc, elem| acc + &elem);

        let path_of_file = Path::new(PATH_SESSION);
        let mut file_to_read: File = OpenOptions::new()
          .write(true)
          .open(path_of_file)
          .unwrap();
        file_to_read.write(result.as_bytes());
        Some((SessionStatus::NewSession, "".to_owned()))
      },
      _ => unreachable!()
    }
  }

  fn check(session_id: &str, file: &File) -> bool {
    let file_clone = file.try_clone();
    let buf_read = BufReader::new(file_clone.unwrap());
    buf_read.lines().any(|line| {
      let cloned_line = line.unwrap().clone();
      let vec: Vec<&str> = cloned_line.split_terminator(':').collect();
      println!("{} == {}", vec[0], session_id);
      vec[0].contains(session_id)
    })
  }

  

  fn write_new_session(file: &mut File, session_id: &str) {
  }

  fn append_to_session(file: &mut File, session_id: &str, message: &str) {
  }
}