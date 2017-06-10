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

impl EvalRes
{
	pub fn new() -> EvalRes
	{
		EvalRes
		{
			ret_type: SymbolType::new(),
			content: String::new(),
		}
	}
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

//lexp
pub struct BoolExpr
{
	pub left: Box<Eval>,
	pub op: String,
	pub right: Box<Eval>,
}

impl Eval for BoolExpr
{
	fn eval(&self) -> EvalRes
	{
		self.right.eval();
		self.left.eval();
		let mut res = EvalRes::new();
		res.content.push_str(&self.left.contents());
		res.content.push_str(&format!(" {} ", &self.op));
		res.content.push_str(&self.right.contents());
		res.ret_type = SymbolType
		{
			name: "bool".to_string(),
			indirection: 0,
			volatile: false,
			constant: false
		};
		res
	}

	fn print(&self, level: usize)
	{
		for _ in 0..level { print!("    "); }
		println!("boolean: ");
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

//eexp
pub struct EqExpr
{
	pub left: Box<Eval>,
	pub op: String,
	pub right: Box<Eval>,
}

impl Eval for EqExpr
{
	fn eval(&self) -> EvalRes
	{
		let lres = self.right.eval();
		let rres = self.left.eval();
		let mut res = EvalRes::new();

		res.content.push_str(&self.left.contents());
		res.content.push_str(&format!(" {} ", &self.op));
		res.content.push_str(&self.right.contents());

		if lres.ret_type.compare(&rres.ret_type)
		{
			res
		}
		else
		{
			panic!()
		}
	}

	fn print(&self, level: usize)
	{
		for _ in 0..level { print!("    "); }
		println!("equality: ");
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

//bexp
pub struct BitwiseExpr
{
	pub left: Box<Eval>,
	pub op: String,
	pub right: Box<Eval>,
}


