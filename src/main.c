#include <testenv_funcs.h>

// no arguments should be passed to the application so no argc/argv here to see
int main()
{
	int exitState;
	exitState = f_cubes();
	return exitState;
}
