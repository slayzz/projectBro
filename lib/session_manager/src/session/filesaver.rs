
pub mod text {
  use std::fs::{File, OpenOptions};
  use std::string::ToString;
  use std::path::Path;

  use session::message::parse::*;
  use session::handlers::handlers::{WebsocketHandler, MessageHandler};

  pub const PATH_SESSION: &'static str = "../session.sess";

  pub fn run_session(text: &str) -> Option<String> {
    let path_of_file = Path::new(PATH_SESSION);
    let file: File = OpenOptions::new()
      .read(true)
      .create(true)
      .append(true)
      .open(path_of_file)
      .unwrap();


    println!("{}", text);
    let parsed_message = text.parse::<ParsedMessage>().unwrap();
    println!("{:?}", parsed_message);
    match parsed_message.from.as_ref() {
      "Server" => {
        return None;
      },
      "Websocket" => {
        let mut websocket_handler = WebsocketHandler::new(file);
        return websocket_handler.handle(parsed_message);
      },
      _ => unreachable!()
    }

    return None;
  }
}