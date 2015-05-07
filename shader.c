#include "shader.h"



GLuint shaderLoad (char * file, GLenum type)
{
	logWrite ("[SHADR] Loading shader from %s\n", file);

	FILE * fp;
	long lSize;
	char * buffer;

	fp = fopen ( file , "r" );
	if( !fp )
	{
		logError ("[SHADR] Could not open %s for reading\n", file);
		return 0;
	}

	fseek( fp , 0 , SEEK_END);
	lSize = ftell( fp );
	fseek(fp, 0, SEEK_SET);

	buffer = (char*)malloc( lSize +1);
	if( !buffer )
	{
		logError ("[SHADR] Could not allocate memory for shader in %s\n", file);
		fclose(fp);
		return 0;
	}

	int res = fread( buffer , 1, lSize, fp);
	if( res!=lSize )
	{
		logWarning ("[SHADR] Read size conflict in %s (using:%d vs %d)\n", file, res, lSize);
		lSize = res;
		//fclose(fp);
		//return 0;
	}
	sprintf(buffer+(lSize), "%c", '\0');

	//logWrite ("[SHADR] Shader dump:\n%s\n", buffer);

	fclose(fp);

	GLuint new_shader = glCreateShader (type);
	glShaderSource (new_shader, 1, (const GLchar * const *)&buffer, NULL);
	free(buffer);

	logWrite ("[SHADR] Shader id:%d type:%d loaded from %s\n", new_shader, type, file);

	return new_shader;
}

int shaderCompile (GLuint shader_id)
{
	glCompileShader (shader_id);
	int params = -1;
	glGetShaderiv (shader_id, GL_COMPILE_STATUS, &params);
	if (params == GL_FALSE)
	{
		logError ("[SHADR] GL shader id %i did not compile\n", shader_id);
		shaderPrintInfo (shader_id);
		glDeleteShader (shader_id);
		return 0;
	}
	logWrite ("[SHADR] Shader %d compiled (%d)\n", shader_id, params);
	return 1;
}

void shaderPrintInfo (GLuint shader_index)
{
	int max_length = 65535;
	int actual_length = 0;
	char log[65535];
	glGetShaderInfoLog (shader_index, max_length, &actual_length, log);
	logWrite ("[SHADR] Shader info log for GL index %u:\n%s\n", shader_index, log);
}

GLuint shaderCreateProgram (GLuint vertex_shader_id, GLuint fragment_shader_id)
{

	GLuint shader_program = glCreateProgram ();
	glAttachShader (shader_program, vertex_shader_id);
	glAttachShader (shader_program, fragment_shader_id);
	glLinkProgram (shader_program);
	int params = -1;
	glGetProgramiv (shader_program, GL_LINK_STATUS, &params);
	if (params == GL_FALSE)
	{
		logError("[SHADR] could not link shader program id %u\n", shader_program);
		shaderPrintProgramInfo (shader_program);
		return -1;
	}
	logWrite ("[SHADR] Shader program id %d linked with vs:%d fs:%d (%d)\n", shader_program, vertex_shader_id, fragment_shader_id, params);
	return shader_program;
}

void shaderPrintProgramInfo (GLuint shader_program)
{
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetProgramInfoLog (shader_program, max_length, &actual_length, log);
	logWrite ("[SHADR] program info log for GL index %u:\n%s\n", shader_program, log);
}

int shaderDestroy(GLuint shader_id)
{
    glDeleteShader(shader_id);
	return 1;
}
int shaderProgramDestroy(GLuint shader_program_id, GLuint vs_id, GLuint fs_id)
{
    glDetachShader(shader_program_id, fs_id);
    glDetachShader(shader_program_id, vs_id);
	glDeleteProgram(shader_program_id);
	return 1;
}
