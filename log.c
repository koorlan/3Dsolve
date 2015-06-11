/*
  Projet application - 3DSolve
  Version : 1.0

  Programme pour la résolution et la manipulation virtuelle de
  casse-tête de type "Snake Cube"

  Auteurs :
  	- L.Aubry <lisa.aubry@insa-cvl.fr>
  	- A.Chazot <alban.chazot@insa-cvl.fr>
  	- K.Colas <korlan.colas@insa-cvl.fr>
  	- A.Gourd <anthony.gourd@insa-cvl.fr>

  Tuteur :
  	- P.Clemente <patrice.clemente@insa-cvl.fr>

  Date : 11 / 06 / 15

  INSA - Centre Val De Loire
  2014-2015
  Promotion 2017

 */

#include "log.h"

int logStart ()
{
	FILE* file = fopen (LOG_FILE, "w");
	if (!file)
	{
		fprintf (stderr, "[ERROR] could not open LOG_FILE log file %s for writing\n", LOG_FILE);
		return 0;
	}
	time_t now = time (NULL);
	char* date = ctime (&now);
	fprintf (file, "[ ][START] Starting log at local time %s", date);
	fclose (file);
	return 1;
}

int logWrite (const char* message, ...)
{
	va_list argptr;
	FILE* file = fopen (LOG_FILE, "a");
	if (!file)
	{
		fprintf (stderr,"[ERROR] could not open LOG_FILE %s file for appending\n",LOG_FILE);
		return 0;
	}
	va_start (argptr, message);
	fprintf (file, "[-]");
	vfprintf (file, message, argptr);
	va_end (argptr);
	fclose (file);
	return 1;
}

int logError (const char* message, ...)
{
	va_list argptr;
	FILE* file = fopen (LOG_FILE, "a");
	if (!file)
	{
		fprintf (stderr,"[ERROR] could not open LOG_FILE %s file for appending\n", LOG_FILE);
		return 0;
	}
	va_start (argptr, message);
	fprintf (file, "[!]");
	vfprintf (file, message, argptr);
	va_end (argptr);
	va_start (argptr, message);
	fprintf (stderr, "[!]");
	vfprintf (stderr, message, argptr);
	va_end (argptr);
	fclose (file);
	return 1;
}

int logWarning (const char* message, ...)
{
	va_list argptr;
	FILE* file = fopen (LOG_FILE, "a");
	if (!file)
	{
		fprintf (stderr,"[ERROR] could not open LOG_FILE %s file for appending\n", LOG_FILE);
		return 0;
	}
	va_start (argptr, message);
	fprintf (file, "[?]");
	vfprintf (file, message, argptr);
	va_end (argptr);
	va_start (argptr, message);
	fprintf (stderr, "[?]");
	vfprintf (stderr, message, argptr);
	va_end (argptr);
	fclose (file);
	return 1;
}

void logGLParams ()
{

	GLenum params[] = {
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_VIEWPORT_DIMS,
		GL_STEREO,
	};
	const char* names[] = {
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO",
	};
	logWrite ("[GLNFO] GL context parameters:\n");
	int i;
	for (i = 0; i < 10; i++) {
		int v = 0;
		glGetIntegerv (params[i], &v);
		logWrite ("[GLNFO] %s %i\n", names[i], v);
	}
	int v[2];
	v[0] = v[1] = 0;
	glGetIntegerv (params[10], v);
	logWrite ("[GLNFO] %s %i %i\n", names[10], v[0], v[1]);
	unsigned char s = 0;
	glGetBooleanv (params[11], &s);
	logWrite ("[GLNFO] %s %u\n", names[11], (unsigned int)s);
	logWrite ("[GLNFO] End of GL context parameters\n");

}
