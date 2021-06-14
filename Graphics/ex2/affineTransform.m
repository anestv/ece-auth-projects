function cq = affineTransform(cp, R, ct)
%AFFINETRANSFORM Perform an affine transform to points of a 3D space

    affine_matrix = [R  ct ; 0 0 0 1];

    cp_h = [cp ; ones(1, size(cp, 2))]; % omogeneis syntetagmenes = [cp ; 1]
    cq_h = affine_matrix * cp_h;
    
    cq = cq_h(1:3, :); % epanafora se kanonikes syntetagmenes
end
