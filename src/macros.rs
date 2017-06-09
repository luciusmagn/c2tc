
#[macro_export]
macro_rules! c_str /* C-ish String */
{
	($str:expr) =>
	{{
		use util::extend_lifetime;
		use std::ffi::CString;

		unsafe { CString::new(extend_lifetime($str)).unwrap().as_ptr() }
	}}
}

#[macro_export]
macro_rules! str_c /* String from C */
{
	($ptr:expr) =>
	{{
		use std::ffi::CStr;
		unsafe { CStr::from_ptr($ptr).to_string_lossy().into_owned() }
	}}
}
