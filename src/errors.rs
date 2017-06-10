#![allow(unused_unsafe)]
use std::os::raw::c_char;
use std::process::exit;
use util::*;

extern
{
	pub static mut throw_errors: i32;

	pub static mut erdfail: error_t;
	pub static mut emafail: error_t;
	pub static mut enoargs: error_t;
	pub static mut enoaccs: error_t;
	pub static mut ebadtok: error_t;
	pub static mut enotoks: error_t;
	pub static mut eparsef: error_t;
}

#[repr(C)]
pub struct error_t
{
	pub code: i32,
	pub name: *const c_char,
}

impl error_t
{
	pub fn new(code: i32, name: *const c_char) -> error_t
	{
		error_t
		{
			code: code,
			name: name,
		}
	}
}

#[no_mangle]
pub fn throw(etype: *const error_t, msg: *const c_char)
{
	println!("{} ({}): {}",
		str_c!((*etype).name),
		unsafe {(*etype).code},
		str_c!(msg)
	);
	unsafe { throw_errors+=1; }
}

pub fn error(etype: *const error_t, msg: &str) -> !
{
	throw(etype, c_str!(msg));
	unsafe { exit((*etype).code); }
}

#[no_mangle]
pub fn get_file_error(filename: *const c_char) -> *const c_char
{
	c_str!(
		&format!("Failed to parse file {} {} {}.",
			GREEN,
			str_c!(filename),
			RESET
		)
	)
}

#[no_mangle]
pub unsafe fn init_errors()
{
	erdfail = error_new(1, c_str!("ERDFAIL"));
	emafail = error_new(2, c_str!("EMAFAIL"));
	enoargs = error_new(3, c_str!("ENOARGS"));
	enoaccs = error_new(4, c_str!("ENOACCS"));
	ebadtok = error_new(5, c_str!("EBADTOK"));
	enotoks = error_new(6, c_str!("ENOTOKS"));
	eparsef = error_new(7, c_str!("EPARSEF"));
}

#[no_mangle]
pub fn error_new(code: i32, name: *const c_char) -> error_t
{
	error_t::new(code, name)
}
