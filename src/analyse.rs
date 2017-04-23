#![crate_type = "staticlib"]

use std::ffi::CString;
use std::os::raw::c_char;

#[repr(C)]
struct mpc_state_t
{
    pos: i64,
    row: i64,
    col: i64,
}

#[repr(C)]
struct mpc_ast_t
{
    tag: *mut c_char,
    contents: *mut c_char,
    children_num: i32,
    children: *mut *mut mpc_ast_t,
}

extern
{
    fn puts(s: *const c_char);
}

#[no_mangle]
unsafe extern fn analyse()
{
    puts(CString::new("Hello world").unwrap().as_ptr());
}

#[no_mangle]
pub unsafe extern fn main()
{
    analyse();
}
