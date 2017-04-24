#![allow(unused_variables)]
#![allow(non_upper_case_globals)]
#![allow(dead_code)]
#![allow(improper_ctypes)]

use vector_alloc;
use types::*;
use std::ffi::CStr;
use std::ffi::CString;
use std::os::raw::c_char;

pub const RED      :&'static str = "\x1b[31m";
pub const GREEN    :&'static str = "\x1b[32m";
pub const YELLOW   :&'static str = "\x1b[33m";
pub const BLUE     :&'static str = "\x1b[34m";
pub const MAGENTA  :&'static str = "\x1b[35m";
pub const CYAN     :&'static str = "\x1b[36m";
pub const RESET    :&'static str = "\x1b[0m";

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
            types: vector_alloc(),
            imports: vector_alloc(),
            functions: vector_alloc(),
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
