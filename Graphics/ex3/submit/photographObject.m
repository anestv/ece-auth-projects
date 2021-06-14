function Im = photographObject(shader, f, C, K, u, bC, M, N, H, W, R, F, S, ka, kd, ks, ncoeff, Ia, I0)

	Normals = findVertNormals(R, F);
	
	[Projected, Depths] = projectCameraKu(f, C, K, u, R);

	P_2d = rasterize(Projected, M, N, H, W);
    %P_2d = P_2d';
    % transpose, to be compatible with paintObject()
	
	
	%% PAINT OBJECT
	% alternatively its own file, Im = paintObject(Vertices, F, Depths, shader, M, N, bC);
	num_Triangles = size(F, 2);
	%num_Vertices = size(Vertices, 1);
	if (shader == 1)
		shade = @shadeGouraud;
	else
		shade = @shadePhong;
	end
	
	% initialise canvas with background bC
	Im = ones(M, N, 3);
	Im(:,:,1) = bC(1);
	Im(:,:,2) = bC(2);
	Im(:,:,3) = bC(3);
	
	%% Find the avg depth of each triangle
	DepthsTriangles = [zeros(num_Triangles, 1) transpose(1:num_Triangles)];
	% 1st column: avg depth, 2nd column: triangle id
	
% 	for i = 1:num_Triangles
% 		% F(i, :) is a vector with the ids of the 3 vertices-koryfes of triangle i
% 		% Depths( F(i, :) ) is a vector with the depths of the 3 vertices
% 		DepthsTriangles(i, 1) = mean( Depths( F(:, i) ));
% 	end
	DepthsTriangles(:, 1) = mean( Depths( F ))'; % for loop is not needed
	
	DepthsTriangles = sortrows(DepthsTriangles, -1); % sort triangles by decreasing depth
	
	
	%% Paint the triangles
	
	for i = 1:num_Triangles
		% fprintf('%d / %d\n', i, num_Triangles);
		currTriangleID = DepthsTriangles(i, 2);
		currVertIDs = F(:, currTriangleID);
		
		p = P_2d(:, currVertIDs);
		
		% check if any vertex (in 2D) is outside the canvas
		if (any(p(:) <= 0) || any(p(:,1) > M) || any(p(:,2) > N))
			continue; % do not paint this triangle
		end
		
		% barykentro == 'centroid' == mean of triangle's vertices
		Pc = mean( R(:,currVertIDs) , 2); % mean of each row
		
		Im = shade(p, Normals(:,currVertIDs), Pc, C, S, ...
			 ka(:,currVertIDs), kd(:,currVertIDs), ks(:,currVertIDs), ...
			 ncoeff, Ia, I0, Im);
	end
	
end
