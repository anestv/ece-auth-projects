function [P_2d, Depths] = photographObject(points, M, N, H, W, w, cam_pos, c_K, c_u)

    [Projected, Depths] = projectCameraKu(w, cam_pos, c_K, c_u, points);

    P_2d = rasterize(Projected, M, N, H, W);
    
    P_2d = P_2d';
    % transpose, to be compatible with paintObject()
end
