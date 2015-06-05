/**
 * Projet application - 3DSolve
 * Version : 1.0
 *
 * Programme pour la résolution et la manipulation virtuelle de
 * casse-tête de type "Snake Cube"
 *
 * Auteurs :
 * 	- L.Aubry <lisa.aubry@insa-cvl.fr>
 * 	- A.Chazot <alban.chazot@insa-cvl.fr>
 * 	- K.Colas <korlan.colas@insa-cvl.fr>
 * 	- A.Gourd <anthony.gourd@insa-cvl.fr>
 *
 * Tuteur :
 * 	- P.Clemente <patrice.clemente@insa-cvl.fr>
 *
 * Date : 11 / 06 / 15
 *
 * INSA - Centre Val De Loire
 * 2014-2015
 * Promotion 2017
 *
 */

	#ifndef FONTS_H
	#define FONTS_H

  /* Define to the path to a TrueType font */
  #define FONT_FILE "fonts/LinLibertine_aBL.ttf"

  /* Define to 1 if you have the <dlfcn.h> header file. */
  #define HAVE_DLFCN_H 1

  /* Define to 1 if you have the <GLUT/glut.h> header file. */
  /* #undef HAVE_GLUT_GLUT_H */

  /* Define to 1 if you have the <GL/glut.h> header file. */
  #define HAVE_GL_GLUT_H 1

  /* Define to 1 if you have the <GL/glu.h header */
  #define HAVE_GL_GLU_H 1

  /* Define to 1 if you have the <GL/gl.h header */
  #define HAVE_GL_GL_H 1

  /* Define to 1 if you have the <inttypes.h> header file. */
  #define HAVE_INTTYPES_H 1

  /* Define to 1 if you have the <memory.h> header file. */
  #define HAVE_MEMORY_H 1

  /* Define to 1 if you have the <OpenGL/glu.h header */
  /* #undef HAVE_OPENGL_GLU_H */

  /* Define to 1 if you have the <OpenGL/gl.h header */
  /* #undef HAVE_OPENGL_GL_H */

  /* Define to 1 if you have the <stdint.h> header file. */
  #define HAVE_STDINT_H 1

  /* Define to 1 if you have the <stdlib.h> header file. */
  #define HAVE_STDLIB_H 1

  /* Define to 1 if you have the <strings.h> header file. */
  #define HAVE_STRINGS_H 1

  /* Define to 1 if you have the <string.h> header file. */
  #define HAVE_STRING_H 1

  /* Define to 1 if you have the `strndup' function. */
  #define HAVE_STRNDUP 1

  /* Define to 1 if you have the <sys/stat.h> header file. */
  #define HAVE_SYS_STAT_H 1

  /* Define to 1 if you have the <sys/types.h> header file. */
  #define HAVE_SYS_TYPES_H 1

  /* Define to 1 if you have the <unistd.h> header file. */
  #define HAVE_UNISTD_H 1

  /* Define to 1 if you have the `wcsdup' function. */
  #define HAVE_WCSDUP 1

  /* Define to 1 if your C compiler doesn't accept -c and -o together. */
  /* #undef NO_MINUS_C_MINUS_O */

  /* Define to the address where bug reports for this package should be sent. */
  #define PACKAGE_BUGREPORT "sam@zoy.org"

  /* Define to the full name of this package. */
  #define PACKAGE_NAME "FTGL"

  /* Define to the full name and version of this package. */
  #define PACKAGE_STRING "FTGL 2.1.3~rc5"

  /* Define to the one symbol short name of this package. */
  #define PACKAGE_TARNAME "ftgl"

  /* Define to the version of this package. */
  #define PACKAGE_VERSION "2.1.3~rc5"

  /* Define to 1 if you have the ANSI C header files. */
  #define STDC_HEADERS 1

  /* Define to 1 if the X Window System is missing or not being used. */
  /* #undef X_DISPLAY_MISSING */

  #include <FTGL/ftgl.h>

  FTGLfont *LoadFont(char *file);

#endif
