function [Projected, Depths] = projectCamera(w, cam_pos, cam_x, cam_y, points)
%PROJECTCAMERA Project object to CCD, given the camera's axes

    cam_z = cross(cam_x, cam_y); % ekswteriko ginomeno
    
    % allagh suntetagmenwn
    pointsCCS = systemTransform(points, cam_x, cam_y, cam_z, cam_pos);
    
    
    Depths = pointsCCS(3, :)'; % the z coordinate according to CCS
    
    % Metasxhmatismos provolis
    Projected(1,:) = w * pointsCCS(1,:) ./ pointsCCS(3,:); % x'=f/z * x
    Projected(2,:) = w * pointsCCS(2,:) ./ pointsCCS(3,:);
    
end
