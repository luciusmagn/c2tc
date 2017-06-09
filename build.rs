extern crate gcc;

fn main()
{
	gcc::Config::new()
		.file("src/mpc.c")
		.file("src/main.c")
		.file("src/parsers.c")
		.file("src/recipe.c")
		.file("src/tree_transform.c")
		.file("src/util.c")
//		.file("src/errors.c")
		.flag("-std=gnu99")
		.compile("libc2tc.a");
}
