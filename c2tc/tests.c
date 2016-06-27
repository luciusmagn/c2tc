#include "shared.h"
#include "microtest.h"
#include "stringutils.h"
#include "vector.h"

/// <summary>
///  This function runs all tests and prints verdict to stdout
/// </summary>
void runtests()
{
	teststrings();
	testvector();
	tiny_verdict();
}