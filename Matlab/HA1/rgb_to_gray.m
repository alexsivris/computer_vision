function [Gray_image] = rgb_to_gray(Image)
% Diese Funktion soll ein RGB-Bild in ein Graustufenbild umwandeln. Falls
% das Bild bereits in Graustufen vorliegt, soll es direkt zurückgegeben werden.

%% Bildgröße und -dimension bestimmen
[x y z] = size(Image);

%% Falls input bereits in Graustufen, Bild zurückgeben und return
if z == 1
    Gray_image = Image;
    return    
else

% Umrechnen des RGB-Bildes in Grauwerte
Gray_image = 0.299 * Image(:,:,1) + 0.587 * Image(:,:,2) + 0.114 * Image(:,:,3);

end

end
