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

#include "fonts.h"

FTGLfont *LoadFont(char *file){
  FTGLfont *font;
  font =  ftglCreateTextureFont(file);
  if(!font)
    {
        fprintf(stderr, "[FONT] Could not load font `%s'\n", file);
        return NULL;
    }
  ftglSetFontFaceSize(font, 20, 72);
     //ftglSetFontDepth(font, 10);
    //ftglSetFontOutset(font, 0, 3);
  ftglSetFontCharMap(font, ft_encoding_unicode);
  return font;
}
