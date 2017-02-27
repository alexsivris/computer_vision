%  Gruppennummer: M19
%  Gruppenmitglieder:   Braun, Benjamin
%                       Fedjaev, Juri
%                       Schreiber, Philipp
%                       Sivris, Alexandros

%% Hausaufgabe 1
%  Einlesen und Konvertieren von Bildern sowie Bestimmung von 
%  Merkmalen mittels Harris-Detektor. 

%  Für die letztendliche Abgabe bitte die Kommentare in den folgenden Zeilen
%  enfernen und sicherstellen, dass alle optionalen Parameter über den
%  entsprechenden Funktionsaufruf fun('var',value) modifiziert werden können.


%% Bild laden
%% Teil I - Bildvorverarbeitung 
%  1. Bild in Graustufen umwandeln
Image = imread('szene.jpg');
Image = double(Image)/255; % In double konvertieren und Wertebereich anpassen
IGray = rgb_to_gray(Image);

% Graustufenbild anzeigen
imshow(IGray), title('Bild in Graustufen'); 

%% 2. Erste Ableitung mittels Sobel-Operator berechnen
[Fx Fy] = sobel_xy(IGray);
subplot(1,2,1), imshow(Fx), title('Ableitung in x-Richtung');
subplot(1,2,2), imshow(Fy), title('Ableitung in y-Richtung');

%% Harris-Merkmale berechnen
tic;
Merkmale = harris_detektor(IGray,'do_plot',true);
toc;
