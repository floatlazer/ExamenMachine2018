# Traitement d'images animées par convolution

Le but de l'examen est de paralléliser un code qui permet d'appliquer une série de filtre sur une séquence d'images correspondantes à une animation.  

Ces images portent toutes la même base pour le nom suivi d'un numéro et de l'extension du fichier. Par exemple, 
`data/simpson/homer-simpson-animated-gif-7` sera le nom de base pour la série d'image  


    data/simpson/homer-simpson-animated-gif-7-0.png
    data/simpson/homer-simpson-animated-gif-7-1.png
    ...
    data/simpson/homer-simpson-animated-gif-7-25.png

En entrée, l'utilisateur du logiciel rentre donc le nom de base de la séquence d'image ( exemple `data/simpson/homer-simpson-animated-gif-7` ) suivi de la série des filtres qu'il veut appliquer.
Les filtres mis-en-oeuvres pour l'examen sont les suivants :  
--------------------------------------------------------------
- renforce_bord : Renforce les bords des objets dans l'image
- detect_bord   : Ne garde que les bords des objets dans l'image
- flou          : applique un filtre rendant l'image plus flou
- contrast      : renforce la netteté de l'image
- repoussage    : donne un petit effet 3D sur les bords des objets de l'image
- gauss3        : Permet d'éliminer les bruits parasites de l'image d'origine.
--------------------------------------------------------------

Ces filtres sont mis en oeuvres à l'aide d'une convolution, c'est à dire qu'à chaque pixel de l'image, on applique une matrice qui va convoler avec les valeurs des voisins de ce pixel et la valeur du pixel lui-même :

p_{i,j} = \sum_{k,l=0,n} A_{k,l}.p_{i+k-n/2,j+l-n/2}

Pour une image en niveau de gris, la valeur du pixel est directement son intensité. Pour une image couleur, il faut transformer la couleur du pixel décrite par défaut dans l'espace RGB ( Rouge-Vert-Bleu ) dans l'espace HSV ( Teinte - Saturation - Valeur ) puis travailler sur la valeur ( l'intensité ) dans cette espace avant de revenir dans l'espace RGB.

*** Votre travail ***
*********************

Votre travail consistera à paralléliser à l'aide des threads ou d'OpenMP ( au choix ) le traitement de chaque image par les filtres, puis de paralléliser à l'aide de MPI le traitement de toutes les images de la séquence en prenant soin du cas où le nombre d'image n'est pas divisible par le nombre de processus.

Bon courage !
