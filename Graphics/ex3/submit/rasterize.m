function P_rast = rasterize(Points, M, N, H, W)
%RASTERIZE Convert points' coordinates to discrete pixel positions
%   Points, P_rast = 2xn matrix

    %% Translate inches to pixel dimensions
    
    pixelsPerInchX = N/W;
    pixelsPerInchY = M/H;
    
    P_rast(1, :) = -Points(1, :) * pixelsPerInchX + N/2;
    P_rast(2, :) = -Points(2, :) * pixelsPerInchY + M/2;
    % pollaplasiazw me -1 giati vgainei panw-katw kai deksia-aristera
    % add N/2 so that pixels are not <0
    
    %% Round to integer values
    
    P_rast = round(P_rast);
    
    %% Pixel values between 1 and N/M
	if (any(P_rast(:) <= 0) || any((P_rast(1, :) > M) | (P_rast(2, :) > N)) )
		warning('Some points are outside the image!');
	end

end
