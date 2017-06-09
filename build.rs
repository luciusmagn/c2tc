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
		.flag("-std=gnu99")
		.flag("-Wno-discarded-qualifiers")
		.compile("libc2tc.a");
}
