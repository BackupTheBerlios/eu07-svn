
#ifndef PREVIEWER_EXPORT_
#define PREVIEWER_EXPORT_ 1

#if defined(_MSC_VER)
    #pragma warning( disable : 4244 )
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4267 )
    #pragma warning( disable : 4275 )
    #pragma warning( disable : 4290 )
    #pragma warning( disable : 4786 )
    #pragma warning( disable : 4305 )
#endif

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__) || defined( __MWERKS__)
	#  ifdef PREVIEWER_LIBRARY
	#    define PREVIEWER_EXPORT   __declspec(dllexport)
	#  else
	#    define PREVIEWER_EXPORT   __declspec(dllimport)
	#endif /* PREVIEWER_LIBRARY */
#else
	#define PREVIEWER_EXPORT 
#endif 

#endif

