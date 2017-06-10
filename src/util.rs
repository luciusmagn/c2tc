#![allow(unused_variables)]
#![allow(non_upper_case_globals)]
#![allow(dead_code)]
#![allow(improper_ctypes)]
#![allow(unused_mut)]

use types::*;
use std::ffi::CStr;
use std::ffi::CString;
use std::os::raw::c_char;
use std::mem::transmute;

pub const RED      :&'static str = "\x1b[31m";
pub const GREEN    :&'static str = "\x1b[32m";
pub const YELLOW   :&'static str = "\x1b[33m";
pub const BLUE     :&'static str = "\x1b[34m";
pub const MAGENTA  :&'static str = "\x1b[35m";
pub const CYAN     :&'static str = "\x1b[36m";
pub const RESET    :&'static str = "\x1b[0m";

pub fn make_string(s: String) -> *mut c_char
{
    CString::new(s).unwrap().into_raw()
}

pub fn read_string(s: *mut c_char) -> String
{
    unsafe { CStr::from_ptr(s).to_string_lossy().into_owned() }
}

impl Module
{
    pub fn new() -> Module
    {
        Module
        {
            types: Vec::new(),
            imports: Vec::new(),
            functions: Vec::new(),
            name: "placeholder".to_string(),
        }
    }
}

impl Import
{
    pub fn new() -> Import
    {
        Import
        {
            local: false,
            w_alias: false,
            name: "placeholder".to_string(),
            alias: "placeholder".to_string(),
        }
    }
}

impl Function
{
	pub fn new() -> Function
	{
		Function
		{
			name: "placeholder".to_string(),
			public: false,
			param_count: 0,
			params: Vec::new(),
			ret_type: SymbolType::new(),
		}
	}
}

impl SymbolType
{
	pub fn new() -> SymbolType
	{
		SymbolType
		{
			name: "placeholder".to_string(),
			indirection: 0,
			constant: false,
			volatile: false,
		}
	}
}

impl Param
{
	pub fn new() -> Param
	{
		Param
		{
			p_type: SymbolType::new(),
			name: "<empty>".to_string(),
		}
	}
}

impl UserType
{
	pub fn new() -> UserType
	{
		UserType
		{
			t_type: TypeKind::TEMP,
			name: "placeholder".to_string(),
			public: false
		}
	}
}

/*
** Defaults
*/

impl Default for UserType
{
	fn default() -> Self
	{
		Self::new()
	}
}

impl Default for Param
{
	fn default() -> Self
	{
		Self::new()
	}
}

impl Default for SymbolType
{
	fn default() -> Self
	{
		Self::new()
	}
}

impl Default for Function
{
	fn default() -> Self
	{
		Self::new()
	}
}

impl Default for Module
{
	fn default() -> Self
	{
		Self::new()
	}
}

impl Default for Import
{
	fn default() -> Self
	{
		Self::new()
	}
}

/*
** Danger zone
*/
pub struct R<'a>(&'a str);
pub unsafe fn extend_lifetime<'a>(r: &'a str) -> &'static str
{
    transmute::<&'a str, &'static str>(r)
}
