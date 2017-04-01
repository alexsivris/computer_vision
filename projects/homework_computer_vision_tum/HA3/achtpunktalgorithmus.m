function [EF] = achtpunktalgorithmus(Korrespondenzen,K)
% Diese Funktion berechnet die Essentielle Matrix oder Fundamentalmatrix
% mittels 8-Punkt-Algorithmus, je nachdem, ob die Kalibrierungsmatrix 'K'
% vorliegt oder nicht

% 8-Punktealgorithmus
if (nargin < 2)
    K = eye(3);
end
A=[];
for n=1:1:size(Korrespondenzen,2)
    point_x1 = K * [Korrespondenzen(1:2,n); 1];
    point_x2 = K * [Korrespondenzen(3:4,n); 1];
    A(end+1,:) = kron(point_x1,point_x2)';
end
% 1.Singulaerwertzerlegung: von A
[U,S,V] = svd(A);
G_stacked = V(:,9);
G = [G_stacked(1:3,1) G_stacked(4:6,1) G_stacked(7:9,1)];

% 2. Singulaerwertzerlegung: von G
[UG,SG,VG] = svd(G);

% EF berechnen
if (nargin < 2)
    % Fundamentalmatrix
    SG(end,:) = 0;
    EF = UG * SG * VG';
else
    % Essentielle Matrix
    EF = UG * [1 0 0;0 1 0;0 0 0] * VG';  
end

end