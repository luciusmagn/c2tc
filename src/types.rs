use std::slice;
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

impl mpc_ast_t
{
    pub unsafe fn get_children(&self) -> &'static[*mut mpc_ast_t]
    {
        slice::from_raw_parts(self.children, self.children_num as usize)
    }
}
