%  Gruppennummer:
%  Gruppenmitglieder:

%% Hausaufgabe 2
%  Bestimmung von Punktkorrespondenzen wischen Merkmalspunkten einer Stereo
%  Aufnahme.

%  Für die letztendliche Abgabe bitte die Kommentare in den folgenden Zeilen
%  enfernen und sicherstellen, dass alle optionalen Parameter über den
%  entsprechenden Funktionsaufruf fun('var',value) modifiziert werden können.


%% Bilder laden
Image1 = imread('szeneL.jpg');
IGray1 = rgb_to_gray(Image1);

Image2 = imread('szeneR.jpg');
IGray2 = rgb_to_gray(Image2);

%% Harris-Merkmale berechnen
Merkmale1 = harris_detektor(IGray1,'segment_length',9,'k',0.05,'min_dist',80,'N',20,'do_plot',true);
Merkmale2 = harris_detektor(IGray2,'segment_length',9,'k',0.05,'min_dist',80,'N',20,'do_plot',true);



%% Korrespondenzschätzung, last parameter is the threshold for the ncc
Korrespondenzen = punkt_korrespondenzen(IGray1,IGray2,Merkmale1,Merkmale2, 0.1);