#![allow(unused_variables)]
#![allow(non_upper_case_globals)]
#![allow(dead_code)]
#![allow(improper_ctypes)]

use std::slice;
use std::ffi::CStr;
use std::ffi::CString;
use std::os::raw::c_void;
use std::os::raw::c_char;

#[repr(C)]
pub struct mpc_state_t
{
    pub pos: i64,
    pub row: i64,
    pub col: i64,
}

#[repr(C)]
pub struct mpc_ast_t
{
    pub tag: *mut c_char,
    pub contents: *mut c_char,
    pub state: mpc_state_t,
    pub children_num: i32,
    pub children: *const *const mpc_ast_t,
}


#[repr(C)]
pub struct vector;

//TODO map structs to C

#[repr(C)]
pub struct module
{
    pub name: *mut c_char,
    pub imports: *mut vector,
    pub functions: *mut vector,
    pub types: *mut vector,
}

#[repr(C)]
pub struct import
{
    pub name: *mut c_char,
    pub alias: *mut c_char,
    pub local: bool,
    pub w_alias: bool,
}

#[repr(C)]
pub struct function
{
    pub name: *mut c_char,
    pub public : bool,
    pub param_count : i32,
    pub params: *mut vector,
    pub ret_type: symbol_type,
}

#[repr(C)]
pub struct param
{
    pub p_type: symbol_type,
    pub name: *mut c_char,
    pub default: String,
    pub w_default: bool,
}

#[repr(C)]
pub struct symbol_type
{
    pub name: *mut c_char,
    pub indirection: i32,
    pub constant: bool,
    pub volatile: bool,
}

#[repr(C)]
pub struct user_type
{
    pub name: *mut c_char,
    pub t_type: type_kind,
}

#[repr(C)]
pub enum type_kind
{
    ALIAS,
    STRUCT,
    ENUM,
}

impl mpc_ast_t
{
    pub unsafe fn get_children(&self) -> &'static[*const mpc_ast_t]
    {
        slice::from_raw_parts(self.children, self.children_num as usize)
    }
}
