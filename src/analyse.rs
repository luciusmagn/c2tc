#![crate_type = "staticlib"]
#![allow(unused_variables)]
#![allow(non_upper_case_globals)]
#![allow(dead_code)]
#![allow(improper_ctypes)]

use util::*;
use types::*;
use std::slice;
use std::os::raw::c_void;

mod util;
mod types;

static mut this_module: *mut module = 0 as *mut module;

#[no_mangle]
pub unsafe extern fn analyse(ast_ptr: *mut mpc_ast_t)
{
    this_module = &mut module::new() as *mut module;

    process(ast_ptr, 0);
}

pub unsafe fn process(ast_ptr: *mut mpc_ast_t, level: usize)
{
    let ast = &*ast_ptr;
    let children = slice::from_raw_parts(ast.children, ast.children_num as usize);

    for i in 0 .. ast.children_num as usize
    {
        let tag = read_string( (*children[i]).tag );
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
                println!("{} {}:", GREEN.to_string() + &"module" + &RESET, read_string((*child.get_children()[0]).contents));
                (*this_module).name = (*child.get_children()[0]).contents;
            },
			
            "statement" => println!("statement"),
            _ => {},
        }
        process(children[i] as *mut mpc_ast_t, level + 1);
    }

}

pub unsafe fn read_func(ast_ptr: *mut mpc_ast_t)
{
    let ref ast = *ast_ptr;
    let mut base_index = 0;
    let mut func: function = function::new(); 

    if read_string((*ast.get_children()[0]).tag) == "public" { func.public = true; base_index = 1}
    
    func.name = (*ast.get_children()[base_index + 2]).contents;
    let ref child = *ast.get_children()[base_index + 1];
    let tag = read_string( child.tag );

    match tag.as_ref()
    {
        "natives" => 
        { 
            println!("it's a native {}", read_string(child.contents) );
            let mut return_type: symbol_type = symbol_type::new();
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

            let mut return_type: symbol_type = symbol_type::new();
            return_type.name = (*child.get_children()[index]).contents;
            return_type.constant = is_const;
            return_type.volatile = is_vol; 

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

	func.params = read_params(ast.get_children()[base_index + 3]);	

	(*(*this_module).functions).add(&mut func as *mut c_void);
    println!("{}type {} with indirection {}", if func.ret_type.constant { "constant " } 
                                               else if func.ret_type.volatile { "volatile " } else { "" }, 
                                               read_string(func.ret_type.name), func.ret_type.indirection);
}

pub unsafe fn read_params(ast_ptr: *const mpc_ast_t) -> *mut vector
{
	let ref ast = *ast_ptr;
	let params:*mut vector = vector::new();
	println!("read_params: {}",  read_string(ast.tag));
	
	params
}