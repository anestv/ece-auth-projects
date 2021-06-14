function I = specularLight(P, N, C, ks, ncoeff, S, I0)
	
	n = size(S, 2); % num of light sources
	N = N(:); % make it vertical
	
	% L : monadiaia dianusmata kateuthinsis fotos
	L = S - repmat(P, 1, n); % = 3xn matrix, opws to S
	L = normc(L); % normalizes each column to have norm=1
	
	NL = N' * L; % = 1xn vector. one column for each source
	
	% R = monadiaio dianysma idanikhs anaklashs
	R = 2*N*NL - L; % = 3xn matrix, opws to L

	V = C - P; % 3x1. To kanoniko viewing vector
	V = V / norm(V);
	
	RV = V' * R; % = 1xn vector. one column for each source
	
	IRV = I0 * (RV .^ ncoeff)' ; % = 3x1 vector.
	% The equivalent I0*(R.V)^ncoeff of a single source
	
	I = IRV .* ks;
	
	%I = min(I, 1); % must be <= 1
	% No need to check here. Is checked after adding the other Light functions
end
