#include "fonts.h"

FTGLfont *LoadFont(char *file){
  FTGLfont *font;
  font =  ftglCreatePixmapFont(file);
  if(!font)
    {
        fprintf(stderr, "[FONT] Could not load font `%s'\n", file);
        return NULL;
    }
  ftglSetFontFaceSize(font, 80, 72);
     //ftglSetFontDepth(font, 10);
    //ftglSetFontOutset(font, 0, 3);
  ftglSetFontCharMap(font, ft_encoding_unicode);
  return font;
}
