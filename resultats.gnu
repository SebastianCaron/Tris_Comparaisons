# pour avoir un titre sur le diagramme
set title 'Comparaison des algorithmes de tris'
# pour nommer l'axe des abscisses
set xlabel 'Taille de la liste'
# pour nommer l'axe des ordonnées
set ylabel 'Nombre doperations '
# la première courbe est tracée par plot
# en utilisant la colonne 1 pour les abscisses et la colonne 2 pour les ordonnées
plot 'resultats.dat' using 1:2 title 'Selection' with linespoints
# les courbes suivantes sont dessinées par l'instruction replot
replot 'resultats.dat' using 1:3 title 'Insertion' with linespoints
replot 'resultats.dat' using 1:4 title 'Bubbles' with linespoints
replot 'resultats.dat' using 1:5 title 'Fusion' with linespoints
replot 'resultats.dat' using 1:6 title 'Quicksort' with linespoints
replot 'resultats.dat' using 1:7 title 'Tas' with linespoints
# l'instruction suivante attend que l'utilisateur appuie sur une touche pour quitter l'application
pause -1
