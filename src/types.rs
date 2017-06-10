use std::os::raw::c_char;
use expressions::*;

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
	pub children: *mut *mut mpc_ast_t,
}

pub struct Module
{
	pub name: String,
	pub imports: Vec<Import>,
	pub functions: Vec<Function>,
	pub types: Vec<UserType>,
}

pub struct Import
{
	pub name: String,
	pub alias: String,
	pub local: bool,
	pub w_alias: bool,
}

pub struct Function
{
	pub name: String,
	pub public : bool,
	pub param_count : i32,
	pub params: Vec<Param>,
	pub ret_type: SymbolType,
}

pub struct Param
{
	pub p_type: SymbolType,
	pub name: String,
}

pub struct SymbolType
{
	pub name: String,
	pub indirection: i32,
	pub constant: bool,
	pub volatile: bool,
}

#[derive(Clone, Copy)]
pub enum Natives
{
	Bool,
	Char,
	UInt8,
	UInt16,
	UInt32,
	UInt64,
	Int8,
	Int16,
	Int32,
	Int64,
	Float32,
	Float64,
}

impl SymbolType
{
	pub fn native(string: &str) -> Option<Natives>
	{
		match string
		{
			"bool" => Some(Natives::Bool),
			"char" => Some(Natives::Char),
			"uint8" => Some(Natives::UInt8),
			"uint16" => Some(Natives::UInt16),
			"uint32" => Some(Natives::UInt32),
			"uint64" => Some(Natives::UInt64),
			"int8" => Some(Natives::Int8),
			"int16" => Some(Natives::Int16),
			"int32" => Some(Natives::Int32),
			"int64" => Some(Natives::Int64),
			"float32" => Some(Natives::Float32),
			"float64" => Some(Natives::Float64),
			_ => None
		}
	}

	pub fn compare(&self, other: &SymbolType) -> bool
	{
		unimplemented!()
	}
}

impl Natives
{
	pub fn is_unsigned(&self) -> bool
	{
		match *self
		{
			Natives::Bool => true,
			Natives::Char => true,
			Natives::UInt8 => true,
			Natives::UInt16 => true,
			Natives::UInt32 => true,
			Natives::UInt64 => true,
			_ => false,
		}
	}

	pub fn is_float(&self) -> bool
	{
		match *self
		{
			Natives::Float64 => true,
			Natives::Float32 => true,
			_ => false
		}
	}

	/// True if type1 is bigger than type2
	/// False if not or if types aren't really comparable
	pub fn bigger(type1: Natives, type2: Natives) -> bool
	{
		unimplemented!()
	}
}

pub struct UserType
{
	pub name: String,
	pub t_type: TypeKind,
	pub public: bool,
}

pub enum TypeKind
{
	ALIAS,
	STRUCT,
	UNION,
	ENUM,
	FUNC,
	TEMP
}

/*
** Globals
*/
pub struct Global
{
	pub name: String,
	pub g_type: SymbolType,
	pub content: Option<Expression>,
}
