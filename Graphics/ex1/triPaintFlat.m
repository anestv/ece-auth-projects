function Y = triPaintFlat(X, V, C)
% Y = TRIPAINTFLAT(X, V, C) Paints a triangle with a solid color
%
% X is a MxNx3 matrix which contains the canvas to be used. May already
% have stuff drawn. The output Y is also MxNx3.
% V is a matrix with 2 columns and 3 rows: each row has the coordinates of
% a vertex of the triangle
% C is a 3x3 matrix: each row has the color [r g b] of the corresponding
% vertex. The triangle's color is the mean color.

  % 277 seconds
  %tic
  
  % keep any previous things
  Y = X;
  
  % find the solid color, = average
  % mia grammh einai ta R,G,B gia mia korufh
  C = mean(C, 1); % gives a row vector, of the average R,G,B
  
  % Briskoume tis koryfes me mikrotero, mesaio kai megalutero y coordinate.
  sortedV = sortrows(V, [2 1]); % sort according to the y coordinate
  Vbot = sortedV(1,:);
  Vmid = sortedV(2,:);
  Vtop = sortedV(3,:);
  
  slope_bottop = (Vtop(2)-Vbot(2)) / (Vtop(1)-Vbot(1));
  slope_botmid = (Vmid(2)-Vbot(2)) / (Vmid(1)-Vbot(1));
  slope_midtop = (Vtop(2)-Vmid(2)) / (Vtop(1)-Vmid(1));
  
  
  % start from the x coord of the bottom vertex
  xmin = Vbot(1); xmax = Vbot(1);
  
  %% find which side is left and right
  if (slope_botmid*slope_bottop > 0) && (slope_botmid > slope_bottop)
      % sto idio tetarthmorio. se kathe periptosh, opoio exei
      % megalutero slope einai aristera
    update_which_slope = 'L';
  elseif (slope_botmid*slope_bottop < 0) && (slope_botmid < 0)
      % se alla tetarthmoria: opoio einai sto pano-deksia einai aristera
    update_which_slope = 'L';
  else
    update_which_slope = 'R';
  end
  
  if (update_which_slope == 'L')
    inv_slope_left = 1/slope_botmid;
    inv_slope_right= 1/slope_bottop;
  else
    inv_slope_left = 1/slope_bottop;
    inv_slope_right= 1/slope_botmid;
  end
  
  %% scanlines bottom to middle
  for y = Vbot(2) : (Vmid(2)-1)
    
    % correct rounding
    xmin_rnd = floor(xmin); % include the whole leftmost pixel: we are in the right triangle
    xmax_rnd = floor(xmax); % don't include the rightmost pixel: we are in the left triangle
    
    % paint pixels from xmin to xmax
    Y(y, xmin_rnd:xmax_rnd, :) = repmat(C, xmax_rnd-xmin_rnd+1, 1); % to have same dimensions

    % update xmin, xmax. Use real, not integer values.
    xmin = xmin + inv_slope_left;
    xmax = xmax + inv_slope_right;
    
  end
  
  %% middle special cases and update
  if (Vbot(2) == Vmid(2)) % there is a horizontal bottom side: paint it
    xmin = min(Vbot(1), Vmid(1)); % we want to paint it, as it belongs to the uppermost triangle
    xmax = max(Vbot(1), Vmid(1));
  elseif (Vtop(2) == Vmid(2)) % there is a horizontal top side: don't paint it
    return; % we are done
  end
  
  % update slope with midtop
  if (update_which_slope == 'L')
    inv_slope_left = 1/slope_midtop;
  else
    inv_slope_right= 1/slope_midtop;
  end
  
  %% middle to top scanlines
  for y = (Vmid(2)) : Vtop(2)
    
    % correct rounding
    xmin_rnd = floor(xmin); % include the whole leftmost pixel: we are in the right triangle
    xmax_rnd = floor(xmax); % don't include the rightmost pixel: we are in the left triangle
    
    % paint pixels from xmin to xmax
    Y(y, xmin_rnd:xmax_rnd, :) = repmat(C, xmax_rnd-xmin_rnd+1, 1); % to have same dimensions
    
    % update xmin, xmax. Use real, not integer values.
    xmin = xmin + inv_slope_left;
    xmax = xmax + inv_slope_right;
    
  end
  
end
