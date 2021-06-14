function Y = shadePhong(p, Vn, Pc, Cam, S, ka, kd, ks, ncoeff, Ia, I0, X)

  Y = X;
  
%   if any(isnan(V(:))) % if any element of V is NaN, don't paint
%       return
%   end
  
  % in ka, each column is for a vertex
  C = [Vn ; ka ; kd ; ks]'; % here each row is for a vertex
  
  % Briskoume tis koryfes me mikrotero, mesaio kai megalutero y coordinate.
  % Kratame ta xromata tous
  sortedV = sortrows([p' C], [2 1]); % sort according to the y coordinate
  C = sortedV(:, 3:end);
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
    
    % interpolate parameters of scanline: for x in xmin_rnd:xmax_rnd
    interpolated = colorInterp(C_xmin, C_xmax, xmin_rnd, xmax_rnd, xmin_rnd:xmax_rnd);
    % interpolate Vn,ka,kn,kd for all sides and all points in scanline
	Vni = interpolated(:, 1:3);
	kai = interpolated(:, 4:6);
	kdi = interpolated(:, 7:9);
	ksi = interpolated(:, 10:12);
	
	for i = 1:(xmax_rnd-xmin_rnd+1)
		% color the pixel
		Color = diffuseLight(Pc, Vni(i,:)', kdi(i,:)', S, I0) + ...
			specularLight(Pc, Vni(i,:)', Cam, ksi(i,:)', ncoeff, S, I0) + ...
			ambientLight(kai(i,:)', Ia);
		% paint the pixel
		Y(y, xmin_rnd+i-1, :) = min(Color, 1);
	end
	
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
    
    % interpolate parameters of scanline: for x in xmin_rnd:xmax_rnd
    interpolated = colorInterp(C_xmin, C_xmax, xmin_rnd, xmax_rnd, xmin_rnd:xmax_rnd);
    % interpolate Vn,ka,kn,kd for all sides and all points in scanline
	Vni = interpolated(:, 1:3);
	kai = interpolated(:, 4:6);
	kdi = interpolated(:, 7:9);
	ksi = interpolated(:, 10:12);
	
	for i = 1:(xmax_rnd-xmin_rnd+1)
		% color the pixel
		Color = diffuseLight(Pc, Vni(i,:)', kdi(i,:)', S, I0) + ...
			specularLight(Pc, Vni(i,:)', Cam, ksi(i,:)', ncoeff, S, I0) + ...
			ambientLight(kai(i,:)', Ia);
		% paint the pixel
		Y(y, xmin_rnd+i-1, :) = Color; % or min(Color, [1;1;1]);
	end
    
    % update xmin, xmax. Use real, not integer values.
    xmin = xmin + inv_slope_left;
    xmax = xmax + inv_slope_right;
    
  end
end

