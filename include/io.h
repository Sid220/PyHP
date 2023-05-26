//
// Created by sid on 5/26/23.
// Automatically import correct IO header based on whether running as Fast CGI or not
//

#ifdef FAST_CGI

#include <fcgi_stdio.h>

#else

#include <stdio.h>

#endif