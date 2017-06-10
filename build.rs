extern crate gcc;

fn main()
{
	gcc::Config::new()
		.file("src/c/mpc.c")
		.file("src/c/main.c")
		.file("src/c/parsers.c")
		.file("src/c/recipe.c")
		.file("src/c/tree_transform.c")
		.file("src/c/util.c")
		.flag("-std=gnu99")
		.flag("-Wno-discarded-qualifiers")
		.compile("libc2tc.a");
}
