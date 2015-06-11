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

#include "renderer.h"

//UP,DOWN,LEFT,RIGHT,FRONT,BACK
const float dir2lr[6][3] =
{
{  0.f ,  0.f ,  1.f  },
{  0.f ,  0.f , -1.f  },
{  0.f ,  1.f ,  0.f  },
{  0.f ,  0.f ,  0.f  },
{  0.f , -1.f ,  0.f  },
{  0.f ,  1.f ,  0.f  }
};

#include "homecubes.h"


extern const float dir2vec[6][3];


void* renderer ( void *arg )
{
	//! [1] OpenGl setup
	logWrite ("[RENDR] Renderer started\n");

	Context* context = (Context*) arg;

	glfwMakeContextCurrent ( context->window );


	GLuint vpID = glGetUniformLocation(context->snake_program, "VP");
	GLuint vpID2 = glGetUniformLocation(context->picking_program, "VP");
	GLuint wID = glGetUniformLocation(context->snake_program, "W");
	GLuint wID2 = glGetUniformLocation(context->picking_program, "W");
	GLuint pickcolorID = glGetUniformLocation(context->picking_program, "colorID");
	GLuint timeID = glGetUniformLocation(context->snake_program, "time");
	GLuint alphaID = glGetUniformLocation(context->snake_program, "alpha");
	mat4x4 WMat;
	mat4x4 PVMat;
	mat4x4 viewMat;
	mat4x4 perMat;

	context->ratio = ((float)context->screen_width)/(float)context->screen_height;
	float last_time=127.f;


	#ifndef _WIN32
		struct timespec time1;
		time1.tv_sec = 0;
		time1.tv_nsec = 100000;
		struct timespec time2;
	#endif
	//! [1]

	//! [2] Player initialization
	Player* curPlayer;
	if (context->playmode == PM_PLAY) curPlayer = gplayer;
	else curPlayer = gsolver;


	int i,j;
	int solidCheck = 0;
	float scoef = 1.f;
	int solidStart = 0;
	float r_angle =  -M_PI/4;
	vec2 foffsetd;
	vec2 foffseta;
	foffsetd[0] = 0.8f;
	foffsetd[1] = 0.8f;
	foffseta[0] = 0.2f;
	foffseta[1] = 0.2f;
	FTGLfont* titleFont = LoadFont ("fonts/recharge bd.ttf");
	ftglSetFontFaceSize(titleFont, 100, 72);
	FTGLfont* pressFont = LoadFont ("fonts/recharge bd.ttf");
	ftglSetFontFaceSize(pressFont, 22, 72);
	FTGLfont* textFont = LoadFont ("fonts/Tahoma.ttf");
	ftglSetFontFaceSize(textFont, 22, 72);
	FTGLfont* stextFont = LoadFont ("fonts/Tahoma.ttf");
	ftglSetFontFaceSize(stextFont, 16, 72);
	//! [2]

	//! [3] Renderer loop
	while (app->running && !glfwWindowShouldClose (context->window))
	{
		int cubesNb = app->snake->length;
		int solidEnd = cubesNb;
		//! [4] FPS limitation control
		float cur_time = glfwGetTime ();
		float fps = 1/((cur_time-last_time));
		if (fps>60.0f)
		{
			#ifdef _WIN32
			Sleep(1);
			#else
			nanosleep (&time1, &time2);
			#endif
			continue;
		}
		else last_time = cur_time;
		//printf ("%f\n", fps); // display fps
		//! [4]


		// Set OpenGl viewport to the entire window
		glViewport (0, 0, context->screen_width, context->screen_height);
		float myX = -(context->screen_width / 2.f) + 20.f;
		switch (app->state)
		{
		case AS_HOME:

			// Affichage de l'écran d'accueil

			glClearColor( 0.1f, 0.1f, 0.1f, 1.f );
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Eye-candy
			glUseProgram (context->snake_program);
			mat4x4_identity(viewMat);
			mat4x4_identity(perMat);
			mat4x4_mul (PVMat, perMat, viewMat);
			glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);
			glUniform1f(alphaID, 1.0f);
			glEnable (GL_DEPTH_TEST);
			mat4x4_look_at(viewMat, (vec3){3,4.5,3},
						(vec3){0,1.5,0},
						(vec3){0,1,0});
			mat4x4_perspective(perMat, context->camera->fov, context->ratio, F_NEAR, F_FAR);
			mat4x4_mul (PVMat, perMat, viewMat);
			glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);
			#ifdef __APPLE__
				glBindVertexArrayAPPLE (context->cube_mesh->vao_id);
			#else
				glBindVertexArray (context->cube_mesh->vao_id);
			#endif

			for ( i=0; i <= 27; i++ )
			{
				mat4x4_identity ( WMat );
				mat4x4_rotate_Y ( WMat, WMat, 0.1f*glfwGetTime());
				mat4x4_translate_in_place ( WMat, homeCube[i][0], homeCube[i][1], homeCube[i][2]);
				//mat4x4_scale3d(WMat, WMat, 0.9f);
				glUniformMatrix4fv ( wID, 1, GL_FALSE, &WMat[0][0] );

				if (i%2==0) glBindTexture(GL_TEXTURE_2D, context->dwoodtex);
				else glBindTexture(GL_TEXTURE_2D, context->lwoodtex);

				glDrawArrays(GL_TRIANGLES, 0, context->cube_mesh->nb_faces);
			}

			// Afficher texte ici
			glUseProgram (0);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-context->screen_width*0.5f,context->screen_width*0.5f,-context->screen_height*0.5f,context->screen_height*0.5f,0,1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glColor4f (0.8f, 0.8f, 0.8f,1.f);
			glTranslatef (-250.f, 100.f,0.f);
			ftglRenderFont( titleFont, "3Dsolve", FTGL_RENDER_ALL);
			glLoadIdentity();
			glTranslatef (-275.f, -220.f,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f - (0.35f*(1+cos(4*glfwGetTime()))));
			ftglRenderFont( pressFont, "Appuyez sur une touche pour continuer", FTGL_RENDER_ALL);

		break;
		case AS_LOAD:

			glClearColor( 0.1f, 0.1f, 0.1f, 1.f );
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

						// Eye-candy
			glUseProgram (context->snake_program);
			mat4x4_identity(viewMat);
			mat4x4_identity(perMat);
			mat4x4_mul (PVMat, perMat, viewMat);
			glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);
			glUniform1f(alphaID, 1.0f);
			glEnable (GL_DEPTH_TEST);
			mat4x4_look_at(viewMat, (vec3){0.000001f,15.f,0.f},
						(vec3){0.f,0.f,0.f},
						(vec3){0.f,1.f,0.f});
			mat4x4_perspective(perMat, context->camera->fov, context->ratio, F_NEAR, F_FAR);
			mat4x4_mul (PVMat, perMat, viewMat);
			glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);
			#ifdef __APPLE__
				glBindVertexArrayAPPLE (context->cube_mesh->vao_id);
			#else
				glBindVertexArray (context->cube_mesh->vao_id);
			#endif

			for ( i=0; i <= 16; i++ )
			{
				mat4x4_identity ( WMat );
				mat4x4_rotate_Y ( WMat, WMat, ((float)i) * (M_PI/8) + (0.25*glfwGetTime()) );
				mat4x4_translate_in_place ( WMat, 10.f, 0, 0);
				mat4x4_scale3d(WMat, WMat, 2 + cos(((float)i) * (M_PI/8)+ 2 * glfwGetTime()));
				glUniformMatrix4fv ( wID, 1, GL_FALSE, &WMat[0][0] );

				if (i%2==0) glBindTexture(GL_TEXTURE_2D, context->dwoodtex);
				else glBindTexture(GL_TEXTURE_2D, context->lwoodtex);

				glDrawArrays(GL_TRIANGLES, 0, context->cube_mesh->nb_faces);
			}


			glUseProgram (0);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-context->screen_width*0.5f,context->screen_width*0.5f,-context->screen_height*0.5f,context->screen_height*0.5f,0,1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef (-50.f, 0.f,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f - (0.35f*(1+cos(4*glfwGetTime()))));
			ftglRenderFont( pressFont, "Calcul...", FTGL_RENDER_ALL);

		break;
		case AS_HELP:

			glClearColor( 0.1f, 0.1f, 0.1f, 1.f );
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram (0);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-context->screen_width*0.5f,context->screen_width*0.5f,-context->screen_height*0.5f,context->screen_height*0.5f,0,1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef (-80.f, 250.f,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f);
			ftglRenderFont( pressFont, "Menu d'aide", FTGL_RENDER_ALL);

			glTranslatef (myX, 170,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f);
			ftglRenderFont( textFont, "Flèche directionnelle droite : prochaine étape de la solution", FTGL_RENDER_ALL);
			glLoadIdentity();
			glTranslatef (myX, 110,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f);
			ftglRenderFont( textFont, "Flèche directionnelle gauche : étape précédente de la solution", FTGL_RENDER_ALL);
			glLoadIdentity();
			glTranslatef (myX, 50,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f);
			ftglRenderFont( textFont, "Espace : activer/désactiver la rotation du snake", FTGL_RENDER_ALL);
			glLoadIdentity();
			glTranslatef (myX, 0,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f);
			ftglRenderFont( textFont, "Entrer : Vue \"explosée\"", FTGL_RENDER_ALL);
			glLoadIdentity();
			glTranslatef (myX, -50,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f);
			ftglRenderFont( textFont, "Echape : Quitter l'application", FTGL_RENDER_ALL);

			glLoadIdentity();
			glTranslatef (-325.f, -220.f,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f - (0.35f*(1+cos(4*glfwGetTime()))));
			ftglRenderFont( pressFont, "Appuyez sur une touche pour quitter ce menu", FTGL_RENDER_ALL);

		break;
		case AS_ABOUT:

			glClearColor( 0.1f, 0.1f, 0.1f, 1.f );
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram (0);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-context->screen_width*0.5f,context->screen_width*0.5f,-context->screen_height*0.5f,context->screen_height*0.5f,0,1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef (-80.f, 250.f,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f);
			ftglRenderFont( pressFont, "A Propos", FTGL_RENDER_ALL);

			glLoadIdentity();
			glTranslatef (myX, 150.f,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f);
			ftglRenderFont( textFont, "3DSolve est une application réalisée dans le cadre du projet", FTGL_RENDER_ALL);
			glLoadIdentity();
			glTranslatef (myX, 120.f,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f);
			ftglRenderFont( textFont, "application de première année à l'INSA-CVL.", FTGL_RENDER_ALL);

			glLoadIdentity();
			glTranslatef (myX, 70.f,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f);
			ftglRenderFont( pressFont, "Auteurs :", FTGL_RENDER_ALL);
			glLoadIdentity();
			glTranslatef (myX, 40.f,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f);
			ftglRenderFont( textFont, "L.Aubry <lisa.aubry@insa-cvl.fr>", FTGL_RENDER_ALL);
			glLoadIdentity();
			glTranslatef (myX, 10.f,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f);
			ftglRenderFont( textFont, "A.Chazot <alban.chazot@insa-cvl.fr>", FTGL_RENDER_ALL);
			glLoadIdentity();
			glTranslatef (myX, -20.f,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f);
			ftglRenderFont( textFont, "K.Colas <korlan.colas@insa-cvl.fr>", FTGL_RENDER_ALL);
			glLoadIdentity();
			glTranslatef (myX, -50.f,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f);
			ftglRenderFont( textFont, "A.Gourd <anthony.gourd@insa-cvl.fr>", FTGL_RENDER_ALL);

			glLoadIdentity();
			glTranslatef (myX, -100.f,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f);
			ftglRenderFont( pressFont, "Encadrant :", FTGL_RENDER_ALL);
			glLoadIdentity();
			glTranslatef (myX, -130.f,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f);
			ftglRenderFont( textFont, "P.Clemente <patrice.clemente@insa-cvl.fr>", FTGL_RENDER_ALL);

			glLoadIdentity();
			glTranslatef (-325.f, -220.f,0.f);
			glColor4f (1.f, 1.f, 1.f, 1.f - (0.35f*(1+cos(4*glfwGetTime()))));
			ftglRenderFont( pressFont, "Appuyez sur une touche pour quitter ce menu", FTGL_RENDER_ALL);
		break;
		case AS_GAME:

			if (context->playmode == PM_PLAY)
				curPlayer = gplayer;
			else
				curPlayer = gsolver;


			if ( context->spread == 1 && scoef > 0.6f )
				scoef -= 0.01f;
			else if ( context->spread == 0 && scoef < 1.f )
				scoef += 0.01f;


			//calcul du "segment" de snake auquel appartient le cube selectionné
			curPlayer->segEnd = cubesNb;
			if (curPlayer->selected!=-1 && curPlayer->selected!=cubesNb)
				for ( i=curPlayer->selected+1; i < cubesNb; i++ )
					if ( app->snake->units[i] == CORNER)
					{
						curPlayer->segEnd = i;
						break;
					}

			//! [5] Color picking (selection d'un cube à la souris)
			if ( context->drawpick == 1 || (bhv_flags&BHV_DRAWPICK)==BHV_DRAWPICK )
			{

				glClearColor( 1.0f, 1.0f, 1.0f, 1.f );
				glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glUseProgram (context->picking_program);
				mat4x4_look_at(viewMat, context->camera->eye,
						context->camera->target, context->camera->up);
				mat4x4_perspective(perMat, context->camera->fov, context->ratio, F_NEAR, F_FAR);
				mat4x4_mul (PVMat, perMat, viewMat);
				glUniformMatrix4fv(vpID2, 1, GL_FALSE, &PVMat[0][0]);

				#ifdef __APPLE__
					glBindVertexArrayAPPLE (context->cube_mesh->vao_id);
				#else
					glBindVertexArray (context->cube_mesh->vao_id);
				#endif

				for ( i=0; i <= curPlayer->segEnd; i++ )
				{
					glUniform3f(pickcolorID, ((float)i)/255.f, 1.f, 0.f);

					mat4x4_identity ( WMat );
					mat4x4_mul (WMat, WMat, curPlayer->realCubePos[i]);
					mat4x4_mul (WMat, WMat, curPlayer->realCubeRot[i]);
					mat4x4_scale3d(WMat, WMat, scoef);
					//mat4x4_scale3d(WMat, WMat, 0.6f);

					glUniformMatrix4fv ( wID2, 1, GL_FALSE, &WMat[0][0] );

					glDrawArrays(GL_TRIANGLES, 0, context->cube_mesh->nb_faces);
				}
				//==========render menu button==========
				mat4x4_identity(viewMat);
				mat4x4_identity(perMat);
				mat4x4_mul (PVMat, perMat, viewMat);
				glUniformMatrix4fv(vpID2, 1, GL_FALSE, &PVMat[0][0]);

				Menu * currentMenu = NULL ;
				Menu * menuCaller = NULL;
				Menu * tmpMenu = NULL;
				Item * itemCaller = NULL ;
				currentMenu = app->menu;
				int id = 0;
				for (j = 0; j < app->menuDepth; j++) {
					drawPickMenuTemplate(context,currentMenu,&menuCaller,&itemCaller,&id, viewMat, perMat, PVMat, WMat, vpID2, pickcolorID,  wID2);
					if(menuCaller == NULL && itemCaller == NULL) break;
					tmpMenu = currentMenu;
					currentMenu = menuCaller;
					menuCaller = tmpMenu;
					tmpMenu = NULL;
				}

				//==========Boutons==========
				#ifdef __APPLE__
					glBindVertexArrayAPPLE (context->rbutton_mesh->vao_id);
				#else
					glBindVertexArray (context->rbutton_mesh->vao_id);
				#endif
				glUniform3f(pickcolorID, 255.f, 255.f, 0.f);
				float xoffset = 0.6f;
				float yoffset = -0.8f;
				mat4x4_identity(WMat);
				mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);
				mat4x4_scale_aniso(WMat, WMat, 1/context->ratio * 0.15f, 0.15f, 0.f);
				glUniformMatrix4fv(wID2, 1, GL_FALSE, &WMat[0][0]);
				glDrawArrays(GL_TRIANGLES, 0, context->rbutton_mesh->nb_faces);

				#ifdef __APPLE__
					glBindVertexArrayAPPLE (context->lbutton_mesh->vao_id);
				#else
					glBindVertexArray (context->lbutton_mesh->vao_id);
				#endif
				glUniform3f(pickcolorID, 255.f, 255.f, 1.f/255.f);
				xoffset = -0.6f;
				yoffset = -0.8f;
				mat4x4_identity(WMat);
				mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);
				mat4x4_scale_aniso(WMat, WMat, 1/context->ratio * 0.15f, 0.15f, 0.f);
				glUniformMatrix4fv(wID2, 1, GL_FALSE, &WMat[0][0]);
				glDrawArrays(GL_TRIANGLES, 0, context->lbutton_mesh->nb_faces);

				glFlush();
				glFinish();
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				unsigned char data[4];
				glReadPixels(gxpos, context->screen_height-gypos, 1, 1,
						GL_RGBA, GL_UNSIGNED_BYTE, data);

				int oldsel = gplayer->selected;
				gplayer->selected = -1;
				if( (data[1]==255?-1:data[1]) != -1){
					app->itemSelected = (data[1]==255?-1:data[1]);
					app->buttonPushed = -1;
				}else if((data[0]==255?-1:data[0]) != -1){
					app->itemSelected = -1;
					app->buttonPushed = -1;
					gplayer->selected = (data[0]==255?-1:data[0]);
				}else if((data[2]==255?-1:data[2]) != -1){
					gplayer->selected = oldsel;
					app->itemSelected = -1;
					app->buttonPushed = (data[2]==255?-1:data[2]);
				}

				//printf("selected: %d\n", data[0]);

				context->drawpick = 0;
				if ((bhv_flags&BHV_DRAWPICK)==BHV_DRAWPICK)
				{
					glfwSwapBuffers(context->window);
					continue;
				}

			}
			//! [5]

			//==========snake 3d==========
			glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram (context->snake_program);
			mat4x4_identity(viewMat);
			mat4x4_identity(perMat);
			mat4x4_look_at(viewMat, context->camera->eye,
					context->camera->target, context->camera->up);
			mat4x4_perspective(perMat, context->camera->fov, context->ratio, F_NEAR, F_FAR);
			mat4x4_mul (PVMat, perMat, viewMat);
			glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);

			for (	solidCheck = 0;
				solidCheck <= (curPlayer->segEnd != cubesNb?1:0);
				solidCheck++)
			{
				if (solidCheck == 0 && curPlayer->segEnd != cubesNb)
				{
					solidStart = curPlayer->segEnd+1;
					solidEnd = cubesNb;
				}
				else if (solidCheck == 1 && curPlayer->segEnd != cubesNb)
				{
					solidStart = 0;
					solidEnd = curPlayer->segEnd+1;
				}
				else
				{
					solidStart = 0;
					solidEnd = cubesNb;
				}

				for ( i=solidStart; i < solidEnd; i++ )
				{
					glUniform1f(timeID, glfwGetTime());

					if ( i > curPlayer->segEnd ) glDisable (GL_DEPTH_TEST);
					else glEnable (GL_DEPTH_TEST);

					mat4x4_identity ( WMat );
					mat4x4_mul (WMat, WMat, curPlayer->realCubePos[i]);
					mat4x4_mul (WMat, WMat, curPlayer->realCubeRot[i]);
					mat4x4_scale3d(WMat, WMat, scoef);
					if (curPlayer->selected == i )
						mat4x4_scale3d(WMat, WMat, 0.8f + (0.2f * abs(cos(4*(glfwGetTime())))));
					else mat4x4_scale3d(WMat, WMat, 0.97f);



					if ( i < cubesNb-1)
					{
						mat4x4 WMat2;
						mat4x4_dup (WMat2, WMat);
						mat4x4_scale_aniso(WMat2, WMat2,
							(dir2vec[curPlayer->steps[i].dir][0]==0?1:1/scoef),
							(dir2vec[curPlayer->steps[i].dir][1]==0?1:1/scoef),
							(dir2vec[curPlayer->steps[i].dir][2]==0?1:1/scoef));
						mat4x4_rotate(WMat2, WMat2,
							dir2lr[curPlayer->steps[i].dir][0],
							dir2lr[curPlayer->steps[i].dir][1],
							dir2lr[curPlayer->steps[i].dir][2], 3.1415 * 0.5f);
						if (curPlayer->steps[i].dir == LEFT)
							mat4x4_rotate_Y (WMat2, WMat2, -3.1415 * 0.5f);
						glUniformMatrix4fv ( wID, 1, GL_FALSE, &WMat2[0][0] );
						glUniform1f(alphaID, (i > curPlayer->segEnd?0.2f:1.0f));
						glBindTexture(GL_TEXTURE_2D, context->linktex);

						#ifdef __APPLE__
							glBindVertexArrayAPPLE (context->link_mesh->vao_id);
						#else
							glBindVertexArray (context->link_mesh->vao_id);
						#endif
						glDrawArrays(GL_TRIANGLES, 0, context->link_mesh->nb_faces);
					}
					glUniform1f(alphaID, (i > curPlayer->segEnd?0.2f:1.0f));
					glUniformMatrix4fv ( wID, 1, GL_FALSE, &WMat[0][0] );

					if (curPlayer->selected == i )
						mat4x4_scale3d(WMat, WMat, 1/(0.8f + (0.2f * abs(cos(4*(glfwGetTime()))))));

					if (i%2==0) glBindTexture(GL_TEXTURE_2D, context->dwoodtex);
					else glBindTexture(GL_TEXTURE_2D, context->lwoodtex);

					#ifdef __APPLE__
						glBindVertexArrayAPPLE (context->cube_mesh->vao_id);
					#else
						glBindVertexArray (context->cube_mesh->vao_id);
					#endif

					if ((bhv_flags&BHV_NOCUBE)!=BHV_NOCUBE)
						glDrawArrays(GL_TRIANGLES, 0, context->cube_mesh->nb_faces);
				}
			}
			glEnable (GL_DEPTH_TEST);

			//forme finale en haut a droite
			glViewport (foffsetd[0] * context->screen_width, foffsetd[1] * context->screen_height,
					foffseta[0] * context->screen_width, foffseta[1] * context->screen_height);
			mat4x4_identity(viewMat);
			mat4x4_identity(perMat);
			//mat4x4_look_at(viewMat, context->camera->eye,
			//		context->camera->target, context->camera->up);
			vec3 vol_offset;
			vol_offset[0]=(app->snake->volume.max.x%2==0 ? app->snake->volume.max.x /2 - 0.5f : (app->snake->volume.max.x)/2);
			vol_offset[1]=(app->snake->volume.max.y%2==0 ? app->snake->volume.max.y /2 - 0.5f : (app->snake->volume.max.y)/2);
			vol_offset[2]=(app->snake->volume.max.z%2==0 ? app->snake->volume.max.z /2 - 0.5f : (app->snake->volume.max.z)/2);
			int dist = 3;
			mat4x4_look_at(viewMat, (vec3){vol_offset[0]+dist,vol_offset[1]+dist,vol_offset[2]+dist},
						vol_offset,
						(vec3){ 0.f ,1.f ,0.f });
			mat4x4_perspective(perMat, 1.1f, context->ratio, F_NEAR, F_FAR);
			mat4x4_mul (PVMat, perMat, viewMat);
			glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);


			#ifdef __APPLE__
				glBindVertexArrayAPPLE (context->cube_mesh->vao_id);
			#else
				glBindVertexArray (context->cube_mesh->vao_id);
			#endif

			for (i=0;i<app->snake->length;i++)
			{
				mat4x4_identity ( WMat );
				mat4x4_rotate_Y ( WMat, WMat, glfwGetTime() * 0.4f );
				mat4x4_translate_in_place (WMat, -vol_offset[0], -vol_offset[1], -vol_offset[2]);
				mat4x4_mul (WMat, WMat, curPlayer->finishedCubePos[i]);
				glUniformMatrix4fv ( wID, 1, GL_FALSE, &WMat[0][0] );

				if (i%2==0) glBindTexture(GL_TEXTURE_2D, context->dwoodtex);
				else glBindTexture(GL_TEXTURE_2D, context->lwoodtex);
				glDrawArrays(GL_TRIANGLES, 0, context->cube_mesh->nb_faces);
			}


			glViewport (0, 0, context->screen_width, context->screen_height);


			//==========snake a plat==========
			mat4x4_identity(viewMat);
			mat4x4_identity(perMat);
			mat4x4_mul (PVMat, perMat, viewMat);
			glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);
			glUniform1f(alphaID, 1.0f);
			#ifdef __APPLE__
				glBindVertexArrayAPPLE (context->square_mesh->vao_id);
			#else
				glBindVertexArray (context->square_mesh->vao_id);
			#endif

			float xoffset = -(app->snake->length*0.3333333f);
			float yoffset = -0.75f * 20.f;
			for ( i=0; i <= app->snake->length-1; i++ )
			{
				if (i%2==0) glBindTexture(GL_TEXTURE_2D, context->dwoodtex);
				else glBindTexture(GL_TEXTURE_2D, context->lwoodtex);

				mat4x4_identity(WMat);
				mat4x4_scale_aniso(WMat, WMat, 1/context->ratio * 0.05f, 0.05f, 0.f);
				mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);
				mat4x4_rotate_Z(WMat, WMat, -r_angle);
				mat4x4_translate_in_place( WMat, curPlayer->flatCubePos[i][0], curPlayer->flatCubePos[i][2], 0);
				if (i==curPlayer->selected)
					mat4x4_scale3d(WMat, WMat, ((0.9f*abs(cos(4*glfwGetTime())))) );
				glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);

				glDrawArrays(GL_TRIANGLES, 0, context->square_mesh->nb_faces);
			}

			//==========Boutons==========
			#ifdef __APPLE__
				glBindVertexArrayAPPLE (context->rbutton_mesh->vao_id);
			#else
				glBindVertexArray (context->rbutton_mesh->vao_id);
			#endif
			glBindTexture(GL_TEXTURE_2D, context->rbutton_tex);

			xoffset = 0.6f;
			yoffset = -0.8f;
			mat4x4_identity(WMat);
			mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);
			mat4x4_scale_aniso(WMat, WMat, 1/context->ratio * 0.15f, 0.15f, 0.f);
			glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, context->rbutton_mesh->nb_faces);

			#ifdef __APPLE__
				glBindVertexArrayAPPLE (context->lbutton_mesh->vao_id);
			#else
				glBindVertexArray (context->lbutton_mesh->vao_id);
			#endif
			glBindTexture(GL_TEXTURE_2D, context->lbutton_tex);

			xoffset = -0.6f;
			yoffset = -0.8f;
			mat4x4_identity(WMat);
			mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);
			mat4x4_scale_aniso(WMat, WMat, 1/context->ratio * 0.15f, 0.15f, 0.f);
			glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, context->lbutton_mesh->nb_faces);


			// textes d'infos
			glUseProgram (0);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-context->screen_width*0.5f,context->screen_width*0.5f,-context->screen_height*0.5f,context->screen_height*0.5f,0,1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glLoadIdentity();
			glTranslatef (-30.f, -275.f,0.f);
			glColor4f ( 1.f, 1.f, 1.f, 1.f );
			if (context->playmode==PM_PLAY)
				ftglRenderFont( stextFont, "Mode: Jeu", FTGL_RENDER_ALL);
			else ftglRenderFont( stextFont, "Mode: Solution", FTGL_RENDER_ALL);
			if (context->errorAlpha>0)
			{
				glLoadIdentity();
				glTranslatef ( -105.f, 0.f, 0.f );
				glColor4f ( 1.f, 0.f, 0.f, context->errorAlpha );
				context->errorAlpha -= 0.0075f;
				ftglRenderFont( pressFont, "Pas de solution", FTGL_RENDER_ALL);
			}
			if (context->winAlpha>0)
			{
				glLoadIdentity();
				glTranslatef ( -115.f, 0.f, 0.f );
				glColor4f ( 0.f, 1.f, 0.f, context->winAlpha );
				context->winAlpha -= 0.0035f;
				static float baseAngle=0;
				if (context->winAlpha>0.5f)
				{
					baseAngle = (baseAngle == 0?context->camera->angle[0]:baseAngle);
					context->camera->angle[0]=baseAngle+cos(((context->winAlpha-0.5f)*2.f)*M_PI)*4;
					scoef = 0.5f + 0.5f * abs(cos(glfwGetTime()));
				} else baseAngle = 0;
				ftglRenderFont( pressFont, "Casse-tête résolu !", FTGL_RENDER_ALL);
			}

			//==========menu==========

			 	mat4x4_identity(viewMat);
				mat4x4_identity(perMat);
				mat4x4_mul (PVMat, perMat, viewMat);
				glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);

				Menu * currentMenu = NULL ;
				Menu * menuCaller = NULL;
				Menu * tmpMenu = NULL;
				Item * itemCaller = NULL ;
				currentMenu = app->menu;
				for (j = 0; j < app->menuDepth; j++) {
					drawMenuTemplate(context,currentMenu,&menuCaller,&itemCaller,  viewMat, perMat, PVMat, WMat, vpID, alphaID,  wID);
					if(menuCaller == NULL && itemCaller == NULL) break;
					tmpMenu = currentMenu;
					currentMenu = menuCaller;
					menuCaller = tmpMenu;
					tmpMenu = NULL;
				}


			//==========texte menu==========
			currentMenu = NULL ;
			menuCaller = NULL;
			tmpMenu = NULL;
			itemCaller = NULL ;
			currentMenu = app->menu;
			for (j = 0; j < app->menuDepth; j++) {
				drawMenuText(context,currentMenu,&menuCaller,&itemCaller);
				if(menuCaller == NULL && itemCaller == NULL) break;
				tmpMenu = currentMenu;
				currentMenu = menuCaller;
				menuCaller = tmpMenu;
				tmpMenu = NULL;

			}

		break;
		default:
		break;
		}

		//Actualisation de l'ecran
		glfwSwapBuffers (context->window);

	}

	ftglDestroyFont(titleFont);
	ftglDestroyFont(pressFont);
	ftglDestroyFont(textFont);
	ftglDestroyFont(stextFont);

	app->running = 0;
	//! [3]

	logWrite ("[RENDR] Renderer stopped\n");

	return NULL;
}

void drawMenuTemplate(Context *context, Menu *menu,Menu **menuCaller,Item **itemCaller ,mat4x4 viewMat,mat4x4 perMat,mat4x4 PVMat,mat4x4 WMat,unsigned int vpID,unsigned int alphaID, unsigned int wID){
	float xoffset = 0.f;
	float yoffset = 0.f;
	float accumulator = 0.f;
	int i=0;

	glUseProgram (context->snake_program);
	#ifdef __APPLE__
	glBindVertexArrayAPPLE (menu->mesh->vao_id);
	#else
	glBindVertexArray (menu->mesh->vao_id);
	#endif
	glUniform1f(alphaID, 1.0f);


	if(menu->type == COLUMN){
		xoffset = -1.f +  menu->margin[0] - menu->bboxRel[0];
		yoffset = 1.f - menu->bboxRel[1] - menu->margin[1];

		if(*itemCaller != NULL && *menuCaller != NULL){
			xoffset += 2*(*menuCaller)->margin[0] - (*menuCaller)->bboxRel[0] + (*menuCaller)->bboxRel[2] ;
			yoffset -= (*menuCaller)->margin[1];
		}

		glBindTexture(GL_TEXTURE_2D, context->menutex);
		mat4x4_identity(WMat);
		mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);
		mat4x4_scale_aniso(WMat, WMat, menu->bboxRel[0], menu->bboxRel[1] , 0.f);

		glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0,menu->mesh->nb_faces);
		accumulator = 0.f;
			for ( i = 0; i < menu->size; i++) {


				if(menu->item[i]->menu != NULL && menu->item[i]->menu->state == OPEN){
						*itemCaller = menu->item[i];
						*menuCaller = menu->item[i]->menu;
				}
				mat4x4_identity(WMat);
				mat4x4_scale3d(WMat, WMat, 1.f);
				mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);
				if (i == 0)
					accumulator =  menu->bboxRel[1] - menu->item[i]->bboxRel[1] - 2*(menu->item[i]->margin[1]/context->screen_height);
				else
					accumulator -= 2*(menu->item[i]->margin[1]/context->screen_height);
				mat4x4_translate_in_place( WMat, 0.f ,accumulator , 0);
				mat4x4_scale_aniso(WMat, WMat, menu->bboxRel[0], menu->bboxRel[1] , 0.f);
				mat4x4_scale_aniso(WMat, WMat, 1.f, (menu->item[i]->bboxRel[1]/menu->bboxRel[1])*2.f , 0.f);

				glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);
				if(i == menu->opened){
					glBindTexture(GL_TEXTURE_2D, context->itemtex);
					glUniform1f(alphaID, 0.5f);
					glDrawArrays(GL_TRIANGLES, 0, menu->mesh->nb_faces);
					glBindTexture(GL_TEXTURE_2D, context->menutex);
					accumulator -= (menu->item[i]->bboxRel[1] - menu->item[i]->bboxRel[3]) + 2*(menu->item[i]->margin[3]/context->screen_height);
					continue;
				}
				//glBindTexture(GL_TEXTURE_2D, context->itemtex);
				glUniform1f(alphaID, 0.0f);
				glDrawArrays(GL_TRIANGLES, 0, menu->mesh->nb_faces);
				accumulator -= (menu->item[i]->bboxRel[1] - menu->item[i]->bboxRel[3]) + 2*(menu->item[i]->margin[3]/context->screen_height);
			}
	}else if(menu->type == ROW){
		//do stuff
	}
}

void drawPickMenuTemplate(struct context *context, Menu *menu,Menu **menuCaller,Item **itemCaller,int *id,mat4x4 viewMat,mat4x4 perMat,mat4x4 PVMat,mat4x4 WMat,unsigned int vpID,unsigned int pickcolorID, unsigned int wID){
	float xoffset = 0.f;
	float yoffset = 0.f;
	float accumulator = 0.f;
	int i=0;

	#ifdef __APPLE__
	glBindVertexArrayAPPLE (menu->mesh->vao_id);
	#else
	glBindVertexArray (menu->mesh->vao_id);
	#endif



	if(menu->type == COLUMN){


		xoffset = -1.f +  menu->margin[0] - menu->bboxRel[0];
		yoffset = 1.f - menu->bboxRel[1] - menu->margin[1];

		if(*itemCaller != NULL && *menuCaller != NULL){
			xoffset += 2*(*menuCaller)->margin[0] - (*menuCaller)->bboxRel[0] + (*menuCaller)->bboxRel[2] ;
			yoffset -= (*menuCaller)->margin[1];
		}

		mat4x4_identity(viewMat);
		mat4x4_identity(perMat);
		mat4x4_mul (PVMat, perMat, viewMat);
		glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);

		#ifdef __APPLE__
		glBindVertexArrayAPPLE (menu->mesh->vao_id);
		#else
		glBindVertexArray (menu->mesh->vao_id);
		#endif

		mat4x4_identity(WMat);
		mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);
		mat4x4_scale_aniso(WMat, WMat, menu->bboxRel[0], menu->bboxRel[1] , 0.f);
		mat4x4_scale_aniso(WMat, WMat, menu->bboxRel[0], menu->bboxRel[1] , 0.f);

		glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);
		accumulator = 0.f;
			for ( i = 0; i < menu->size; i++) {
				if(menu->item[i]->menu != NULL && menu->item[i]->menu->state == OPEN){
						*itemCaller = menu->item[i];
						*menuCaller = menu->item[i]->menu;
				}
				glUniform3f(pickcolorID,1.f, (float)((*id))/255.f, 0.f);
				*id += 1 ;
				mat4x4_identity(WMat);
				mat4x4_scale3d(WMat, WMat, 1.f);
				mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);
				if (i == 0)
					accumulator =  menu->bboxRel[1] - menu->item[i]->bboxRel[1] - 2*(menu->item[i]->margin[1]/context->screen_height);
				else
					accumulator -= 2*(menu->item[i]->margin[1]/context->screen_height);
				mat4x4_translate_in_place( WMat, 0.f ,accumulator , 0);
				mat4x4_scale_aniso(WMat, WMat, menu->bboxRel[0], menu->bboxRel[1] , 0.f);
				mat4x4_scale_aniso(WMat, WMat, 1.f, (menu->item[i]->bboxRel[1]/menu->bboxRel[1] * 2.0f) , 0.f);  //2.0f coef d'augmentation de la hitbox
				glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);
				glDrawArrays(GL_TRIANGLES, 0,menu->mesh->nb_faces);
				accumulator -= (menu->item[i]->bboxRel[1] - menu->item[i]->bboxRel[3]) + 2*(menu->item[i]->margin[3]/context->screen_height);
			}
	}else if(menu->type == ROW){
		//do stuff
	}

}


void drawMenuText(Context *context, Menu *menu,Menu **menuCaller,Item **itemCaller){
	float accumulator = 0.f;
	int i = 0;
	float xoffset = 0.f;
	float yoffset = 0.f;

	glUseProgram (0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-context->screen_width/2,context->screen_width/2,-context->screen_height/2,context->screen_height/2,0,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(*itemCaller != NULL && *menuCaller != NULL){
		xoffset += 2*(*menuCaller)->margin[0] - (*menuCaller)->bboxRel[0] + (*menuCaller)->bboxRel[2] ;//+ (*itemCaller)->bboxRel[2];
		yoffset -= (*menuCaller)->margin[1];
	}

	accumulator = 0.f;

	for ( i = 0;  i < menu->size; i++) {
		if(menu->item[i]->menu != NULL && menu->item[i]->menu->state == OPEN){
				*itemCaller = menu->item[i];
				*menuCaller = menu->item[i]->menu;
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef( (xoffset ) *(context->screen_width/2),
									(yoffset )*(context->screen_height/2),0.f);
		glTranslatef( (-1.f  + menu->margin[0] ) *(context->screen_width/2),
									(1.f  -  menu->margin[1] )*(context->screen_height/2),0.f);  //go to the top left of the menu
		accumulator += menu->item[i]->margin[1] + (menu->item[i]->bbox[3]-menu->item[i]->bbox[1]);
		glTranslatef(menu->item[i]->margin[0],-accumulator,0.f);
		accumulator += menu->item[i]->margin[3] ;
		glScalef(abs(1.f- menu->item[i]->bboxRel[2]),abs(1.f-menu->item[i]->bboxRel[1]),0.f);  //Fit the beast
		glColor4f (menu->item[i]->descriptor.color.r, menu->item[i]->descriptor.color.g,menu->item[i]->descriptor.color.b,menu->item[i]->descriptor.color.a);
		ftglRenderFont(menu->item[i]->descriptor.font, menu->item[i]->descriptor.name, FTGL_RENDER_ALL);
	}

}
