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
