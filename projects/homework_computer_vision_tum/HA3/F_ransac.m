function [Korrespondenzen_robust] = F_ransac(Korrespondenzen,varargin)
% Diese Funktion implementiert den RANSAC-Algorithmus zur Bestimmung von
% robusten Korrespondenzpunktpaaren

% Input parser
P = inputParser;

% Liste der optionalen Parameter
% Fensterl?nge
P.addOptional('epsilon', 0.5, @isnumeric)
% Minimal geforderte Korrelation
P.addOptional('p', 0.99, @(x)isnumeric(x));
% Plot ein/aus
P.addOptional('tolerance', 20, @(x)isnumeric(x));

% Lese den Input
P.parse(varargin{:});

% Extrahiere die Variablen aus dem Input-Parser
epsilon   = P.Results.epsilon;
p         = P.Results.p;
tolerance = P.Results.tolerance;
% RANSAC
k = 8;
n_points = size(Korrespondenzen,2);
e3 = [0 0 1]';
e3_hat = [0 -1 0; 1 0 0; 0 0 0];
d_sampson = @(x1,x2,F) (x2'*F*x1)^2/(norm(e3_hat*F*x1)+norm(x2'*F*e3_hat));
Korrespondenzen_robust = [];
global_d = 1e15;
s = floor((log(1-p)/(log(1-(1-epsilon)^k))))
for i=1:s
    % Randomly select k points
    idx = randperm(n_points,k); 
    sample = Korrespondenzen(:,idx);

    % Calculate fundamental matrix
    x1 = [sample(1:2,:);ones([1 k])];
    x2 = [sample(3:4,:);ones([1 k])];
    F = achtpunktalgorithmus(sample);

    % Calculate distnace to form consensus set
    consensus = [];
    combined_d = 0;
    for j = 1:8
        d = d_sampson(x1(:,j),x2(:,j),F);
        if d < tolerance
            consensus(:,end+1) = ([x1(1:end-1,j)' x2(1:end-1,j)']');
            combined_d = combined_d + d;
        end
    end

    % Update global set if necessary
    if length(consensus) > length(Korrespondenzen_robust)
        Korrespondenzen_robust = consensus;
    elseif length(consensus) == length(Korrespondenzen_robust)
         if combined_d < global_d;
            Korrespondenzen_robust = consensus;
            global_d = combined_d;
         end
    end
end
     
end