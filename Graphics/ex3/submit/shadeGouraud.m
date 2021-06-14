function Y = shadeGouraud(p, Vn, Pc, C, S, ka, kd, ks, ncoeff, Ia, I0, X)

	% find colors of vertices
	for i = 1:3
		% find color of vertex i
		Color(:,i) = diffuseLight(Pc, Vn(:,i), kd(:,i), S, I0) + ...
			specularLight(Pc, Vn(:,i), C, ks(:,i), ncoeff, S, I0) + ...
			ambientLight(ka(:,i), Ia);
	end
	
	Color = min(Color, ones(3)); % must be <= 1
	
	% use old triPaintGoureaud
	Y = triPaintGouraud(X, p', Color');
end
