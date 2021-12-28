#ifndef HAVE_COROUNITE_H
#define HAVE_COROUNITE_H

#ifdef COROUTINE_WITH_LONG_JUMP

#include <setjmp.h>

#define CO_BEGIN                                          \
	static jmp_buf _coroutine_buffer;                     \
	static char _coroutine_iter = 0;                      \
	do { 		    		                              \
		if (_coroutine_iter != 0) {                       \
			longjmp(_coroutine_buffer, _coroutine_iter);  \
		}                                                 \
	} while(0)

#define CO_END(z)           \
	do { 		            \
		_coroutine_iter=0;  \
		return (z);         \
	} while(0)


#define CO_ENDV             \
	do { 		            \
		_coroutine_iter=0;  \
		return;             \
	} while(0)


#define CO_RETURN(value)                       \
	do { 		    		                   \
		_coroutine_iter++;                     \
		if (setjmp(_coroutine_buffer) == 0) {  \
			return (value);                    \
		}  	    		                       \
	} while(0)

#define CO_RETURNV                             \
	do { 		    		                   \
		_coroutine_iter++;                     \
		if (setjmp(_coroutine_buffer) == 0) {  \
			return        ;                    \
		}  	    		                       \
	} while(0)

#else // COROUTINE_WITH_LONG_JUMP

// version from https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html

#define CO_BEGIN      static int scrLine = 0; switch(scrLine) { case 0:;
#define CO_END(z)     } scrLine = 0; return (z)
#define CO_ENDV       } scrLine = 0; return

#define CO_RETURN(z)                    \
        do {                            \
            scrLine=__LINE__;           \
            return (z); case __LINE__:; \
        } while (0)

#define CO_RETURNV                  \
        do {                        \
            scrLine=__LINE__;       \
            return; case __LINE__:; \
        } while (0)

#endif // COROUTINE_WITH_LONG_JUMP

#endif // HAVE_COROUNITE_H
