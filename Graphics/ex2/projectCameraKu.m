function [Projected, Depths] = projectCameraKu(w, cam_pos, c_K, c_u, points)
%PROJECTCAMERAKU Project object to CCD, given the camera's target and up vector

    cam_z = c_K - cam_pos;
    cam_z = cam_z / norm(cam_z); % kanonikopoihsh, gia na einai monadiaio
    
    cam_y = c_u - cam_z * (c_u' * cam_z); % = u - provolh tou u sto cam_z
    cam_y = cam_y / norm(cam_y); % kanonikopoihsh, gia na einai monadiaio
    
    cam_x = cross(cam_y, cam_z); % ekswteriko ginomeno
    
    [Projected, Depths] = projectCamera(w, cam_pos, cam_x, cam_y, points);
    
end
