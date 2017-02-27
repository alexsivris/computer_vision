function [T,R, lambdas, P1] = rekonstruktion(T1,T2,R1,R2, Korrespondenzen, K)
% Funktion zur Bestimmung der korrekten euklidischen Transformation, der
% Tiefeninformation und der 3D Punkte der Merkmalspunkte in Bild 1

tmp_R = [R1;R2];
tmp_T = [T1;T2];

% Loesungsmatrix - fuer jede R,T-Kombination
Sol = zeros(size(Korrespondenzen,2),4);
% Gleichungssystem Bild 1
M=zeros(size(Korrespondenzen,2),size(Korrespondenzen,2)+1);

combi_counter = 1;
for r=1:2
    for t=1:2
        for m=1:size(Korrespondenzen,2)
            x_1 = [Korrespondenzen(1:2,m);1];
            x_2 = [Korrespondenzen(3:4,m);1];
            
            % richtiges R,T bestimmen
            % Kreuzprodukt in Matrixschreibweise
            x_2_roof = [0 -x_2(3,1) x_2(2,1); x_2(3,1) 0 -x_2(1,1); -x_2(2,1) x_2(1,1) 0];
            M(m,m) = x_2_roof * tmp_R(r,1) * x_1;
            
            % letzte Spalte
            M(m,end) = x_2_roof * T(t,1);
        end
        
        % Loesung berechnen - Minimierungsproblem
        [U,S,V] = svd(M);
        Sol(:,combi_counter) = S(end,end) * V(:,end)';
        
        combi_counter = combi_counter + 1;
    end
end

% Suche beste Loesung (Kombination von R,T mit den meisten positiven
% Tiefen)
total_pos = zeros(1,size(Sol,2));
for m=1:size(Sol,2)
    total_pos(1,m) = length(find(Sol(:,m) > 0));
end

[,idx] = max(total_pos);
[r,c] = ind2sub(idx);

lambdas = Sol(:,c);

% richtiges R,T rausfinden
if (idx <=2)
    R = R1;
else
    R = R2;
end
if (idx == 2 || idx == 4)
    T = T2;
else
    T = T1;
end

% 3D-Raumpunkt
P1 = lambdas * [Korrespondenzen(1:2,:);1];
end