#include "shader.h"



GLuint shader_load (char * file, GLenum type)
{
	log_write ("[SHADR] Loading shader from %s\n", file);

	FILE * fp;
	long lSize;
	char * buffer;
	
	fp = fopen ( file , "r" );
	if( !fp )
	{
		log_error ("[SHADR] Could not open %s for reading\n", file);
		return 0;
	}

	fseek( fp , 0 , SEEK_END);
	lSize = ftell( fp );
	fseek(fp, 0, SEEK_SET);

	buffer = (char*)malloc( lSize );
	if( !buffer )
	{
		log_error ("[SHADR] Could not allocate memory for shader in %s\n", file);
		fclose(fp);
		return 0;
	}

	int res = fread( buffer , 1, lSize, fp);
	if( res!=lSize )
	{
		log_warning ("[SHADR] Read size conflict in %s (using:%d vs %d)\n", file, res, lSize);
		lSize = res;
		//fclose(fp);
		//return 0;
	}
	sprintf(buffer+(lSize), "%c", '\0');

	//log_write ("[SHADR] Shader dump:\n%s\n", buffer);
	
	fclose(fp);

	GLuint new_shader = glCreateShader (type);
	glShaderSource (new_shader, 1, (const GLchar * const *)&buffer, NULL);
	free(buffer);

	log_write ("[SHADR] Shader id:%d type:%d loaded from %s\n", new_shader, type, file);

	return new_shader;
}

int shader_compile (GLuint shader_id)
{
	glCompileShader (shader_id);
	int params = -1;
	glGetShaderiv (shader_id, GL_COMPILE_STATUS, &params);
	if (params == GL_FALSE)
	{
		log_error ("[SHADR] GL shader id %i did not compile\n", shader_id);
		shader_print_info_log (shader_id);
		glDeleteShader(shader_id);
		return 0;
	}
	log_write ("[SHADR] Shader %d compiled (%d)\n", shader_id, params);
	return 1;
}

void shader_print_info_log (GLuint shader_index)
{
	int max_length = 65535;
	int actual_length = 0;
	char log[65535];
	glGetShaderInfoLog (shader_index, max_length, &actual_length, log);
	log_write ("[SHADR] Shader info log for GL index %u:\n%s\n", shader_index, log);
}

GLuint shader_create_program(GLuint vertex_shader_id, GLuint fragment_shader_id)
{

	GLuint shader_program = glCreateProgram ();
	glAttachShader (shader_program, vertex_shader_id);
	glAttachShader (shader_program, fragment_shader_id);
	glLinkProgram (shader_program);
	int params = -1;
	glGetProgramiv (shader_program, GL_LINK_STATUS, &params);
	if (params == GL_FALSE) 
	{
		log_error("[SHADR] could not link shader program id %u\n", shader_program);
		shader_print_program_info_log (shader_program);
		return -1;
	}
	log_write ("[SHADR] Shader program id %d linked with vs:%d fs:%d (%d)\n", shader_program, vertex_shader_id, fragment_shader_id, params);
	return shader_program;
}

void shader_print_program_info_log (GLuint shader_program)
{
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetProgramInfoLog (shader_program, max_length, &actual_length, log);
	log_write ("[SHADR] program info log for GL index %u:\n%s\n", shader_program, log);
}

int shader_destroy(GLuint shader_id)
{
    glDeleteShader(shader_id);
	return 1;
}
int shader_program_destroy(GLuint shader_program_id, GLuint vs_id, GLuint fs_id)
{
    glDetachShader(shader_program_id, fs_id);
    glDetachShader(shader_program_id, vs_id);
	glDeleteProgram(shader_program_id);
	return 1;
}
