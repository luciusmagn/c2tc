//#![crate_type = "staticlib"]
#![feature(start)]

#[macro_use]
pub mod macros;

pub mod util;
pub mod tests;
pub mod types;
pub mod analyse;
pub mod errors;

extern
{
    fn c2tc_main(argc: isize, argv: *const *const u8) -> isize;
}

#[start]
pub fn start(_argc: isize, _argv: *const *const u8) -> isize
{
    unsafe { c2tc_main(_argc, _argv) }
}

#[no_mangle]
pub fn help()
{
    println!("OPTIONS:                                        \
           \n--help -h                         print this text\
           \n--dir -d                              chdir first\
           \n--usage -?              print simple usage string\
           \n--file -f                     switch to file-mode\
           \n--ast0 -a                           print raw AST\
           \n--ast1 -A                         print clean AST\
           \n--output -o name     change file-mode target name\
           \n--info -i                       print information");
}
