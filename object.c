#include "object.h"


Object * objectLoad(const char * file)
{

	Object * object = (Object *) malloc(sizeof(Object));


	logWrite ("[OBJCT] Loading object from %s\n", file);

	FILE * fp;

	fp = fopen ( file , "r" );
	if( !fp )
	{
		logError ("[OBJCT] Could not open %s for reading\n", file);
		return 0;
	}

	fscanf(fp, "method %d\n", &(object->method));
	//printf("method %d\n", (object->method));

	if (object->method == M_QUADS)
	{
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
		logWrite ("[OBJCT] Read %d vertices\n", npoints);


		int color_format;
		fscanf(fp, "colors %d\n", &color_format);
		//printf("colors %d\n", color_format);
		float * colors = (float *) malloc (npoints * 3 * sizeof(float));
		for(i=0;i<(npoints*3);i=i+3)
		{
			fscanf(fp, "%f %f %f\n", &(colors[i]), &(colors[i+1]), &(colors[i+2]));
			//printf("%f %f %f\n", (colors[i]), (colors[i+1]), (colors[i+2]));
		}
		logWrite ("[OBJCT] Read %d vertex colors\n", npoints);


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
		logWrite ("[OBJCT] Read %d indices\n", object->nb_faces);

		free(t_indices);

		fclose(fp);

		logWrite ("[OBJCT] Object loaded from %s\n", file);

		GLuint points_vbo = 0;
		glGenBuffers (1, &points_vbo);
		glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
		glBufferData (GL_ARRAY_BUFFER, (3*npoints) * sizeof (float), points, GL_STATIC_DRAW);
		logWrite ("[OBJCT] VBO %d created for vertices\n", points_vbo);

		GLuint colors_vbo = 0;
		glGenBuffers (1, &colors_vbo);
		glBindBuffer (GL_ARRAY_BUFFER, colors_vbo);
		glBufferData (GL_ARRAY_BUFFER, (3*npoints) * sizeof (float), colors, GL_STATIC_DRAW);
		logWrite ("[OBJCT] VBO %d created for colors\n", colors_vbo);

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

		logWrite ("[OBJCT] VAO %d created bound to VBO %d and VBO %d\n", vao, points_vbo, colors_vbo);
	}
	else if (object->method == M_TRIS)
	{
		int npoints;
		fscanf(fp, "points %d\n", &npoints);
		//printf("points %d\n", npoints);
		float * points = (float *) malloc (npoints * 3 * sizeof(float));
		int i;
		int nfaces = 0;
		for(i=0;i<(npoints*3);i=i+3)
		{
			fscanf(fp, "%f %f %f\n", &(points[i]), &(points[i+1]), &(points[i+2]));
			//printf("%f %f %f\n", (points[i]), (points[i+1]), (points[i+2]));
			nfaces++;
		}
		object->nb_faces = nfaces;
		logWrite ("[OBJCT] Read %d vertices\n", npoints);

		int uv_nb;
		fscanf(fp, "uvs %d\n", &uv_nb);
		//printf("uvs %d\n", uv_nb);
		float * uvs = malloc (uv_nb * 2 * sizeof(float));
		for(i=0;i<(uv_nb*2);i=i+2)
		{
			fscanf(fp, "%f %f\n", &(uvs[i]), &(uvs[i+1]));
			//printf("%f %f %f\n", (uvs[i]), (uvs[i+1]));
		}
		logWrite ("[OBJCT] Read %d uvs\n", npoints);

		fclose(fp);

		logWrite ("[OBJCT] Object loaded from %s\n", file);

		GLuint points_vbo = 0;
		glGenBuffers (1, &points_vbo);
		glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
		glBufferData (GL_ARRAY_BUFFER, (3*npoints) * sizeof (float), points, GL_STATIC_DRAW);
		logWrite ("[OBJCT] VBO %d created for vertices\n", points_vbo);

		GLuint uvs_vbo = 0;
		glGenBuffers (1, &uvs_vbo);
		glBindBuffer (GL_ARRAY_BUFFER, uvs_vbo);
		glBufferData (GL_ARRAY_BUFFER, (2*uv_nb) * sizeof (float), uvs, GL_STATIC_DRAW);
		logWrite ("[OBJCT] VBO %d created for uvs\n", uvs_vbo);

		GLuint vao = 0;
		glGenVertexArrays (1, &vao);
		glBindVertexArray (vao);
		glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer (GL_ARRAY_BUFFER, uvs_vbo);
		glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);
		glEnableVertexAttribArray (1);

		glBindVertexArray (vao);
		object->vao_id = vao;

		logWrite ("[OBJCT] VAO %d created bound to VBO %d and VBO %d\n", vao, points_vbo, uvs_vbo);
	}
	else if (object->method == 50)
	{

		float baseverts[4096];
		float baseuvs[4096];
		//float normals[1024];
		int faces[4096][3];
		int bvcnt = 0;
		int uvcnt = 0;
		int nrcnt = 0;
		int fccnt = 0;
		while ( 1 )
		{
			char lineHeader[128];
			int res = fscanf(fp, "%s", lineHeader);
			if (res == EOF)
				break;
			if ( strcmp( lineHeader, "v" ) == 0 )
			{
				fscanf(fp, "%f %f %f\n", &baseverts[bvcnt], &baseverts[bvcnt+1], &baseverts[bvcnt+2] );
				bvcnt += 3;
			}
			else if ( strcmp( lineHeader, "vt" ) == 0 )
			{
				fscanf(fp, "%f %f\n", &baseuvs[uvcnt], &baseuvs[uvcnt+1] );
				baseuvs[uvcnt+1] = 1 - baseuvs[uvcnt+1];
				uvcnt += 2;
			}
			else if ( strcmp( lineHeader, "vn" ) == 0 )
			{
			//	fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
				nrcnt += 3;
			}
			else if ( strcmp( lineHeader, "f" ) == 0 )
			{
				fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &(faces[fccnt][0]), &(faces[fccnt][1])
					, &(faces[fccnt][2]), &(faces[fccnt+1][0]), &(faces[fccnt+1][1]), &(faces[fccnt+1][2])
					, &(faces[fccnt+2][0]), &(faces[fccnt+2][1]), &(faces[fccnt+2][2]) );
				fccnt += 3;
			}
		}
		fclose(fp);

		/*
		bvcnt -= 3;
		uvcnt -= 2;
		nrcnt -= 3;
		fccnt -= 3;
		*/

		float compverts[4096];
		float compuvs[4096];
		int cvcnt = 0;
		int face, dim;
		for ( face = 0; face <= fccnt; face++ )
		{
			for (dim=0;dim<3;dim++)
				compverts[face*3+dim] = baseverts[(faces[face][0]-1)*3+dim];
			for (dim=0;dim<2;dim++)
				compuvs[face*2+dim] = baseuvs[(faces[face][1]-1)*2+dim];
			cvcnt++;
			printf("%f %f %f\n", compverts[face*3+0],compverts[face*3+1],compverts[face*3+2]);
		}
		printf ("v: %d\n", cvcnt);

		object->nb_faces = cvcnt;

		logWrite ("[OBJCT] Object loaded from %s\n", file);

		GLuint points_vbo = 0;
		glGenBuffers (1, &points_vbo);
		glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
		glBufferData (GL_ARRAY_BUFFER, (3*cvcnt) * sizeof (float), compverts, GL_STATIC_DRAW);
		logWrite ("[OBJCT] VBO %d created for vertices\n", points_vbo);

		GLuint uvs_vbo = 0;
		glGenBuffers (1, &uvs_vbo);
		glBindBuffer (GL_ARRAY_BUFFER, uvs_vbo);
		glBufferData (GL_ARRAY_BUFFER, (2*cvcnt) * sizeof (float), compuvs, GL_STATIC_DRAW);
		logWrite ("[OBJCT] VBO %d created for uvs\n", uvs_vbo);

		GLuint vao = 0;
		glGenVertexArrays (1, &vao);
		glBindVertexArray (vao);
		glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer (GL_ARRAY_BUFFER, uvs_vbo);
		glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);
		glEnableVertexAttribArray (1);

		glBindVertexArray (vao);
		object->vao_id = vao;

		logWrite ("[OBJCT] VAO %d created bound to VBO %d and VBO %d\n", vao, points_vbo, uvs_vbo);
	}

	return object;
}

int objectDestroy(Object * object)
{
	return 0;
}
