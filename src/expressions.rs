#![allow(dead_code)]
use types::SymbolType;

pub trait Eval
{
	fn eval(&self) -> EvalRes;
	fn print(&self, level: usize);
	fn contents(&self) -> String;
}

pub struct EvalRes
{
	ret_type: SymbolType,
	content: String,
}

pub struct Expression
{
	pub e_type: SymbolType,
	pub content: Box<Eval>,
}

impl Eval for Expression
{
	fn eval(&self) -> EvalRes
	{
		self.content.eval()
	}

	fn print(&self, level: usize)
	{
		for _ in 0..level { print!("    "); }
		println!("expression:");
		self.content.print(level+1);
	}

	fn contents(&self) -> String
	{
		self.content.contents()
	}
}

pub struct ParamExp
{
	pub params: Vec<Box<Eval>>
}

//params
impl Eval for ParamExp
{
	fn eval(&self) -> EvalRes
	{
		for (index, cnd) in self.params.iter().enumerate()
		{
			if index == &self.params.len() - 1
			{
				return cnd.eval();
			}
			else{ let _ = cnd.eval(); }
		}
		unreachable!()
	}

	fn print(&self, level: usize)
	{
		for _ in 0..level { print!("    "); }

		println!("parameter list:\n");

		for param in &self.params
		{
			param.print(level+1);
		}
	}

	fn contents(&self) -> String
	{
		let mut temp = String::new();
		for param in &self.params
		{
			temp.push_str(&param.contents());
			temp.push(' ');
		}
		temp
	}
}

//elexp
pub struct CondExpr
{
	pub cond: Box<Eval>,
	pub content: Box<Eval>,
}

impl Eval for CondExpr
{
	fn eval(&self) -> EvalRes
	{
		self.cond.eval();
		self.content.eval()
	}

	fn print(&self, level: usize)
	{
		for _ in 0..level { print!("    "); }
		println!("condition: "); self.cond.print(level+1);

		for _ in 0..level { print!("    "); }
		println!("content:"); self.content.print(level+1);
	}

	fn contents(&self) -> String
	{
		let mut temp = String::new();
		temp.push_str(&self.cond.contents());
		temp.push_str(" ? ");
		temp.push_str(&self.content.contents());
		temp
	}
}

//asexp
pub struct AssignExpr
{
	pub left: Box<Eval>,
	pub op: String,
	pub right: Box<Eval>,
}

impl Eval for AssignExpr
{
	fn eval(&self) -> EvalRes
	{
		self.left.eval();
		self.right.eval()
	}

	fn print(&self, level: usize)
	{
		for _ in 0..level { print!("    "); }
		println!("assign: ");
		self.left.print(level+1);
		for _ in 0..level+1 { print!("    "); }
		println!("{}", &self.op);
		self.right.print(level+1);
	}

	fn contents(&self) -> String
	{
		let mut temp = String::new();
		temp.push_str(&self.left.contents());
		temp.push_str(&format!(" {} ", &self.op));
		temp.push_str(&self.right.contents());
		temp
	}
}
