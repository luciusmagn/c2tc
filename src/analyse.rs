//#![feature(plugin)]
//#![plugin(clippy)]
#![allow(unused_variables)]
#![allow(non_upper_case_globals)]
#![allow(dead_code)]
#![allow(improper_ctypes)]

use util::*;
use types::*;
use std::slice;
use std::mem;
use std::os::raw::c_void;
//use std::os::raw::c_char;

static mut this_module: *mut module = 0 as *mut module;

#[no_mangle]
pub unsafe extern fn analyse(ast_ptr: *mut mpc_ast_t)
{
    this_module = &mut module::new() as *mut module;

	let v = vector::new();
	(*v).add(mem::transmute( make_string("potato".to_string()) ));

    process(ast_ptr, 0);
    print_module();
}

pub unsafe fn process(ast_ptr: *mut mpc_ast_t, level: usize)
{
    let ast = &(*ast_ptr);
    let children = slice::from_raw_parts(ast.children, ast.children_num as usize);

    for child_ptr in children.iter().take(ast.children_num as usize)//0 .. ast.children_num as usize
    {
        let child = *child_ptr;
        let tag = read_string((*child).tag);

        match tag.as_ref()
        {
            "func" => read_func(child as *mut mpc_ast_t),
            "module" => (*this_module).name = (*(*child).get_children()[0]).contents,
            "import" => read_import(child as *mut mpc_ast_t),
            "usertype" => read_usertype(child as *mut mpc_ast_t),
            "statement" => println!("statement"),
            _ => {},
        }
        process(child as *mut mpc_ast_t, level + 1);
    }

}

pub unsafe fn read_import(ast_ptr: *mut mpc_ast_t)
{
	let ast = &(*ast_ptr);
	let mut imp = &mut(*Box::into_raw(Box::new(import::new())));
	imp.name = (*ast.get_children()[0]).contents;

	for i in 1..ast.children_num as usize
	{
		let content = read_string((*ast.get_children()[i]).contents);
		match content.as_ref()
		{
			"local" => imp.local = true,
			_ =>
			{ 
				imp.w_alias = true;
				imp.alias = (*ast.get_children()[i]).contents; 
			},
		}
	}

	(*(*this_module).imports).add(
		mem::transmute(imp as *mut import)
	);
}

pub unsafe fn read_usertype(ast_ptr: *mut mpc_ast_t)
{
	let ast = &(*ast_ptr);
	let mut index = 0;
	let mut t: &mut user_type = &mut(*Box::into_raw(Box::new(user_type::new())));

	if read_string((*ast.get_children()[0]).tag) == "public" { t.public = true; index = 1; }

	let child = &(*ast.get_children()[index]);
	let tag = read_string( (*ast.get_children()[index]).tag );

	t.name = (*child.get_children()[0]).contents;
	
	match tag.as_ref()
	{
		"globalunion" => t.t_type = type_kind::UNION,
		"structure" => t.t_type = type_kind::STRUCT,
		"enumtype" => t.t_type = type_kind::ENUM,
		"functype" => t.t_type = type_kind::FUNC,
		"alias" => t.t_type = type_kind::ALIAS,
		_ => {}
	}

	(*(*this_module).types).add(
		mem::transmute(t as *mut user_type)		
	);
}

pub unsafe fn read_func(ast_ptr: *mut mpc_ast_t)
{
    let ast = &(*ast_ptr);
    let mut base_index = 0;
    let mut func: &mut function = &mut(*Box::into_raw(Box::new(function::new()))); 

    if read_string((*ast.get_children()[0]).tag) == "public" { func.public = true; base_index = 1; }
    
    func.name = (*ast.get_children()[base_index + 2]).contents;
    let child = &(*ast.get_children()[base_index + 1]);
    let tag = read_string( child.tag );

    func.ret_type = read_type(child);

	func.params = read_params(ast.get_children()[base_index + 3]);	

	(*(*this_module).functions).add(mem::transmute(func as *mut function));
}

/// read example
///	for i in 0..(*params).total()
///	{
///		let d = (*params).get(i) as *mut param;
///		println!(
///			"test param: {} {}", 
///			`read_string`( (*d).name ),
///			`read_string`( (*d).`p_type`.name ) 
///		);
///	}
pub unsafe fn read_params(ast_ptr: *const mpc_ast_t) -> *mut vector
{
	let ast = &(*ast_ptr);
	let params:*mut vector = vector::new();
	for i in 0..ast.children_num as usize
	{
		let child = &(*ast.get_children()[i]);
		let mut p = &mut(*Box::into_raw(Box::new(param::new())));
		p.p_type = read_type( &(*child.get_children()[0]) );
		p.name = (*child.get_children()[1]).contents;

		vector_add(
			params,
			(p as *mut param) as *mut c_void
		);

		let p:*mut param = (*params).get(i) as *mut param;
	}

	params
}

pub unsafe fn read_param(ast_ptr: *const mpc_ast_t) -> param
{
	let ast = &(*ast_ptr);
	let mut p = param::new();
	p.p_type = read_type( &(*ast.get_children()[0]) );
	if ast.children_num != 1 {p.name = (*ast.get_children()[1]).contents };
	println!("{}", read_string(p.name));
	p
}

pub unsafe fn read_type(ast: &mpc_ast_t) -> symbol_type
{
	if read_string(ast.tag) == "natives" || ast.children_num == 0
	{
		let mut s: symbol_type = symbol_type::new();
		s.name = ast.contents;
		return s;
	}

    let mut index = 0;
    let mut is_const = false;
    let mut is_vol = false;

   	if read_string( (*ast.get_children()[0]).contents ) == "const"
   	{
       	is_const = true;
       	index += 1;
   	}
   	if read_string( (*ast.get_children()[0]).contents ) == "volatile"
   	{
       	is_vol = true;
       	index += 1;
   	}

	let mut s: symbol_type = symbol_type::new();
	s.name = (*ast.get_children()[index]).contents;
	s.constant = is_const;
	s.volatile = is_vol; 
	
	for i in 0..ast.children_num as usize
	{
	    if read_string( (*ast.get_children()[i]).tag ) == "ptrop"
	    || read_string( (*ast.get_children()[i]).tag ) == "index"
	    {
	        s.indirection += 1;
	    }
	}
	s
}

pub unsafe fn print_module()
{
	let module = &(*this_module);
	let mut imports = &mut (*module.imports);
	let mut funcs = &mut (*module.functions);
	let mut types = &mut (*module.types);

	println!("{} {}:", 
		GREEN.to_string() + "module" + RESET, 
		read_string((*this_module).name)
	);
	
	for i in 0..imports.total()
	{
		print!("  {}imports module{} ", YELLOW, RESET);
		print!("{}'{}'{}", 
			RED, 
			read_string( 
			(*(imports.get(i) as *mut import)).name
			), 
			RESET
		);
		if (*(imports.get(i) as *mut import)).local
		{
			print!("{}{}{}",
				MAGENTA,
				" locally",
				RESET
			);
		}
		if (*(imports.get(i) as *mut import)).w_alias
		{
			print!(" {}with alias{} ", CYAN, RESET);
			print!("{}'{}'{}",
				YELLOW, 
				read_string( 
					(*(imports.get(i) as *mut import)).alias
				), 
				RESET
			);
		}
		println!();
	}

	println!("\n  {}functions:{}", MAGENTA, RESET);
	for i in 0..funcs.total()
	{
		print_func(funcs.get(i) as *mut function);
	}

	println!("\n  {}types:{}", MAGENTA, RESET);
	for i in 0..types.total()
	{
		print_usertype(types.get(i) as *mut user_type);
	}
}

pub unsafe fn print_func(func_ptr: *mut function)
{
	let func = &(*func_ptr);
	let mut params = &mut (*func.params);
	if func.public { print!("    {}public {}function{} ", GREEN, CYAN, RESET); }
	else { print!("    {}function{} ", CYAN, RESET); }

	print!("{}{}{}", YELLOW, read_string(func.name), RESET);
	print!(" {}returning{} ", MAGENTA, RESET);
	print_type(&func.ret_type);

	println!();
	for i in 0..params.total()
	{
		let p = &(*(params.get(i) as *mut param));
		print!("      parameter{} {}{}: ",
			MAGENTA,
			read_string(p.name),
			RESET
		);
		print_type(&p.p_type);
		println!();
	}
}

pub unsafe fn print_usertype(type_ptr: *mut user_type)
{
	let t = &(*type_ptr);

	if t.public { print!("    {}public{} ", GREEN, CYAN); }
		   else { print!("    "); }

	match t.t_type
	{
		type_kind::ALIAS => print!("{}{}{}", YELLOW, "alias type", RESET),
		type_kind::STRUCT => print!("{}{}{}", YELLOW, "structure", RESET),
		type_kind::UNION => print!("{}{}{}", YELLOW, "union", RESET),
		type_kind::ENUM => print!("{}{}{}", YELLOW, "enumeration", RESET),
		type_kind::FUNC => print!("{}{}{}", YELLOW, "function type", RESET),
		type_kind::TEMP => {}	
	}

	println!(" {}{}{}", CYAN, read_string(t.name), RESET);
}

pub unsafe fn print_type(s_type: &symbol_type)
{
	if s_type.constant { print!("constant "); }
	if s_type.volatile { print!("volatile "); }

	print!("{}{}{} ",
		RED,
		read_string(s_type.name),
		RESET
	);

	print!("{}[i:{}]{}",
		GREEN,
		s_type.indirection,
		RESET
	);
}
