#if !defined __my_quirks_header_defined
#define __my_quirks_header_defined

// gcc-11 doesn't seem to like the template parameter, and wants it implicit.
#if defined __GNUC__ && (__GNUC__ >= 11)
#define INLINE_TEMPLATE_PARAMS_NOT_ALLOWED
#endif

#endif
