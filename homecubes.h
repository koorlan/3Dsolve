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

// Only data here
const float homeCube[27][3] =
{
{-1.f,-1.f,-1.f },
{ 0.f,-1.f,-1.f },
{ 1.f,-1.f,-1.f },
{-1.f,-1.f, 0.f },
{ 0.f,-1.f, 0.f },
{ 1.f,-1.f, 0.f },
{-1.f,-1.f, 1.f },
{ 0.f,-1.f, 1.f },
{ 1.f,-1.f, 1.f },

{-1.f, 0.f,-1.f },
{ 0.f, 0.f,-1.f },
{ 1.f, 0.f,-1.f },
{-1.f, 0.f, 0.f },
{ 0.f, 0.f, 0.f },
{ 1.f, 0.f, 0.f },
{-1.f, 0.f, 1.f },
{ 0.f, 0.f, 1.f },
{ 1.f, 0.f, 1.f },

{-1.f, 1.f,-1.f },
{ 0.f, 1.f,-1.f },
{ 1.f, 1.f,-1.f },
{-1.f, 1.f, 0.f },
{ 0.f, 1.f, 0.f },
{ 1.f, 1.f, 0.f },
{-1.f, 1.f, 1.f },
{ 0.f, 1.f, 1.f },
{ 1.f, 1.f, 1.f }
};
