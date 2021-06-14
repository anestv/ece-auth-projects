function R = rotationMatrix(theta, u)
%ROTATIONMATRIX Epistrefei ton pinaka peristrofhs gyrw apo aksona u, kata gwnia theta
%   theta: gwnia peristrofhs (se rad)
%   u: monadiaio dianysma toy aksona peristrofhs (poy perna apo to O)
    
    u_norm = sqrt(u' * u);
    if (u_norm ~= 1)
        warning('Function rotationMatrix: to dianusma u den einai monadiaio');
        u = u / u_norm; % kanonikopoihsh
    end
    
    tmp = sin(theta) * [0, -u(3), u(2); u(3), 0, -u(1); -u(2), u(1), 0];
    
    R = tmp + cos(theta)*eye(3) + (1-cos(theta))*(u*u');
end
