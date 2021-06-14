function I = diffuseLight(P, N, kd, S, I0)

	n = size(S, 2); % num of light sources
	N = N(:); % make it vertical
	
	% L : monadiaia dianusmata kateuthinsis fotos
	L = S - repmat(P, 1, n); % = 3xn matrix, opws to S
	normL = sqrt(sum( L.^2 , 1));
	L = L ./ normL; % normalizes each column to have norm=1
	
	%f_att = min(1000*normL.^-2 , 1);
	% Too dark with this f_att
	f_att = 1;
	
	fNL = f_att .* (N' * L); % = 1xn vector. one column for each source
	
	INL = I0 * fNL'; % = 3x1 vector.
	% The equivalent I0*(N.L) of a single source
	
	I = INL .* kd;
	
	% I = min(I, 1); % must be <= 1
	% No need to check here. Is checked after adding the other Light functions
	I = max(I, 0); % must be >= 0
end
