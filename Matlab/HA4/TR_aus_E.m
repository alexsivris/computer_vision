function [T1,R1,T2,R2] = TR_aus_E(E)
% In dieser Funktion sollen die moeglichen euklidischen Transformationen
% aus der Essentiellen Matrix extrahiert werden
[U,S,V] = svd(E);
RZ_PLUS = [0 -1 0;1 0 0;0 0 1];
RZ_MINUS = [0 1 0;-1 0 0;0 0 1];
R1 = U * R_PLUS' * V';
R2 = U * R_MINUS' * V';

T1 = U * R_PLUS * S * V';
T2 = U * R_MINUS * S * V';
end