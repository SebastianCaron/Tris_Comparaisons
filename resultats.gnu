# pour avoir un titre sur le diagramme
set title 'Comparaison des algorithmes de tris'
# pour nommer l'axe des abscisses
set xlabel 'Taille de la liste'
# pour nommer l'axe des ordonnées
set ylabel 'Nombre doperations '
# la première courbe est tracée par plot
# en utilisant la colonne 1 pour les abscisses et la colonne 2 pour les ordonnées
plot 'resultats.dat' using 1:2 title 'Tri par Selection' with linespoints
# les courbes suivantes sont dessinées par l'instruction replot
replot 'resultats.dat' using 1:3 title 'Tri par Insertion' with linespoints
replot 'resultats.dat' using 1:4 title 'Tri à Bulle' with linespoints
replot 'resultats.dat' using 1:5 title 'Tri Fusion' with linespoints
replot 'resultats.dat' using 1:6 title 'Tri par tas' with linespoints
replot 'resultats.dat' using 1:7 title 'Quicksort' with linespoints
replot 'resultats.dat' using 1:8 title 'Tri à Peigne' with linespoints
replot 'resultats.dat' using 1:9 title 'Tri de Shell' with linespoints
replot 'resultats.dat' using 1:10 title 'Smoothsort' with linespoints
# l'instruction suivante attend que l'utilisateur appuie sur une touche pour quitter l'application
pause -1
