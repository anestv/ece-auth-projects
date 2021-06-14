function Normals = findVertNormals(R, F)
	
	numTriangles = size(F, 2);
	
	Normals = zeros(3, size(R, 2));
% 	triaglesContaining = zeros(1, size(R, 2));
	% we want Normals(:, verx_id) to be the avg of the normal vectors of
	% the triagles that include it. We store the sum in Normals, and the
	% number of triangles with the vertex in triaglesContaining.
	
	for k = 1:numTriangles
		vert_ids = F(:, k);
		side1 = R(:,vert_ids(2)) - R(:,vert_ids(1));
		side2 = R(:,vert_ids(3)) - R(:,vert_ids(1));
		
		% the normal vector of this triangle
		NofTriangle = cross(side1, side2);
		NofTriangle = NofTriangle / norm(NofTriangle); % normalize it
% 		Ntriag(:,k) = NofTriangle;
		
		Normals(:, vert_ids(1)) = NofTriangle + Normals(:, vert_ids(1));
		Normals(:, vert_ids(2)) = NofTriangle + Normals(:, vert_ids(2));
		Normals(:, vert_ids(3)) = NofTriangle + Normals(:, vert_ids(3));
		
% 		triaglesContaining(vert_ids(1)) = 1 + triaglesContaining(vert_ids(1));
% 		triaglesContaining(vert_ids(2)) = 1 + triaglesContaining(vert_ids(2));
% 		triaglesContaining(vert_ids(3)) = 1 + triaglesContaining(vert_ids(3));
	end
	
% 	if any(triaglesContaining ~= 0)
% 		warn('there is a vertex that no triangle uses');
% 	end
	
	% to find the average, divide the sum by the number
% 	Normals = Normals ./ repmat(triaglesContaining, 3, 1);
	
	% The output vectors should be unitary (monadiaia). But taking the
	% average does not preserve this. Example: ((1,0)+(0,1))/2=(.5,.5) is not unitary
	% Since we normalize each column, no need to divide by triaglesContaining
	Normals = normc(Normals);
end
