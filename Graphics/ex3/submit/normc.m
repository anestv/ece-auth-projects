function M = normc(M)
% Normalises each column of the matric to have a norm of 1.
% Function normc already exists in a MATLAB toolbox.
% This implementation aims to copy its behaviour
% It is faster to use the toolbox's version of the function

	normM = sqrt(sum( M.^2 , 1));
	M = M ./ repmat(normM, size(M,1), 1);
end
