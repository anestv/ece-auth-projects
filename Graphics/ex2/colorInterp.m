function color = colorInterp(A, B, a, b, x)
% color = COLORINTERP(A, B, a, b, x)
% Returns an interpolation between the colors A and B (rows [r g b]),
% for the number x between a and b. Can work with a horizontal vector of
% many xs. Gives a matrix where each  column has the 3 colors of a point.
  
  % commented for speed
%  if ( nnz((x-a) .* (x-b) > 0)) % if there is even one x outside the range
%    error("In colorInterp, (at least one) x is outside of range a..b");
%  endif
  
  if (b == a) && (x == a) % when xmin = xmax = x
    ratio_of_B = 1; % can be any real num, as long as A==B
  else
    ratio_of_B = (x(:)-a) / (b-a); % convert x to vertical vector
  end
  
  color = repmat(A, length(ratio_of_B), 1) + ratio_of_B * (B-A); % gives matrix with 3 columns and length(x) rows
  
end
