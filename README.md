# 3Dsolve - snakepuzzle
*Korlan - achazot - dibi -KansassCityShuffle - agourd*

### TODO List

1. Rédaction au propre de l'algorithme de résolution du snake (doit être prêt pour la présentation de Jeudi 23/04)
2. Rédaction d'un diagramme UML (Anthony is on it)
3. Réalisation d'un PPT pour expliquer visuellement le fonctionnement de notre algo (oui/non ?)
4. Commencer l'implémentation d'un contexte graphique

### Cross plateform Compilation
####library needed:

* ftgl
* freetype2
* glfw3
* glew
* for windows ( on MinGW install pthreads-win32 package)

#Linux
Note : libglfw2 just enough if package not present on repo otherwise goto glfw website, download and compile library.
```
$ sudo apt-get install ftgl-dev libftgl2 libfreetype6 libfreetype6-dev libglfw-dev liglfw3 libglew-dev libglew1.10
```


#Osx
Note : cake brew command
```
$ brew install ftgl freetype pkg-config glew homebrew/versions/glfw3
```
