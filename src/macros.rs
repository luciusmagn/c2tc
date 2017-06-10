#![allow(unused_unsafe)]

#[macro_export]
macro_rules! c_str /* C-ish String */
{
	($str:expr) =>
	{{
		use util::extend_lifetime;
		use std::ffi::CString;

		unsafe { CString::new(extend_lifetime($str)).unwrap().as_ptr() }
	}}
}

#[macro_export]
macro_rules! str_c /* String from C */
{
	($ptr:expr) =>
	{{
		use std::ffi::CStr;
		unsafe { CStr::from_ptr($ptr).to_string_lossy().into_owned() }
	}}
}

/// Dereference pointer and convert struct member `mem` to `String`
#[macro_export]
macro_rules! dfs /* dereferenced's member to String */
{
	($ptr:expr, $mem:ident) =>
	{{
		str_c!((*$ptr).$mem)
	}}
}

/// Dereference pointer and convert struct member `mem` to `usize`
#[macro_export]
macro_rules! dfu /* dereferenced's member to usize */
{
	($ptr:expr, $mem:ident) =>
	{{
		(*$ptr).$mem as usize
	}};
	($ptr:expr, $mem1:ident . $mem2:ident) =>
	{{
		(*$ptr).$mem1.$mem2 as usize
	}}
}

/// `print!` for lazy people - no format, args are thrown in delimited with spaces
#[macro_export]
macro_rules! trace
{
	($arg:expr) =>
	{{
		print!("{}", $arg);
	}};
	($($arg:expr)+) =>
	{{$(
		print!("{}", $arg);
	)+}};
}
