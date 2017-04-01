function  Merkmale = harris_detektor(Image,varargin) 
% In dieser Funktion soll der Harris-Detektor implementiert werden, der
% Merkmalspunkte aus dem Bild extrahiert

% Bildgradienten mittels Sobel-Filter einlesen
[Fx,Fy] = sobel_xy(Image);
Merkmale = Image;
%% Berechne Harris-Matrix G(x)
% Eigenvektoren/-Werte von Image
tau = 0.1;
for row=1:size(Image,1)
    for col=1:size(Image,2)
        harris_matrix = [Fx(row,col)^2 Fx(row,col)*Fy(row,col); Fx(row,col)*Fy(row,col) Fy(row,col)^2];
        %[V,LAMBDA] = eig(harris_matrix);
        H = det(harris_matrix) - 0.05 * trace(harris_matrix)^2;
        if (H > tau)
            % Ecke
            Merkmale(row,col,1) = 255;
        elseif (H < -1*tau)
            % Kante
            Merkmale(row,col,2) = 255;
        end
    end
end

if (strcmp(varargin{1}, 'do_plot') && varargin{2})
    imshow(Merkmale);
end


%% 


end
