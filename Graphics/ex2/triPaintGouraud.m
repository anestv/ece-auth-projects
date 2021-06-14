function Y = triPaintGouraud(X, V, C)
% Y = TRIPAINTGOURAUD(X, V, C) Paints a triangle with a gradient color
%
% X is a MxNx3 matrix which contains the canvas to be used. May already
% have stuff drawn. The output Y is also MxNx3.
% V is a matrix with 2 columns and 3 rows: each row has the coordinates of
% a vertex of the triangle
% C is a 3x3 matrix: each row has the color [r g b] of the corresponding
% vertex. The triangle is painted with a gradient of the vertices' colors.

  % 297 seconds
  
  % keep any previous things
  Y = X;
  
  if any(isnan(V(:))) % if any element of V is NaN, don't paint
      return
  end
  
  % Briskoume tis koryfes me mikrotero, mesaio kai megalutero y coordinate.
  % Kratame ta xromata tous
  sortedV = sortrows([V C], [2 1]); % sort according to the y coordinate
  C = sortedV(:, 3:5);
  sortedV = sortedV(:, 1:2);
  
  Vbot = sortedV(1,:);
  Vmid = sortedV(2,:);
  Vtop = sortedV(3,:);
  Cbot = C(1,:);
  Cmid = C(2,:);
  Ctop = C(3,:);
  
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
    
    % the below points are to be used together with Vbot in the 1st loop, and Vtop in the 2nd loop,
    % in order to find the color of the edges of the scanline. Before the 2nd loop, the 3 changes
    % to 1, as the 2nd_points are then used together with Vtop.
    left_2nd_point = 2; % middle  point will be used for color interpolation of left edge of scanline
    right_2nd_point= 3; % the top point will be used for color interpolation of right edge of scanline

  else
    inv_slope_left = 1/slope_bottop;
    inv_slope_right= 1/slope_botmid;
    
    left_2nd_point = 3; % the top point will be used for color interpolation of left edge of scanline
    right_2nd_point= 2; % middle  point will be used for color interpolation of right edge of scanline
  end

  
  %% scanlines bottom to middle
  for y = Vbot(2) : (Vmid(2)-1)
    
    % find the colors of the edges
    % color2_left_interp = C(left_2nd_point, :); color2_right_interp = C(right_2nd_point, :);
    % y_left_high = sortedV(left_2nd_point, 2);  y_right_high = sortedV(right_2nd_point, 2);
    C_xmin = colorInterp(Cbot, C(left_2nd_point, :), Vbot(2), sortedV(left_2nd_point, 2), y);
    C_xmax = colorInterp(Cbot, C(right_2nd_point,:), Vbot(2), sortedV(right_2nd_point,2), y);
    
    % correct rounding
    xmin_rnd = floor(xmin); % include the whole leftmost pixel: we are in the right triangle
    xmax_rnd = floor(xmax); % don't include the rightmost pixel: we are in the left triangle
    
    % find colors of pixels of scanline: for x in xmin_rnd:xmax_rnd
    pixel_colors = colorInterp(C_xmin, C_xmax, xmin_rnd, xmax_rnd, xmin_rnd:xmax_rnd);
    
    % paint pixels from xmin to xmax
    Y(y, xmin_rnd:xmax_rnd, :) = pixel_colors;
    
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
    
    right_2nd_point = 1; % So, the right side is bottom-top. In the 2nd loop,
    % the top point is always used in the colorInterp calculation
  else
    inv_slope_right= 1/slope_midtop;
    
    left_2nd_point = 1;
  end
  
  
  %% middle to top scanlines
  for y = (Vmid(2)) : Vtop(2)
    
    % find the colors of the edges
    % color2_left_interp = C(left_2nd_point, :); color2_right_interp = C(right_2nd_point, :);
    % y_left_high = sortedV(left_2nd_point, 2);  y_right_high = sortedV(right_2nd_point, 2);
    C_xmin = colorInterp(Ctop, C(left_2nd_point, :), Vtop(2), sortedV(left_2nd_point, 2), y);
    C_xmax = colorInterp(Ctop, C(right_2nd_point,:), Vtop(2), sortedV(right_2nd_point,2), y);

    % correct rounding
    xmin_rnd = floor(xmin); % include the whole leftmost pixel: we are in the right triangle
    xmax_rnd = floor(xmax); % don't include the rightmost pixel: we are in the left triangle
    
    % find colors of pixels of scanline: for x in xmin_rnd:xmax_rnd
    pixel_colors = colorInterp(C_xmin, C_xmax, xmin_rnd, xmax_rnd, xmin_rnd:xmax_rnd);
    
    % paint pixels from xmin to xmax
    Y(y, xmin_rnd:xmax_rnd, :) = pixel_colors;
    
    % update xmin, xmax. Use real, not integer values.
    xmin = xmin + inv_slope_left;
    xmax = xmax + inv_slope_right;
    
  end
  
  
end
