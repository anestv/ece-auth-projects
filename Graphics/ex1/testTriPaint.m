% To test the correct functionality of function triPaintFlat

C = [0 0 1];

for i = 1:100
    Img = ones(100, 100, 3);
    V = randi([1 100], 3, 2);
    Img = triPaintFlat(Img, V, C);
    
    Img(V(1,2), V(1,1), :) = [1 0 0];
    Img(V(2,2), V(2,1), :) = [1 0 0];
    Img(V(3,2), V(3,1), :) = [1 0 0];
    
    image(Img);
    pause
end
