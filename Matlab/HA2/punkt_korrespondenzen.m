function [Korrespondenzen] = punkt_korrespondenzen(I1,I2,Mpt1,Mpt2,tresh,varargin)
% In dieser Funktion sollen die extrahierten Merkmalspunkte aus einer
% Stereo-Aufnahme mittels NCC verglichen werden um Korrespondenzpunktpaare
% zu ermitteln.
%% Input parser
P = inputParser;

% Liste der optionalen Parameter
% Lï¿½nge des Bildsegments
P.addOptional('window_length', 30, @isnumeric)
% Minimaler Korrelationswert
P.addOptional('min_corr', 1, @isnumeric)

% Plot ein/aus
P.addOptional('do_plot', false, @islogical);

% Lese den Input
P.parse(varargin{:});

% Extrahiere die Variablen aus dem Input-Parser
window_length   = P.Results.window_length;
do_plot         = P.Results.do_plot;
min_corr        = P.Results.min_corr;

%% Normalize images

% extraction of feature points
window_side = floor(window_length/2);
feature_patches_1 = patch_extraction(Mpt1,I1,window_side);
feature_patches_2 = patch_extraction(Mpt2,I2,window_side);

%% a) window length
if mod(window_length, 2) == 0
    window_length = window_length+1;
    message = ['The window side length was even. It has been changed to the next higher integer length.\nNew window length: ', num2str(window_length), '.\n'];
    fprintf(message);
end
N = window_length^2; % calculate number of window elements

%% Berechne NCC der beiden Bilder

%iterate over all patches

Korrespondenzen = [];

for fp1=1:length(feature_patches_1)
    
    %initiate ncc variable
    ncc_current = 0;
    fp12_match = 0;
    
    feature_patches_1(:,:,fp1) = normal(feature_patches_1(:,:,fp1));
    
    for fp2=1:length(feature_patches_2)
        
        feature_patches_2(:,:,fp2) = normal(feature_patches_2(:,:,fp2));
        
        tmp_ncc = ncc(feature_patches_1(:,:,fp1),feature_patches_2(:,:,fp2));
        if(tmp_ncc > ncc_current && tmp_ncc > tresh)
            ncc_current = tmp_ncc;
            fp12_match = fp2;
        end
    end
    
    if(fp12_match > 0)
        Korrespondenzen(:,end+1) = [Mpt1(1, fp1); Mpt1(2, fp1); Mpt2(1, fp2); Mpt2(2, fp2)];
    end
end

    function ncc_val = ncc(patch1, patch2)
        ncc_val = (1/(N-1))*trace(double(patch1)'*double(patch2));
    end



    function im_n = normal(im_patch)
        patch_mean = mean(im_patch(:));
        patch_sigma = std(double(im_patch(:)));
        im_n = 1/patch_sigma .* (im_patch - patch_mean);
    end
end