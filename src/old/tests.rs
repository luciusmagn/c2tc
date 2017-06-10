use std::os::raw::c_char;

extern
{
	pub fn puts(s: *const c_char); //laziness ftw
	/*pub fn teststrings();
	pub fn testvector();*/
	static finalmessage: *const c_char;
	static tests_run: i32;
	static tests_passed: i32;
}

#[no_mangle]
pub fn runtests()
{
	unsafe
	{
		/*teststrings();
		testvector();*/
		tiny_verdict();
	}
}


#[no_mangle]
pub fn tiny_verdict()
{
	unsafe
	{
		println!("\x1b[36mmicrotest framework - results [Tests passed: {}/{}]\n\x1b[36m",
			tests_passed,
			tests_run
		);
		println!("==============================================");
		puts(finalmessage);
	}
}
