#include "object.h"


Object * object_load(const char * file)
{
	
	Object * object = (Object *) malloc(sizeof(Object));

	
	log_write ("[OBJCT] Loading object from %s\n", file);

	FILE * fp;

	fp = fopen ( file , "r" );
	if( !fp )
	{
		log_error ("[OBJCT] Could not open %s for reading\n", file);
		return 0;
	}
	
	fscanf(fp, "method %d\n", &(object->method));
	//printf("method %d\n", (object->method));
	int npoints;
	fscanf(fp, "points %d\n", &npoints);
	//printf("points %d\n", npoints);
	float * points = (float *) malloc (npoints * 3 * sizeof(float));
	int i;
	for(i=0;i<(npoints*3);i=i+3)
	{
		fscanf(fp, "%f %f %f\n", &(points[i]), &(points[i+1]), &(points[i+2]));
		//printf("%f %f %f\n", (points[i]), (points[i+1]), (points[i+2]));
	}
	log_write ("[OBJCT] Read %d vertices\n", npoints);
	
	
	int color_format;
	fscanf(fp, "colors %d\n", &color_format);
	//printf("colors %d\n", color_format);
	float * colors = (float *) malloc (npoints * 3 * sizeof(float));
	for(i=0;i<(npoints*3);i=i+3)
	{
		fscanf(fp, "%f %f %f\n", &(colors[i]), &(colors[i+1]), &(colors[i+2]));
		//printf("%f %f %f\n", (colors[i]), (colors[i+1]), (colors[i+2]));
	}
	log_write ("[OBJCT] Read %d vertex colors\n", npoints);

	
	fscanf(fp, "faces %d\n", &(object->nb_faces));
	// printf("faces %d\n", object->nb_faces);
	object->indices = (GLubyte *) malloc (4 * object->nb_faces * sizeof(GLubyte));
	int * t_indices = (int *) malloc (4 * object->nb_faces * sizeof(int));
	// printf("file %s\n", file);
	if (object->method == M_QUADS)
	{
		for(i=0;i<((object->nb_faces)*4);i=i+4)
		{
			fscanf(fp, "%d %d %d %d\n",&(t_indices[i]),&(t_indices[i+1]),&(t_indices[i+2]),&(t_indices[i+3]));
			object->indices[i] = (GLubyte) t_indices[i];
			object->indices[i+1] = (GLubyte) t_indices[i+1];
			object->indices[i+2] = (GLubyte) t_indices[i+2];
			object->indices[i+3] = (GLubyte) t_indices[i+3];
			 // printf("%d %d %d %d\n",(object->indices[i]),
				 // (object->indices[i+1]), (object->indices[i+2]),
				 // (object->indices[i+3]));
		}
	}
	log_write ("[OBJCT] Read %d indices\n", object->nb_faces);
	
	free(t_indices);

	fclose(fp);

	log_write ("[OBJCT] Object loaded from %s\n", file);

	GLuint points_vbo = 0;
	glGenBuffers (1, &points_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glBufferData (GL_ARRAY_BUFFER, (3*npoints) * sizeof (float), points, GL_STATIC_DRAW);
	log_write ("[OBJCT] VBO %d created for vertices\n", points_vbo);

	GLuint colors_vbo = 0;
	glGenBuffers (1, &colors_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, colors_vbo);
	glBufferData (GL_ARRAY_BUFFER, (3*npoints) * sizeof (float), colors, GL_STATIC_DRAW);	
	log_write ("[OBJCT] VBO %d created for colors\n", colors_vbo);
	
	GLuint vao = 0;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer (GL_ARRAY_BUFFER, colors_vbo);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (0);
	glEnableVertexAttribArray (1);
	
	glBindVertexArray (vao);
	object->vao_id = vao;

	log_write ("[OBJCT] VAO %d created bound to VBO %d and VBO %d\n", vao, points_vbo, colors_vbo);
	return object;
}

int object_destroy(Object * object)
{
	return 0;
}
