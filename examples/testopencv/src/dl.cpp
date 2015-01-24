#include <dlfcn.h>

/* Open the shared object FILE and map it in; return a handle that can be
   passed to `dlsym' to get symbol values from it.  */
void *dlopen (const char *__file, int __mode) {
	return 0;
}

/* Unmap and close a shared object opened by `dlopen'.
   The handle cannot be used again after calling `dlclose'.  */
int dlclose (void *__handle) {
	return 0;
}

/* Find the run-time address in the shared object HANDLE refers to
   of the symbol called NAME.  */
void *dlsym (void *__restrict __handle,
		    const char *__restrict __name) {
	return 0;
}

