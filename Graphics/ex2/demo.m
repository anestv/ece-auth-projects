clc
clear

%% Load data %%
load('hw2.mat');

if (size(V, 1) ~= 3)
    V = V'; % oi synarthseis ftiaxthkan gia 3xK opws leei h ekfwnhsh
end
num_points = size(V, 2);

disp 'Note: each call to paintObject() can take ~5 minutes';

%% Step 0 -Initial position 
% 0.1 Photograph object with photographObject
[V_2d, Depths] = photographObject(V, M, N, H, W, w, cv, ck, cu);

% 0.2 Paint object with ObjectPainter with gouraud
I0 = paintObject(V_2d, F, C, Depths, 'gouraud');

% Save result
imwrite(I0, '0.jpg');

%% Step 1 - Translate by t1
% 1.1 Apply translation
V = V + repmat(t1, 1, num_points);

% 1.2 Photograph object with photographObject
[V_2d, Depths] = photographObject(V, M, N, H, W, w, cv, ck, cu);

% 1.3 Paint object with ObjectPainter with gouraud
I1 = paintObject(V_2d, F, C, Depths, 'gouraud');

% Save result
imwrite(I1, '1.jpg');

%% Step 2 - Rotate by theta around given axis
% 2.1 Apply rotation
% de xreiazetai metatopish, giati o aksonas perna apo to O
R = rotationMatrix(theta, g);
V = affineTransform(V, R, [0;0;0]);


% 2.2 Photograph object with photographObject
[V_2d, Depths] = photographObject(V, M, N, H, W, w, cv, ck, cu);

% 2.3 Paint object with ObjectPainter with gouraud
I2 = paintObject(V_2d, F, C, Depths, 'gouraud');

% Save result
imwrite(I2, '2.jpg');

%% Step 3 - Translate back
% 3.1 Apply translation
V = V + repmat(t2, 1, num_points);

% 3.2 Photograph object with photographObject
[V_2d, Depths] = photographObject(V, M, N, H, W, w, cv, ck, cu);

% 3.3 Paint object with ObjectPainter with gouraud
I3 = paintObject(V_2d, F, C, Depths, 'gouraud');

% Save result
imwrite(I3, '3.jpg');
