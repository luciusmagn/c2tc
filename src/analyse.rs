#![crate_type = "staticlib"]
#![allow(unused_variables)]
#![allow(non_upper_case_globals)]

use std::ptr;
use std::slice;
use std::vec::Vec;
use std::ffi::CStr;
use std::ffi::CString;
use std::os::raw::c_void;
use std::os::raw::c_char;
use std::collections::LinkedList;

#[repr(C)]
pub struct mpc_state_t
{
    pos: i64,
    row: i64,
    col: i64,
}

#[repr(C)]
pub struct mpc_ast_t
{
    tag: *mut c_char,
    contents: *mut c_char,
    state: mpc_state_t,
    children_num: i32,
    children: *const *const mpc_ast_t,
}


#[repr(C)]
pub struct vector;

//TODO map structs to C

#[repr(C)]
pub struct module
{
    name: *mut c_char,
    imports: *mut vector,
    functions: *mut vector,
    types: *mut vector,
}

#[repr(C)]
pub struct import
{
    name: *mut c_char,
    alias: *mut c_char,
    local: bool,
    w_alias: bool,
}

#[repr(C)]
pub struct function
{
    name: *mut c_char,
    public : bool,
    param_count : i32,
    params: *mut vector,
    node: *const mpc_ast_t,
}

#[repr(C)]
pub struct param
{
    p_type: symbol_type,
    name: *mut c_char,
    default: String,
    w_default: bool,
}

#[repr(C)]
pub struct symbol_type
{
    name: *mut c_char,
    indirection: i32,
}

#[repr(C)]
pub struct user_type
{
    name: *mut c_char,
    t_type: type_kind,
}

#[repr(C)]
enum type_kind
{
    ALIAS,
    STRUCT,
    ENUM,
}

impl mpc_ast_t
{
    unsafe fn get_children(&self) -> &'static[*const mpc_ast_t]
    {
        slice::from_raw_parts(self.children, self.children_num as usize)
    }
}



extern
{
    fn puts(s: *const c_char);
    fn vector_alloc() -> *mut vector;
    fn vector_init(v: *mut vector);
    fn vector_add(l: *mut vector, d: *mut c_void);
    fn vector_total(l: *mut vector, carry: i32) -> i32;
    fn vector_get(l: *mut vector, index: i32, carry: i32) -> *mut c_void;
}

const RED      :&'static str = "\x1b[31m";
const GREEN    :&'static str = "\x1b[32m";
const YELLOW   :&'static str = "\x1b[33m";
const BLUE     :&'static str = "\x1b[34m";
const MAGENTA  :&'static str = "\x1b[35m";
const CYAN     :&'static str = "\x1b[36m";
const RESET    :&'static str = "\x1b[0m";

static mut this_module: *mut module = 0 as *mut module;

#[no_mangle]
pub unsafe extern fn analyse(ast_ptr: *mut mpc_ast_t)
{
    this_module = &mut (module 
        {
            name: make_string("placeholder".to_string()),
            functions: vector_alloc(),
            imports: vector_alloc(),
            types: vector_alloc(),
        }) as *mut module;

    println!("test: {} {}", CStr::from_ptr((*this_module).name).to_string_lossy().into_owned().len(), 
                            read_string((*this_module).name));
    process(ast_ptr, 0);
}

pub unsafe fn process(ast_ptr: *mut mpc_ast_t, level: usize)
{

    let ast = &*ast_ptr;
    let children = slice::from_raw_parts(ast.children, ast.children_num as usize);

    for i in 0 .. ast.children_num as usize
    {
        let tag = CStr::from_ptr( (*children[i]).tag ).to_string_lossy().into_owned();
        let ref child = *children[i];

        match tag.as_ref()
        {
            "func" =>
            {
                println!("{}: {}", MAGENTA.to_string() + &"tag" + &RESET, YELLOW.to_string() + &tag + &RESET);
                read_func(children[i] as *mut mpc_ast_t);
            },
            "module" =>
            {
                println!("{}: {}", MAGENTA.to_string() + &"tag", YELLOW.to_string() + &tag + &RESET);
                println!("{} {}:", GREEN.to_string() + &"module" + &RESET,
                        CStr::from_ptr( (*child.get_children()[0]).contents ).to_string_lossy().into_owned());
            },
            "statement" => println!("statement"),
            _ => {},
        }
        process(children[i] as *mut mpc_ast_t, level + 1);
    }

}

pub unsafe fn make_string(s: String) -> *mut c_char
{
    CString::new(s).unwrap().into_raw()
}

pub unsafe fn read_string(s: *mut c_char) -> String
{
    CStr::from_ptr(s).to_string_lossy().into_owned()
}

pub unsafe fn read_func(ast_ptr: *mut mpc_ast_t)
{
    
}
