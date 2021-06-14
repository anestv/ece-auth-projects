
load 'duck_hw1.mat'

% disable "divide by 0" warnings in octave
if (exist('OCTAVE_VERSION')) 
  warning('off', 'Octave:divide-by-zero');
end

Image = paintObject(V_2d, F, C, D, 'flat');

imwrite(Image, 'duckFlat.jpg');
