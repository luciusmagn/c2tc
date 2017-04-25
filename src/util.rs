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

pub unsafe fn make_string(s: String) -> *mut c_char
{
    CString::new(s).unwrap().into_raw()
}

pub unsafe fn read_string(s: *mut c_char) -> String
{
    CStr::from_ptr(s).to_string_lossy().into_owned()
}

impl module
{
    pub unsafe fn new() -> module 
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
    pub unsafe fn new() -> import
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
	pub unsafe fn new() -> function
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
	pub unsafe fn new() -> symbol_type
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
	pub unsafe fn new() -> param
	{
		param
		{
			p_type: symbol_type::new(),
			name: make_string("placeholder".to_string()),
		}
	}
}

impl vector
{
	pub unsafe fn new() -> *mut vector
	{ 
		let mut x = vector_alloc();
		vector_init(x);
		x 
	}
	pub unsafe fn add(&mut self, item: *mut c_void)
	{
		vector_add(self as *mut vector, item);
	}
}