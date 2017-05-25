#![allow(unused_variables)]
#![allow(non_upper_case_globals)]
#![allow(dead_code)]
#![allow(improper_ctypes)]
#![allow(unused_mut)]

use types::*;
use std::ffi::CStr;
use std::ffi::CString;
use std::os::raw::c_char;
use std::os::raw::c_void;

pub const RED      :&'static str = "\x1b[31m";
pub const GREEN    :&'static str = "\x1b[32m";
pub const YELLOW   :&'static str = "\x1b[33m";
pub const BLUE     :&'static str = "\x1b[34m";
pub const MAGENTA  :&'static str = "\x1b[35m";
pub const CYAN     :&'static str = "\x1b[36m";
pub const RESET    :&'static str = "\x1b[0m";

extern
{
    pub fn puts(s: *const c_char);
    pub fn vector_alloc() -> *mut vector;
    pub fn vector_init(v: *mut vector);
    pub fn vector_add(l: *mut vector, d: *mut c_void);
    pub fn vector_total(l: *mut vector) -> i32;
    pub fn vector_get(l: *mut vector, index: i32) -> *mut c_void;
}

pub fn make_string(s: String) -> *mut c_char
{
    CString::new(s).unwrap().into_raw()
}

pub fn read_string(s: *mut c_char) -> String
{
    unsafe { CStr::from_ptr(s).to_string_lossy().into_owned() }
}

impl module
{
    pub fn new() -> module 
    {
        module
        {
            types: vector::new(),
            imports: vector::new(),
            functions: vector::new(),
            name: make_string("placeholder".to_string()),
        }
    }
}

impl import
{
    pub fn new() -> import
    {
        import
        {
            local: false,
            w_alias: false,
            name: make_string("placeholder".to_string()),
            alias: make_string("placeholder".to_string()),
        }
    }
}

impl function
{
	pub fn new() -> function
	{
		function
		{
			name: make_string("placeholder".to_string()),
			public: false,
			param_count: 0,
			params: vector::new(),
			ret_type: symbol_type::new(),
		}
	}
}

impl symbol_type
{
	pub fn new() -> symbol_type
	{
		symbol_type
		{
			name: make_string("placeholder".to_string()),
			indirection: 0,
			constant: false,
			volatile: false,
		}
	}
}

impl param
{
	pub fn new() -> param
	{
		param
		{
			p_type: symbol_type::new(),
			name: make_string("<empty>".to_string()),
		}
	}
}

impl user_type
{
	pub fn new() -> user_type
	{
		user_type
		{
			t_type: type_kind::TEMP,
			name: make_string("placeholder".to_string()),
			public: false
		}
	}
}

/*
** Defaults
*/

impl Default for user_type
{
	fn default() -> Self
	{
		Self::new()
	}
}

impl Default for param
{
	fn default() -> Self
	{
		Self::new()
	}
}

impl Default for symbol_type
{
	fn default() -> Self
	{
		Self::new()
	}
}

impl Default for function
{
	fn default() -> Self
	{
		Self::new()
	}
}

/*
** C vector
*/

impl vector
{
	pub fn new() -> *mut vector
	{ 
		unsafe
		{
			let mut x = vector_alloc();
			vector_init(x);
			x
		} 
	}
	pub fn add(&mut self, item: *mut c_void)
	{
		unsafe { vector_add(self as *mut vector, item); }
	}
	pub fn get(&mut self, index: usize) -> *mut c_void
	{
		unsafe { vector_get(self as *mut vector, index as i32) }
	}
	pub fn total(&mut self) -> usize
	{
		unsafe { vector_total(self as *mut vector) as usize }
	}
}
