extern crate libc;
mod filesaver;

pub mod named_fifo { 
  use std::fs::{File, OpenOptions};
  use super::libc::*;
  use std::ffi::{CStr, CString};
  use std::io::{Write, Read};
  use session::filesaver::text::*;

  const SERVER_SESSION: &'static str = "/tmp/server_session.tmp";
  const DAEMON_SESSION: &'static str = "/tmp/daemon_session.tmp";

  pub struct FifoControl {
    read_fifo: File,
    write_fifo: File
  }

  impl FifoControl {
    pub fn new() -> Self {
      create_fifos().expect("Cant create fifos");
      FifoControl {
        read_fifo: OpenOptions::new()
                              .read(true)
                              .open(SERVER_SESSION)
                              .expect("Cant find file_server"),
        write_fifo: OpenOptions::new()
                              .write(true)
                              .open(DAEMON_SESSION)
                              .expect("Cant find file_daemon")
      }
    }

    pub fn read(&mut self, buffer: &mut Vec<u8>) {
      self.read_fifo.read_to_end(buffer).expect("Cant read from file");
    }

    pub fn write(&mut self, data: &[u8]) {
      self.write_fifo.write(data);
    }
  }

  fn create_fifos() -> Result<(), &'static str> {
    let mut err = unsafe { __errno_location() };
    unsafe { *err = 0 };

    let mut status: c_int = unsafe {
      mkfifo(CString::new(DAEMON_SESSION).unwrap().as_ptr(), S_IRWXU)
    };
    // let err_num: c_int = 17;
    unsafe {
      if status < 0 && *err != 17 as c_int {
        perror(CString::new("mkfifo").unwrap().as_ptr());
        return Err("Cant create daemon fifo");
      }
    }

    status = unsafe {
      mkfifo(CString::new(SERVER_SESSION).unwrap().as_ptr(), S_IRWXU)
    };

    unsafe {
      if status < 0 && *err != 17 as c_int {
        perror(CString::new("mkfifo").unwrap().as_ptr());
        return Err("Cant create server fifo");
      }
    }
    return Ok(());
  }  

  impl Drop for FifoControl {
    fn drop(&mut self) {
      unsafe {
        unlink(CString::new(SERVER_SESSION).unwrap().as_ptr());
        unlink(CString::new(DAEMON_SESSION).unwrap().as_ptr());
      }
    }
  }

  pub struct App;

  impl App {
    pub fn go() {
      loop {
        let mut fifo_control = FifoControl::new();
        let mut buffer = Vec::new();
        fifo_control.read(&mut buffer);

        let (session_status, message) =
          run_session(::std::str::from_utf8(&buffer).unwrap()).unwrap();
        println!("Status -> {}, messages -> {}",
                 session_status.to_string(),
                 message);
        // run_session("FromServer:qwerasdfzxcv");
        fifo_control.write(b"superlol\n");
        // ::std::thread::sleep(::std::time::Duration::new(3, 0));
      }
    }
  }
}