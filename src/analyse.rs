use util::*;
use types::*;
use ast::Ast;

static mut THIS_MODULE: *mut Module = 0 as *mut Module;

#[no_mangle]
pub fn analyse(ast_ptr: *mut mpc_ast_t)
{
	unsafe { THIS_MODULE = &mut Module::new() as *mut Module; }

	process(&Ast::new(ast_ptr), 0);
	print_module();
}

pub fn process(ast: &Ast, level: usize)
{
	for child in &ast.children
	{
		match child.tag.as_ref()
		{
			"func" => read_func(child),
			"module" => unsafe { (*THIS_MODULE).name = (&child.children[0]).contents.clone() },
			"import" => read_import(child),
			"usertype" => read_usertype(child),
			"statement" => println!("statement"),
			_ => {},
		}
		process(child, level + 1);
	}
}

pub fn read_import(ast: &Ast)
{
	let mut imp = Import::new();
	imp.name = (&ast.children[0]).contents.clone();

	for child in &ast.children
	{
		match child.contents.as_ref()
		{
			"local" => imp.local = true,
			_ =>
			{
				imp.w_alias = true;
				imp.alias = child.contents.clone();
			},
		}
	}

	unsafe { (*THIS_MODULE).imports.push(imp); }
}

pub fn read_usertype(ast: &Ast)
{
	let mut t = UserType::new();
	let index = if ast.children[0].tag == "public"
	{
		t.public = true;
		1
	} else {0};

	let child = &ast.children[index];

	t.name = (&child.children[0]).contents.clone();

	match child.tag.as_ref()
	{
		"globalunion" => t.t_type = TypeKind::UNION,
		"structure" => t.t_type = TypeKind::STRUCT,
		"enumtype" => t.t_type = TypeKind::ENUM,
		"functype" => t.t_type = TypeKind::FUNC,
		"alias" => t.t_type = TypeKind::ALIAS,
		_ => {}
	}

	unsafe { (*THIS_MODULE).types.push(t); }
}

pub fn read_func(ast: &Ast)
{
	let mut func = Function::new();
	let base_index = if ast.children[0].tag == "public"
	{
		func.public = true;
		1
	} else {0};

	func.name = (&ast.children[base_index + 2]).contents.clone();
	let child =  &ast.children[base_index + 1];

	func.ret_type = read_type(child);

	func.params = read_params(&ast.children[base_index + 3]);

	unsafe { (*THIS_MODULE).functions.push(func); }
}

pub fn read_params(ast: &Ast) -> Vec<Param>
{
	let mut params = Vec::new();
	for child in &ast.children
	{
		let mut p = Param::new();
		p.p_type = read_type(&child.children[0]);
		p.name = (&child.children[1]).contents.clone();

		params.push(p);
	}

	params
}

pub fn read_type(ast: &Ast) -> SymbolType
{
	if ast.tag == "natives" || ast.children.is_empty()
	{
		let mut s = SymbolType::new();
		s.name = ast.contents.clone();
		return s;
	}

	let mut index = 0;

	let is_const = if ast.children[0].contents == "const"
	{
		index += 1;
		true
	} else { false };

	let is_vol = if ast.children[0].contents == "volatile"
	{
		index += 1;
		true
	} else { false };

	let mut s = SymbolType::new();
	s.name = (&ast.children[index]).contents.clone();
	s.constant = is_const;
	s.volatile = is_vol;

	for child in &ast.children
	{
		if child.tag == "ptrop"
		|| child.tag == "index"
		{
			s.indirection += 1;
		}
	}
	s
}

pub fn print_module()
{
	let module = unsafe { &(*THIS_MODULE) };

	println!("{} {}:",
		GREEN.to_string() + "module" + RESET,
		unsafe { &(*THIS_MODULE).name }
	);

	for import in &module.imports
	{
		print!("  {}imports module{} ", YELLOW, RESET);
		print!("{}'{}'{}",
			RED,
			import.name,
			RESET
		);
		if import.local
		{
			print!("{}{}{}",
				MAGENTA,
				" locally",
				RESET
			);
		}
		if import.w_alias
		{
			print!(" {}with alias{} ", CYAN, RESET);
			print!("{}'{}'{}",
				YELLOW,
				import.alias,
				RESET
			);
		}
		println!();
	}

	println!("\n  {}functions:{}", MAGENTA, RESET);
	for func in &module.functions
	{
		print_func(func);
	}

	println!("\n  {}types:{}", MAGENTA, RESET);
	for utype in &module.types
	{
		print_usertype(utype);
	}
}

pub fn print_func(func: &Function)
{
	if func.public { print!("    {}public {}function{} ", GREEN, CYAN, RESET); }
	else { print!("    {}function{} ", CYAN, RESET); }

	print!("{}{}{}", YELLOW, func.name, RESET);
	print!(" {}returning{} ", MAGENTA, RESET);
	print_type(&func.ret_type);

	println!();
	for param in &func.params
	{
		print!("      parameter{} {}{}: ",
			MAGENTA,
			param.name,
			RESET
		);
		print_type(&param.p_type);
		println!();
	}
}

pub fn print_usertype(utype: &UserType)
{
	if utype.public { print!("    {}public{} ", GREEN, CYAN); }
			   else { print!("    "); }

	match utype.t_type
	{
		TypeKind::ALIAS => print!("{}{}{}", YELLOW, "alias type", RESET),
		TypeKind::STRUCT => print!("{}{}{}", YELLOW, "structure", RESET),
		TypeKind::UNION => print!("{}{}{}", YELLOW, "union", RESET),
		TypeKind::ENUM => print!("{}{}{}", YELLOW, "enumeration", RESET),
		TypeKind::FUNC => print!("{}{}{}", YELLOW, "function type", RESET),
		TypeKind::TEMP => {}
	}

	println!(" {}{}{}", CYAN, utype.name, RESET);
}

pub fn print_type(s_type: &SymbolType)
{
	if s_type.constant { print!("constant "); }
	if s_type.volatile { print!("volatile "); }

	print!("{}{}{} ",
		RED,
		s_type.name,
		RESET
	);

	print!("{}[i:{}]{}",
		GREEN,
		s_type.indirection,
		RESET
	);
}
