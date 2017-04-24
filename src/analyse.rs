#![crate_type = "staticlib"]
#![allow(unused_variables)]
#![allow(non_upper_case_globals)]
#![allow(dead_code)]
#![allow(improper_ctypes)]

use util::*;
use types::*;
use std::slice;
use std::ffi::CStr;
use std::ffi::CString;
use std::os::raw::c_void;
use std::os::raw::c_char;

mod util;
mod types;

extern
{
    fn puts(s: *const c_char);
    fn vector_alloc() -> *mut vector;
    fn vector_init(v: *mut vector);
    fn vector_add(l: *mut vector, d: *mut c_void);
    fn vector_total(l: *mut vector) -> i32;
    fn vector_get(l: *mut vector, index: i32) -> *mut c_void;
}

static mut this_module: *mut module = 0 as *mut module;

#[no_mangle]
pub unsafe extern fn analyse(ast_ptr: *mut mpc_ast_t)
{
    this_module = &mut module::new() as *mut module;

    vector_init((*this_module).types);
    vector_init((*this_module).imports);
    vector_init((*this_module).functions);

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
    let ref ast = *ast_ptr;
    let mut func: function = function 
    {
        name: make_string("placeholder".to_string()),
        public: false,
        param_count: 0,
        params: vector_alloc(),
        ret_type: symbol_type
        {
            name: make_string("placeholder".to_string()),
            indirection: 0,
            constant: false,
            volatile: false,
        },
    };

    if read_string((*ast.get_children()[0]).tag) == "public" { func.public = true; }

    if func.public
    {
        func.name = (*ast.get_children()[3]).contents;
        let ref child = *ast.get_children()[2];
        let tag = read_string(child.tag);
        match tag.as_ref()
        {
            "natives" => 
            { 
                println!("it's a native {}", read_string(child.contents) );
                let mut return_type: symbol_type = symbol_type
                {
                    name: child.contents,
                    indirection: 0,
                    constant: false,
                    volatile: false,
                };

                return_type.name = child.contents;
                func.ret_type = return_type;
            },
            "type" =>
            {
                let mut index = 0;
                let mut is_const = false;
                let mut is_vol = false;

                if read_string( (*child.get_children()[0]).contents ) == "const"
                {
                    is_const = true;
                    index += 1;
                }
                if read_string( (*child.get_children()[0]).contents ) == "volatile"
                {
                    is_vol = true;
                    index += 1;
                }

                let mut return_type: symbol_type = symbol_type
                {
                    name: (*child.get_children()[index]).contents,
                    indirection: 0,
                    constant: is_const,
                    volatile: is_vol,
                };

                for i in 0..child.children_num as usize
                {
                    if read_string( (*child.get_children()[i]).tag ) == "ptrop"
                    || read_string( (*child.get_children()[i]).tag ) == "index"
                    {
                        return_type.indirection += 1;
                    }
                }

                func.ret_type = return_type;
            },
            _ => {}
        }
    }
    else
    {
        func.name = (*ast.get_children()[2]).contents;
        let ref child = *ast.get_children()[1];
        let tag = read_string( child.tag );
        match tag.as_ref()
        {
            "natives" => 
            { 
                println!("it's a native {}", read_string(child.contents) );
                let mut return_type: symbol_type = symbol_type
                {
                    name: child.contents,
                    indirection: 0,
                    constant: false,
                    volatile: false,
                };

                return_type.name = child.contents;
                func.ret_type = return_type;
            },
            "type" =>
            {
                let mut index = 0;
                let mut is_const = false;
                let mut is_vol = false;

                if read_string( (*child.get_children()[0]).contents ) == "const"
                {
                    is_const = true;
                    index += 1;
                }
                if read_string( (*child.get_children()[0]).contents ) == "volatile"
                {
                    is_vol = true;
                    index += 1;
                }

                let mut return_type: symbol_type = symbol_type
                {
                    name: (*child.get_children()[index]).contents,
                    indirection: 0,
                    constant: is_const,
                    volatile: is_vol,
                };

                for i in 0..child.children_num as usize
                {
                    if read_string( (*child.get_children()[i]).tag ) == "ptrop"
                    || read_string( (*child.get_children()[i]).tag ) == "index"
                    {
                        return_type.indirection += 1;
                    }
                }

                func.ret_type = return_type;
            },
            _ => {}
        }
    }    
    println!("{}type {} with indirection {}", if func.ret_type.constant { "constant " } 
                                               else if func.ret_type.volatile { "volatile " } else { "" }, 
                                               read_string(func.ret_type.name), func.ret_type.indirection);
}
